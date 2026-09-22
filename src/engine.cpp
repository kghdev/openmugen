#include "global.h"

/*
================================================================================
Represents the main fighting engine in which
all fighters are represented and the stages

================================================================================
*/

CEngine::CEngine()
    : m_pMemManager(NULL),
      m_pVideoSystem(NULL),
      m_pTimer(NULL),
      m_pVMp1(NULL),
      m_pVMp2(NULL),
      m_adventurePhase(ADVENTURE_WALK),
      m_stage(0),
      m_phaseTime(0),
      m_worldX(0),
      m_opponentHealth(0),
      m_opponentMaxHealth(0),
      m_teamSize(1),
      m_teamHealthPercent(100),
      m_qteTime(0),
      m_impactTime(0),
      m_specialTime(0),
      m_sceneVariant(0),
      m_qteActive(false),
      m_qteResolved(false),
      m_victoryScene(false) {
    for (int i = 0; i < 5; ++i) m_recruited[i] = false;
}

CEngine::~CEngine() {
    delete m_pVMp1;
    delete m_pVMp2;
}

/*
================================================================================
Init the engine call the function once at the start
of OpenMugen
================================================================================
*/
void CEngine::InitEngine(CMemManager* m, CVideoSystem* v, CGameTimer* t) {
    // Set All pinter to CEngine class
    m_pMemManager  = m;
    m_pVideoSystem = v;
    m_pTimer       = t;

    // Set all needed pointer to need class by class player
    player1.SetPointers(v, m->GetAllocater(P1), t);
    strcpy(player1.myPlayerConst.PlayerData.szPlayerName, "player1");
    player2.SetPointers(v, m->GetAllocater(P2), t);
    strcpy(player2.myPlayerConst.PlayerData.szPlayerName, "player2");
    player3.SetPointers(v, m->GetAllocater(P3), t);
    strcpy(player3.myPlayerConst.PlayerData.szPlayerName, "player3");
    player4.SetPointers(v, m->GetAllocater(P4), t);
    strcpy(player4.myPlayerConst.PlayerData.szPlayerName, "player4");

    // get the virtual machines
    m_pVMp1 = new CVirtualMachine;
    m_pVMp2 = new CVirtualMachine;

    // Set players to the VM
    m_pVMp1->SetPlayers(&player1, &player2);
    m_pVMp2->SetPlayers(&player2, &player1);

    // now set the VM to the player
    player1.SetVM(m_pVMp1);
    player2.SetVM(m_pVMp2);

    /*Set the ground limit of the player
     This value should be read out the stage def*/
    player1.SetGroundValue(220);
    player2.SetGroundValue(220);

    // this values also should read out of the stage def
    player1.SetPos(70, 220);
    player2.SetPos(240, 220);

    player1.LoadPlayer("");
    player2.LoadPlayer("");

    player1.SetKeyBoard(P1);
    player2.SetKeyBoard(P2);

    player2.FaceLeft();
    player1.SetDebug(false);
    player1.SetPos(20, 220);
    player2.SetPos(280, 220);
    BeginStage();
}

/*
================================================================================
Updates all the engine members and draw them to the screen
Handles also AI and player movement
================================================================================
*/
void CEngine::RunEngine() {
    RunAdventure();
}

bool CEngine::IsAdventureInputPressed() const {
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    return keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_UP] ||
           keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A];
}

bool CEngine::IsAttackPressed() const {
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    return keys[SDL_SCANCODE_Y] || keys[SDL_SCANCODE_U] || keys[SDL_SCANCODE_H] ||
           keys[SDL_SCANCODE_J];
}

void CEngine::BeginStage() {
    m_adventurePhase = ADVENTURE_WALK;
    m_phaseTime       = 0;
    m_worldX          = m_stage * 1000;
    m_qteActive       = false;
    m_qteResolved     = false;
    m_victoryScene    = false;
    m_impactTime      = 0;
    m_specialTime     = 0;
    m_sceneVariant    = m_stage % 6;
    player1.SetPos(20, 220);
    player2.SetPos(280, 220);
}

void CEngine::BeginFight() {
    m_adventurePhase     = ADVENTURE_FIGHT;
    m_phaseTime           = 0;
    m_qteTime             = 0;
    m_qteActive           = false;
    m_qteResolved         = false;
    m_impactTime          = 0;
    m_specialTime         = 0;
    m_sceneVariant        = m_stage % 6;
    m_opponentMaxHealth   = (m_stage == 12) ? 1500 : ((m_stage == 2) ? 1200 : 1000);
    m_opponentHealth      = m_opponentMaxHealth;
    player1.SetPos(70, 220);
    player2.SetPos(240, 220);
}

void CEngine::ResolveQte(bool success) {
    m_qteActive   = false;
    m_qteResolved = true;
    if (success) {
        m_opponentHealth -= (m_stage == 12) ? 180 : 120;
        m_teamHealthPercent += 2;
        m_impactTime = 8;
    } else {
        m_teamHealthPercent -= 5;
        m_impactTime = 5;
    }
}

void CEngine::RunAdventure() {
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    ++m_phaseTime;
    if (m_impactTime > 0) --m_impactTime;
    if (m_specialTime > 0) --m_specialTime;

    if (m_adventurePhase == ADVENTURE_WALK) {
        if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]) {
            m_worldX += 3;
            int screenX = 20 + (m_worldX % 280);
            player1.SetPos((s16)screenX, 220);
        }
        if (m_worldX >= (m_stage * 1000) + 300) {
            m_adventurePhase = ADVENTURE_SCENE;
            m_phaseTime       = 0;
        }
    } else if (m_adventurePhase == ADVENTURE_SCENE) {
        /* Storyboard beats are deliberately timed so each trigger is visible. */
        if (m_phaseTime >= 150) {
            if (m_stage == 11) {
                m_adventurePhase = ADVENTURE_ESCAPE;
                m_phaseTime       = 0;
            } else {
                BeginFight();
            }
        }
    } else if (m_adventurePhase == ADVENTURE_FIGHT) {
        ++m_qteTime;
        if (!m_qteActive && m_qteTime >= 120) {
            m_qteActive   = true;
            m_qteResolved = false;
            m_qteTime     = 0;
        }
        if (m_qteActive && IsAdventureInputPressed()) ResolveQte(true);
        if (m_qteActive && m_qteTime > 90) ResolveQte(false);
        if (IsAttackPressed() && (m_phaseTime % 8 == 0)) {
            m_opponentHealth -= 35;
            m_impactTime = 5;
            if (m_phaseTime % 48 == 0) m_specialTime = 24;
        }

        /* The mini-boss gains power at half health and marks the team reward. */
        if (m_stage == 2 && m_opponentHealth <= 600 && m_opponentHealth > 0 &&
            m_teamHealthPercent < 130) {
            m_teamHealthPercent = 130;
            m_opponentHealth += 100;
        }

        if (m_opponentHealth <= 0) {
            if (m_stage != 2 && m_stage != 12 && m_teamSize < 5) {
                m_recruited[m_teamSize] = true;
                ++m_teamSize;
            } else if (m_stage != 2 && m_stage != 12) {
                m_recruited[m_stage % 5] = true;
            }
            m_victoryScene = true;
            m_adventurePhase = ADVENTURE_SCENE;
            m_phaseTime = 0;
        }
    } else if (m_adventurePhase == ADVENTURE_ESCAPE) {
        if ((keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D])) {
            m_worldX += 4;
            player1.SetPos((s16)(20 + (m_worldX % 280)), 220);
        }
        if (m_phaseTime >= 10800 || m_worldX >= (m_stage * 1000) + 500) {
            ++m_stage;
            BeginStage();
        }
    }

    if (m_adventurePhase == ADVENTURE_SCENE && m_victoryScene && m_phaseTime >= 90) {
        ++m_stage;
        if (m_stage >= 13) {
            m_adventurePhase = ADVENTURE_COMPLETE;
            m_phaseTime = 0;
        } else {
            BeginStage();
        }
    }

    if (m_adventurePhase != ADVENTURE_COMPLETE) {
        player1.UpDatePlayer();
        player2.UpDatePlayer();
    }
    DrawAdventureScene();
    player1.DrawPlayer();
    player2.DrawPlayer();
    DrawCombatEffects();
    DrawAdventureHud();
}

void CEngine::DrawAdventureScene() {
    int pulse = (m_phaseTime % 30) / 10;
    int scroll = (m_worldX / 4) % 320;

    if (m_adventurePhase == ADVENTURE_SCENE || m_adventurePhase == ADVENTURE_COMPLETE) {
        m_pVideoSystem->DrawRect(0, 0, 320, 240, 8, 10, 24);
        m_pVideoSystem->DrawRect(0, 18, 320, 2, 32, 52, 92);
        m_pVideoSystem->DrawRect(0, 220, 320, 20, 3, 4, 12);
        for (int i = -1; i < 8; ++i)
            m_pVideoSystem->DrawRect((i * 48 + scroll) % 336 - 16, 168, 32, 52, 18, 24, 44);
        m_pVideoSystem->DrawRect(0, 0, 320, 12, 0, 0, 0);
        m_pVideoSystem->DrawRect(0, 228, 320, 12, 0, 0, 0);
    } else {
        m_pVideoSystem->DrawRect(0, 0, 320, 18, 5, 8, 18);
        m_pVideoSystem->DrawRect(0, 18, 320, 2, 28, 44, 78);
        m_pVideoSystem->DrawRect(0, 198, 320, 42, 7, 9, 18);
        m_pVideoSystem->DrawRect(0, 198, 320, 2, 40, 54, 72);
        for (int i = -1; i < 9; ++i)
            m_pVideoSystem->DrawRect((i * 42 - scroll) % 360 - 20, 154, 22, 44, 14, 20, 34);
    }

    if (m_adventurePhase == ADVENTURE_SCENE) {
        int focusX = 156 + (pulse - 1) * 2;
        m_pVideoSystem->DrawRect(focusX, 72, 8, 76, 54, 64, 96);
        m_pVideoSystem->DrawRect(focusX - 8, 82, 24, 8, 54, 64, 96);
        m_pVideoSystem->DrawRect(focusX - 4, 148, 16, 14, 42, 50, 78);
        m_pVideoSystem->DrawRect(0, 0, 320, 2, 110, 32, 44);
        m_pVideoSystem->DrawRect(0, 238, 320, 2, 110, 32, 44);
        if (!m_victoryScene) {
            int opponentX = 224 - ((m_phaseTime < 60) ? m_phaseTime : 60);
            m_pVideoSystem->DrawRect(opponentX, 92, 18, 54, 120, 28, 42);
            m_pVideoSystem->DrawRect(opponentX - 3, 82, 24, 10, 170, 42, 56);
            m_pVideoSystem->DrawRect(opponentX - 8, 146, 34, 4, 200, 58, 65);
        }
    }
}

void CEngine::DrawCombatEffects() {
    if (m_adventurePhase != ADVENTURE_FIGHT) return;

    if (m_impactTime > 0) {
        int radius = 8 + m_impactTime * 2;
        int centerX = 150 + ((m_phaseTime % 3) - 1) * 3;
        m_pVideoSystem->DrawRect(centerX - radius, 112 - radius, radius * 2, 3, 255, 220, 120);
        m_pVideoSystem->DrawRect(centerX - 2, 112 - radius, 4, radius * 2, 255, 220, 120);
        m_pVideoSystem->DrawRect(centerX - radius / 2, 112 + radius / 2, radius, 2, 255, 90, 50);
    }

    if (m_specialTime > 0) {
        m_pVideoSystem->DrawRect(22, 88, 276, 2, 250, 72, 48);
        m_pVideoSystem->DrawRect(22, 150, 276, 2, 250, 72, 48);
        m_pVideoSystem->DrawText(105, 92, "SPECIAL!");
    }
}

void CEngine::DrawAdventureHud() {
    const char* phase = "WALK RIGHT";
    int displayStage = (m_stage < 13) ? m_stage + 1 : 13;
    if (m_adventurePhase == ADVENTURE_SCENE) phase = "CUTSCENE";
    if (m_adventurePhase == ADVENTURE_FIGHT) phase = "FIGHT";
    if (m_adventurePhase == ADVENTURE_ESCAPE) phase = "ESCAPE!";
    if (m_adventurePhase == ADVENTURE_COMPLETE) phase = "ADVENTURE COMPLETE";

    m_pVideoSystem->DrawRect(3, 3, 314, 30, 8, 12, 25);
    m_pVideoSystem->DrawRect(3, 31, 314, 1, 38, 62, 104);
    m_pVideoSystem->DrawRect(6, 7, 3, 20, 80, 190, 255);
    m_pVideoSystem->DrawText(12, 8, "ADVENTURE  AREA %02d/13  TEAM %d/5  HP %d%%", displayStage,
                             m_teamSize, m_teamHealthPercent);
    m_pVideoSystem->DrawText(12, 20, ">> %s", phase);
    if (m_adventurePhase == ADVENTURE_SCENE) {
        if (m_victoryScene)
            m_pVideoSystem->DrawText(35, 105, "VICTORY! SPECIAL FINISH - RECRUITING...");
        else if (m_stage == 2)
            m_pVideoSystem->DrawText(35, 105, "A MINI BOSS CRUSHES A STRANGER!");
        else if (m_stage == 11)
            m_pVideoSystem->DrawText(35, 105, "THE AREA WILL BE DESTROYED - RUN!");
        else if (m_stage == 12)
            m_pVideoSystem->DrawText(35, 105, "THE BOSS ARRIVES... CAMERA LOW, THEN WIDE");
        else
            m_pVideoSystem->DrawText(45, 105, "A NEW OPPONENT STEPS INTO THE STREET");
        if (!m_victoryScene) {
            const char* rival = "RIVAL";
            if (m_sceneVariant == 1) rival = "SHADOW NINJA";
            if (m_sceneVariant == 2) rival = "IRON BRAWLER";
            if (m_sceneVariant == 3) rival = "WILD CARD";
            if (m_sceneVariant == 4) rival = "FALLEN HERO";
            if (m_sceneVariant == 5) rival = "STREET HUNTER";
            m_pVideoSystem->DrawText(112, 122, "VS  %s", rival);
        }
    }
    if (m_adventurePhase == ADVENTURE_FIGHT) {
        m_pVideoSystem->DrawRect(4, 35, 150, 13, 8, 12, 25);
        m_pVideoSystem->DrawRect(7, 38, 144 * m_opponentHealth / m_opponentMaxHealth, 7, 210, 54, 66);
        m_pVideoSystem->DrawText(8, 50, "OPPONENT %d/%d", m_opponentHealth, m_opponentMaxHealth);
        if (m_stage == 2) m_pVideoSystem->DrawText(45, 50, "DEFEAT THIS MINI BOSS FOR REWARDS");
        if (m_qteActive) m_pVideoSystem->DrawText(55, 70, "QTE! PRESS A/D OR AN ATTACK NOW!");
    }
    if (m_adventurePhase == ADVENTURE_ESCAPE)
        m_pVideoSystem->DrawText(45, 50, "RUN RIGHT! %d SEC", (10800 - m_phaseTime) / 60);
}
