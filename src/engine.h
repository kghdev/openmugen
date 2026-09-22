#ifndef __ENGINE__H
#define __ENGINE__H
#include "player.h"

class CEngine {
    CPlayer          player1, player2, player3, player4;
    CMemManager*     m_pMemManager;
    CVideoSystem*    m_pVideoSystem;
    CGameTimer*      m_pTimer;
    CVirtualMachine* m_pVMp1;
    CVirtualMachine* m_pVMp2;
    enum AdventurePhase {
        ADVENTURE_WALK,
        ADVENTURE_SCENE,
        ADVENTURE_FIGHT,
        ADVENTURE_ESCAPE,
        ADVENTURE_COMPLETE
    };
    AdventurePhase m_adventurePhase;
    int            m_stage;
    int            m_phaseTime;
    int            m_worldX;
    int            m_opponentHealth;
    int            m_opponentMaxHealth;
    int            m_teamSize;
    int            m_teamHealthPercent;
    int            m_qteTime;
    int            m_impactTime;
    int            m_specialTime;
    int            m_sceneVariant;
    bool           m_qteActive;
    bool           m_qteResolved;
    bool           m_victoryScene;
    bool           m_recruited[5];

   public:
    CEngine();

    ~CEngine();

    void ResetEngine();

    void RunEngine();

    void InitEngine(CMemManager* m, CVideoSystem* v, CGameTimer* t);

   private:
    void RunAdventure();
    void DrawAdventureScene();
    void DrawAdventureHud();
    void DrawCombatEffects();
    void BeginStage();
    void BeginFight();
    void ResolveQte(bool success);
    bool IsAdventureInputPressed() const;
    bool IsAttackPressed() const;
};

#endif
