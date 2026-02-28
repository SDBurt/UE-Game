# Crucible

A real-time action RPG built in Unreal Engine 5.7 (C++), inspired by Arknights: Endfield, Bayonetta, and Monster Hunter.

## What This Is

The C++ source and configuration for an action RPG combat system built on the Gameplay Ability System (GAS). This repo contains **source code only** — no content assets (meshes, textures, animations). You'll need UE 5.7 and the Third Person template content to build.

**Current state:** Early vertical slice — damage pipeline proven, basic attack and dodge abilities, automated test suite.

## Core Systems

- **GAS-based combat** — abilities, attributes, damage execution, stagger
- **Perfect dodge** — i-frame mechanic with invulnerability window
- **Elemental reactions** — Heat, Electric, Cryo, Nature with combo effects
- **Party command system** — direct control + AI party members with tactical commands
- **Stagger/break** — accumulate stagger damage to create attack windows
- **Automated tests** — 19 tests covering damage, i-frames, clamping, and tags

## Project Structure

```
Source/
├── Crucible/               # Main game module
│   ├── Character/          # Player character, target dummy
│   ├── Combat/             # GAS abilities, attributes, damage execution
│   ├── Core/               # Game mode, player controller, asset manager
│   ├── Data/               # Ability data definitions
│   └── Tags/               # Native gameplay tag registration
├── CrucibleTests/          # Automated test module
│   ├── Helpers/            # Test pawn and utilities
│   └── Tests/              # Damage, i-frame, clamping, tag tests
Config/                     # Engine and game configuration
Docs/                       # Design documents
```

## Prerequisites

- Unreal Engine 5.7
- Visual Studio 2022 (or compatible C++ compiler)
- Git LFS (for any future binary assets)

## Setup

1. Clone the repo
2. Right-click `Crucible.uproject` → Generate Visual Studio project files
3. Open in Visual Studio or double-click the `.uproject` to open in the UE editor
4. The project expects the **GameplayAbilities** plugin (enabled in `.uproject`)

## Plugins

The project uses the **Visual Studio Tools for Unreal Engine** plugin (not included). Install it from the [Epic Marketplace](https://www.unrealengine.com/marketplace/) if you want VS integration features — the project builds fine without it.

## License

[MIT](LICENSE)
