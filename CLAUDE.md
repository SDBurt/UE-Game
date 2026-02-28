# CLAUDE.md — Crucible

## Project Identity

- **Name:** Crucible
- **Engine:** Unreal Engine 5.7
- **Platform targets:** Desktop (PC) primary, Mobile (iOS/Android) secondary
- **Development Environment:** Visual studio with Git Bash Terminal, Unreal Engine
- **Role:**: Assist with development and provide step-by-step guides for changes in Unreal Engine around blueprints and the Unreal Editor for Game Development

## Combat Model

**Primary reference: Arknights: Endfield** — party command system:
- Control one character directly, AI party members fight alongside
- Issue tactical commands to party members (position, skill use, target focus)
- Chain attacks trigger when conditions are met across the party
- NOT Genshin-style character switching — party members are always on-field, main character switches models (not locations)

**Inspiration:** Bayonetta / DMC, Clair Obscur: Expedition 33, Monster Hunter Wilds

### Core Mechanics
- Real-time melee/ranged combo chains with cancel windows
- Perfect dodge: i-frame with slow-mo, counter opportunity, resource gain
- Block: damage mitigation with stamina cost; perfect block (i-frame parry) on precise timing triggers a counter
- Sprint: faster movement with stamina cost
- Elemental reactions: Heat, Electric, Cryo, Nature — combining for amplified effects
- Stagger system: accumulate stagger damage to break enemy poise
- Skill energy: recovered by linked skill/combo use or specific character abilities
- Chain attacks: party-wide coordinated attacks triggered by stagger/element conditions
- Ultimates: high-cost abilities charged through combat performance
- Combo inputs: combination skill keys (e.g. Y+B on controller, two keys/mouse buttons on M&KB) produce different skills
- Attack stances/modes: weapon-dependent stance switching that changes the moveset

### Weapons
- **Weapon types**: Each type has its own moveset, stances, combo trees, and cancel windows
- **Sharpness**: Weapons dull with use (Monster Hunter style) — affects damage and bounce chance. Sharpening restores edge during combat downtime
- **Ammo & projectiles**: Ranged weapons use ammo types with different properties (piercing, explosive, elemental, status)

### Defense
- **Armor types**: Light/medium/heavy affecting dodge speed, block stability, and elemental resistance
- **Blocking**: Stamina-based; some enemy attacks are blockable, some are not (indicated visually)
- **Parry**: Perfect block timing creates i-frame parry window → triggers counter attack
- **Dodge vs block**: Player chooses defensive style based on weapon/armor/enemy attack type

### Buffs, Debuffs & Status Effects
- Buffs: attack up, defense up, elemental coating, sharpness bonus, stamina regen, etc.
- Buff sources: consumables, skills, weapons, armor, gear set bonuses (e.g. 2-piece/4-piece)
- Debuffs: poison, burn, freeze, shock, bleed, defense down, blunt weapon penalty
- Status buildup: repeated application accumulates until threshold triggers the full effect

### Enemy Behavior
- **Knockdown & tiring**: Enemies can be knocked down (stagger break) or tire out from exertion, creating attack windows
- **Escape & recovery**: Enemies flee to recover health/stamina/energy when weakened — players must pursue or cut off
- **Enemy combos & skills**: Enemies have combo chains and special attacks; each is tagged as dodgeable, blockable, parriable, or must-avoid
- **Enrage**: Enemies become more aggressive and dangerous at health thresholds

### Items & Crafting
- **Materials**: Gathered from the world and carved/dropped from enemies (Monster Hunter style)
- **Consumables**: Healing potions, stamina drinks, elemental coatings, buffs, antidotes, traps
- **Crafting**: Combine materials using recipes to create consumables, weapon upgrades, armor pieces
- **Recipes**: Discovered through progression, NPC vendors, or experimentation

## GAS Architecture

- **ASC on Character** (Pattern A — migrate to PlayerState later if needed)
- **Attributes**: Health, MaxHealth, SkillPoints, MaxSkillPoints, StaggerDamage, MaxStaggerDamage, IncomingDamage (meta)
- **Native GameplayTags**: Registered via `CrucibleAssetManager::StartInitialLoading()`
- **Damage pipeline**: Ability → Montage → Sphere Trace → Apply GE (with DamageExecution) → IncomingDamage meta attr → PostGameplayEffectExecute subtracts Health, adds Stagger
- **Ability activation**: Input → `TryActivateAbilitiesByTag()`
- **ATTRIBUTE_ACCESSORS macro**: Defined in `CrucibleAttributeSet.h` — engine does NOT ship it

## C++ and UE5 Standards

### Rules
- UE5 naming: PascalCase, `A`/`U`/`F`/`E`/`I` prefixes, `bBooleans`
- UE5 types only: `FString`, `TArray`, `TMap`, `TSet` — never `std::` containers or smart pointers
- `UPROPERTY()` on all UObject pointers, `NewObject`/`CreateDefaultSubobject` — never raw `new`/`delete`
- Composition over deep inheritance, interfaces for cross-cutting behavior
- Forward declarations in headers, one class per file (filename = class name without prefix)
- Avoid `Tick()` — use timers, delegates, events
- Never hardcode asset paths — use soft references
- Always consider mobile performance

### C++ vs Blueprint
- **C++ owns:** Game logic, ability behavior, attribute math, system architecture, input handling
- **Blueprint owns:** Asset references (montages, GEs, meshes), GameplayTag assignment on abilities, tuning values, visual feedback, UI
- **Rule of thumb:** Follow best practice. Don't re-invent or over complicate things.

## UE 5.7 Gotchas

- Native GameplayTags NOT available during CDO construction — set ability tags in Blueprint editor, not C++
- `AbilityTags` deprecated in UE 5.7 — use `GetAssetTags()` / editor Asset Tags
- `AssetManagerClassName` config goes in `[/Script/Engine.Engine]` section, NOT `[/Script/Engine.AssetManagerSettings]`
- BP GameMode must override DefaultPawnClass/PlayerControllerClass to point to BP subclasses
- Live Coding / Hot Reload: safe for function body changes only — restart editor for constructors, headers, statics, UPROPERTY changes
- Cross-module symbol access requires `CRUCIBLE_API` on classes/structs
- `MakeOutgoingSpec()` takes `TSubclassOf<UGameplayEffect>` — for programmatic GEs, construct `FGameplayEffectSpec` directly

## How to Help Me Best

- Step-by-step instructions with architectural context first
- Include all `#include` directives and proper UE5 macros
- Flag mobile performance issues proactively
- For multiple approaches, explain tradeoffs briefly and recommend one
- Push back on bad architecture — explain why
- Simplest approach for vertical-slice scale — don't over-engineer
- Don't force everything into C++ — use the editor for what it's good at
