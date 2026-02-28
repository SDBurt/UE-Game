# Combat Design Document — Crucible

## Overview

Crucible's combat system blends real-time action combat (Bayonetta/DMC fluidity) with a party command system (Arknights: Endfield). The player directly controls one character while AI party members fight alongside, responding to tactical commands.

---

## Element Reaction Matrix

Four primary elements interact in pairs:

| Trigger \ Aura | Heat | Electric | Cryo | Nature |
|-----------------|------|----------|------|--------|
| **Heat** | — | — | Solidification | Combustion |
| **Electric** | — | — | Electrification | — |
| **Cryo** | Solidification | Electrification | — | — |
| **Nature** | Combustion | Corrosion | — | — |
| **Physical** | — | — | Shatter* | — |

*Shatter only triggers on Frozen (Cryo-aura'd) targets.

### Reaction Definitions

**Combustion** (Heat + Nature)
- AoE fire burst centered on target
- Deals % of triggering hit as bonus damage to all enemies in radius
- Clears both element auras

**Electrification** (Electric + Cryo)
- Chain lightning bounces to 3 nearby enemies
- Each bounce deals diminishing damage
- Applies brief Shocked status to all hit

**Corrosion** (Nature + Electric)
- DoT applied for 8 seconds
- Reduces target's defense by 20% for duration
- Stacks up to 3 times

**Solidification** (Cryo + Heat)
- Creates a crystalline formation at target location
- Crystal can be destroyed for AoE shatter damage
- Crystal blocks movement for both enemies and players

**Shatter** (Physical hit on Frozen target)
- Massive bonus burst damage (2x physical hit)
- Guaranteed stagger contribution
- Removes Frozen status

---

## Stagger System

### Attributes
- `StaggerDamage` — current accumulated stagger on the target
- `MaxStaggerDamage` — threshold for stagger break

### Flow
1. Every attack adds to target's `StaggerDamage`
2. Different attacks contribute different stagger values
3. When `StaggerDamage >= MaxStaggerDamage`:
   - Apply `Status.Staggered` gameplay tag
   - Fire `Event.ChainOpportunity` gameplay event
   - Target plays stagger animation (looping, vulnerable)
   - Damage taken increased by 50% during stagger
4. Stagger window lasts 3-5 seconds (tunable)
5. After window: `StaggerDamage` resets to 0, target recovers

### Stagger Decay
- If not hit for 2 seconds, `StaggerDamage` decays at 10/sec
- Keeps pressure on the player to maintain combos

---

## Skill Point Economy

### Resource: SP (Skill Points)
- **Pool:** Shared across the entire party
- **Max:** 100 SP (tunable via `MaxSkillPoints` attribute)

### Generation
| Action | SP Generated |
|--------|-------------|
| Basic attack hit | +5 SP |
| Perfect dodge | +15 SP |
| Elemental reaction | +10 SP |
| Chain attack | +0 (costs SP) |

### Costs
| Ability Type | SP Cost |
|-------------|---------|
| Skill 1-4 | 15-30 SP (varies per character) |
| Chain Attack | 40 SP |
| Ultimate | 60 SP |

### Design Intent
No passive SP regen forces aggressive play. Players must attack to earn resources for skills, creating a risk/reward loop: engage to build SP, spend SP for powerful abilities.

---

## Chain Attack System

### Trigger Conditions
A chain attack opportunity opens when ANY condition is met:
1. **Stagger break:** Target's stagger bar fills
2. **Elemental reaction:** Any reaction triggers on a target
3. **Manual trigger:** Player presses chain trigger (E key) during opportunity window

### Sequence
1. **Activation:** Time slows, camera pulls back to cinematic angle
2. **Party order:** Each party member performs their chain skill in sequence
3. **Combined element:** Final hit applies the combined element of all participants
4. **Resolution:** Time resumes, SP is consumed from shared pool

### Chain Skills
Each character has one chain skill (separate from their 4 regular skills):
- Triggered only during chain sequences
- Has unique animation and properties
- Element matches the character's innate element

---

## Party Skill Activation

### Command Flow
```
Player presses party member's skill button
  → System checks: SP sufficient? Cooldown ready? Member alive?
  → If valid:
      → AI party member interrupts current action
      → Executes commanded skill (plays montage, applies GE)
      → SP deducted from shared pool
      → Cooldown begins
  → If invalid:
      → UI feedback (insufficient SP flash, cooldown indicator)
```

### AI Behavior (When Not Commanded)
- Party members auto-attack nearby enemies
- They avoid enemy attacks (basic dodge AI)
- They do NOT use skills automatically (SP is precious)
- They maintain formation relative to player character

---

## Damage Pipeline (Technical)

```
1. Ability activates (CrucibleGameplayAbility)
2. Play attack montage
3. On anim notify: sphere trace for targets
4. For each hit target:
   a. Create GE spec from DamageEffectClass
   b. GE uses CrucibleDamageExecution
   c. Execution calculates final damage:
      - Base damage (from ability data or SetByCaller)
      - Element multipliers (future)
      - Stagger bonus (if target staggered)
      - Defense reduction (if corroded)
   d. Writes to IncomingDamage meta attribute
5. PostGameplayEffectExecute on target's AttributeSet:
   a. Subtract IncomingDamage from Health
   b. Add IncomingDamage to StaggerDamage
   c. Reset IncomingDamage to 0
   d. Check death (Health <= 0 → State.Dead)
   e. Check stagger (StaggerDamage >= MaxStaggerDamage → Status.Staggered)
```
