
[![CI](https://github.com/humbertodias/openmugen/actions/workflows/ci.yml/badge.svg)](https://github.com/humbertodias/openmugen/actions/workflows/ci.yml)
[![CD](https://github.com/humbertodias/openmugen/actions/workflows/cd.yml/badge.svg)](https://github.com/humbertodias/openmugen/actions/workflows/cd.yml)
![GitHub all downloads](https://img.shields.io/github/downloads/humbertodias/openmugen/total)


# 🥋 OpenMugen

**OpenMugen** is a free and open-source 2D fighting game engine inspired by the classic **M.U.G.E.N**, built with **SDL2**.  
Its main goal is to offer a **simple**, **extensible**, and **cross-platform** foundation for creating customizable fighting games.


[**Play**](https://humbertodias.github.io/openmugen/) directly in your browser 


## 🎮 Controls

| Player | Movement     | Punch       | Kick       |
|--------|--------------|-------------|------------|
| **P1** | `A` / `D`    | `Y`, `U`    | `H`, `J`   |
| **P2** | `←` / `→`    | -    | -   |

> ⚠️ **Note:** Player 2 controls are **under development**.

---

## Adventure mode

The default game flow is now a 13-area side-scrolling adventure rather than a
single arcade match. Hold `D` or the right arrow to walk to the next encounter.
Story scenes pause the route before each fight; use `Y`, `U`, `H`, or `J` to
attack, and answer the on-screen QTE with a movement or attack input. Victorious
opponents can join the team until all five slots are filled.

Area 3 is a powered-up mini-boss encounter. Area 12 is the three-minute escape
sequence, followed by the final boss and the adventure ending. The HUD shows
the current area, team size, team health reward, opponent health, and the
current scene objective. Cinematic encounters use versus-style reveal cards,
variant opponent identities, animated entrances, impact flashes, special-move
frames, and victory/recruitment transitions.

## 🖥️ Desktop Version

### Requirements (Ubuntu/Debian)

```bash
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-gfx-dev
```

### Build Instructions

```bash
cmake -Bbuild -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

Then run:

```bash
./build/OpenMugen
```

On Windows, install CMake and an SDL2 development environment, then run
`build-Windows.bat` from the repository directory. An optional configuration
can be supplied, for example `build-Windows.bat Debug`. The executable and
runtime data are placed under `build`.
<img alt="OpenMugen Gameplay" src="https://github.com/user-attachments/assets/e7a9a194-2dde-4d4f-b41a-24b9154fe2fb" />


## 🚧 Roadmap

- [ ] Fully implement player movement - https://github.com/humbertodias/openmugen/issues/2
- [ ] Enhanced collision and hitbox system - https://github.com/humbertodias/openmugen/issues/3
- [ ] Add sound and music engine - https://github.com/humbertodias/openmugen/issues/4
- [ ] Custom animations and character scripting  
- [ ] Basic AI opponent


## 🤝 Contributing

Pull requests are welcome!  
If you find a bug or have a feature suggestion, feel free to [open an issue](https://github.com/humbertodias/openmugen/issues).


## 📄 License

OpenMugen is licensed under the [MIT License](LICENSE).
