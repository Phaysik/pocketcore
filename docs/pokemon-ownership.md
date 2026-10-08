# Pokemon Data Ownership Contract

This document defines the single owning structure for every persistent, derived, and battle-only Pokemon property in the engine. Its purpose is to guarantee that each value has exactly one source of truth so future changes cannot introduce a second one.

## Definitions

- **Authoritative source** — The one structure that owns a value. Every reader must resolve the value from here. Writes happen only through this owner.
- **Intentional derived/cache state** — A value that is deliberately recomputed from, or snapshotted from, an authoritative source for performance or decoupling. It is never written independently of its source and may be regenerated at any time.
- **Non-owning reference** — A pointer or view that observes a value owned elsewhere. It carries no ownership and must not outlive its owner.

The engine separates three kinds of data:

1. **Definition data** — the immutable templates registered once per species, move, nature, ability, item, learnset, type, effect, status, terrain, weather, or multiplier. Owned by the `*Meta` structures.
2. **Persistent instance data** — a single Pokemon's identity and configuration, materialized from definition data when the Pokemon is built. Owned by `Pokemon`.
3. **Battle-only state** — transient state that exists solely for the duration of a battle. Owned by `BattleSlot` and `BattleState`.

## Ownership contract per structure

### `PokemonMeta` (`include/Pokemon/pokemonMeta.h`)

Authoritative source for the **species definition**. It owns `mName` (`std::string`), `mAbilityPool`, `mBaseStats`, `mTypeIDs`, `mLevel`, `mPokemonID`, and `mLearnsetID`. Nothing in `PokemonMeta` is derived.

### `LearnsetMeta` (`include/Learnset/learnsetMeta.h`)

Authoritative source for the **set of moves a species can learn**. It owns `mName`, `mEntries` (the `MoveLearnEntry` list), and `mLearnsetID`. Nothing is derived.

### `MoveMeta` (`include/Move/moveMeta.h`)

Authoritative source for the **move definition**: `mHitCountPolicy`, `mName`, `mTriggers`, `mMoveID`, `mTypeID`, `mPower`, `mTargetID`, `mRangeID`, `mAccuracy`, `mPriority`, `mPPMaxAmount`, `mPPDefaultAmount`, and `mSpecial`. The default and maximum PP values here are the source from which a `Pokemon` instance's per-slot PP is materialized. Nothing is derived.

### `NatureMeta` (`include/Nature/natureMeta.h`)

Authoritative source for the **nature definition**: `mStatMultipliers`, `mName`, `mTriggers`, `mTargetID`, and `mNatureID`. `mStatMultipliers` is the source from which each `Pokemon`'s applied nature multipliers are materialized. Nothing is derived.

### `AbilityMeta` (`include/Ability/abilityMeta.h`)

Authoritative source for the **ability definition**: `mName`, `mTriggers`, `mAbilityID`, and `mTargetID`. Nothing is derived.

### `ItemMeta` (`include/Item/itemMeta.h`)

Authoritative source for the **item definition**: `mName`, `mTriggers`, `mItemID`, `mTargetID`, and `mIsConsumable`. Nothing is derived.

### `TypeMeta` (`include/Types/typeMeta.h`)

Authoritative source for the **type definition**: `mOffensiveMatchups` (this type's effectiveness against every registered type, indexed by internal type index), `mName`, and `mTypeID`. Nothing is derived.

### `EffectMeta` (`include/Effect/effectMeta.h`)

Authoritative source for the **effect definition**: `mName`, `mApply` (the effect function), `mEffectID`, `mMayChangeWeather`, `mMayChangeTerrain`, and `mMayChangeStatus`. Nothing is derived.

### `StatusMeta` (`include/Status/statusMeta.h`)

Authoritative source for the **status definition**: `mName`, `mStatusInteractions`, `mStatusID`, and `mStatusClassification`. `mStatusClassification` is the single source deciding whether a status is owned by `Pokemon` (non-volatile) or by `BattleSlot` (volatile); `BattleSlot::addVolatileStatus` reads it to enforce that split. Nothing is derived.

### `TerrainMeta` (`include/Terrain/terrainMeta.h`)

Authoritative source for the **terrain definition**: `mName`, `mTerrainInteractions`, and `mTerrainID`. Nothing is derived.

### `WeatherMeta` (`include/Weather/weatherMeta.h`)

Authoritative source for the **weather definition**: `mName`, `mWeatherInteractions`, and `mWeatherID`. Nothing is derived.

### `MultiplierMeta` (`include/Multiplier/multiplierMeta.h`)

Authoritative source for the **multiplier definition**: `mName`, `mMultiplierID`, and `mApplicationPolicy`. Nothing is derived.

### `Pokemon` (`include/Pokemon/pokemon.h`)

Authoritative source for a **single Pokemon instance**. It splits into three groups:

- **Persistent instance state (authoritative):** `mPokemonID`, `mBaseStats`, `mPokemonIVs`, `mPokemonEVs`, `mNatureMultipliers`, `mMoveIDs`, `mMaxPP`, `mTypeIDs`, `mAbilityIDs`, `mItemIDs`, `mNatureIDs`, and `mLevel`. These are materialized (snapshotted) from definition data at construction and are owned independently thereafter — a `Pokemon` holds no live link back to its `PokemonMeta`.
- **Battle-live authoritative state:** `mHealth` (current HP), `mCurrentPP` (remaining PP per slot), and `mNonVolatileStatusIDs` (burn, sleep, and other non-volatile conditions). These persist on the Pokemon across switches and outside battle.
- **Derived/cache state:** `mCalculatedStats` (recomputed from base stats, IVs, EVs, level, and nature multipliers by `recomputeStats`) and `mLevelDamageFactor` (recomputed from level by `setLevel`). These are never written directly by callers; they are regenerated whenever a contributing input changes.
- **Non-owning reference:** `mName` is a `std::string_view` into a caller-owned backing string (typically `PokemonMeta::mName`). Its backing storage must outlive the `Pokemon`.

#### Stat recalculation contract

> Any mutation that changes the inputs to the stat calculation must leave calculated stats immediately consistent with the new inputs.

`Pokemon::recomputeStats()` is the single authoritative calculation function. Public stat-input setters invoke it before returning; callers do not need a separate refresh. Private input-update helpers may defer recalculation while constructors initialize inputs, but construction recomputes before exposing the instance.

| Mutation API                                                                   | Recalculation behavior                                                                                                                                                                  |
| ------------------------------------------------------------------------------ | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `setPokemonIV()`                                                               | Recomputes after validation, whether the value is accepted or rejected.                                                                                                                 |
| `setPokemonIVsArray()`                                                         | Recomputes after validation, whether the array is accepted or rejected.                                                                                                                 |
| `setPokemonEV()`                                                               | Recomputes after validation, whether the value is accepted or rejected.                                                                                                                 |
| `setPokemonEVsArray()`                                                         | Recomputes after validation, whether the array is accepted or rejected.                                                                                                                 |
| `setLevel()`                                                                   | Updates the level damage factor and recomputes all calculated stats.                                                                                                                    |
| `setAttack()`, `setDefense()`, `setSpAttack()`, `setSpDefense()`, `setSpeed()` | Replaces the corresponding base stat and recomputes all calculated stats.                                                                                                               |
| `setPokemonID()`                                                               | Replaces species identity and the supplied base-stat record, then recomputes.                                                                                                           |
| `setNatureID()`                                                                | Resolves and stores the nature's multipliers, then recomputes. An unregistered ID leaves state unchanged and does not recompute.                                                        |
| `setNatureIDsArray()`                                                          | Resolves multipliers and recomputes. If any ID is unregistered, existing identifiers and multiplier rows remain unchanged; stats are recomputed from those retained inputs.             |
| `setMaximumHealth()`                                                           | Replaces base HP and recomputes all calculated stats, preserving missing health through the shared HP adjustment rules.                                                                 |

Invalid IV/EV values and arrays leave the corresponding stored inputs unchanged. Calculated stats still reflect those retained inputs on return.

`setMaximumHealth()` accepts **base HP**, not the desired calculated maximum: `getMaximumHealth()` returns the value derived from base HP, IVs, EVs, level, and nature multipliers. `setHealth()` clamps its argument to the existing calculated maximum without recalculating stats.

#### Current HP during recalculation

`recomputeStats()` also adjusts current HP when the calculated maximum changes. This policy applies to every stat-input setter that invokes it, including IV, EV, level, base-stat, species, and nature mutations; it is not specific to `setMaximumHealth()`.

- **Fainted Pokemon:** Current HP remains zero, even if maximum HP increases. Stat changes do not revive a Pokemon.
- **Maximum HP increases:** A living Pokemon gains the same amount of current HP, capped at the new maximum. The amount of missing HP is preserved.
- **Maximum HP decreases:** A living Pokemon loses the same amount of current HP, with a floor of 1. Missing HP is preserved unless that floor is reached; a stat change does not faint a living Pokemon.
- **Maximum HP is unchanged:** Current HP is unchanged.

For example, decreasing maximum HP from 160 to 80 while current HP is 150 leaves 70 current HP, preserving the 10 missing HP. Increasing maximum HP from 40 to 41 while current HP is 25 leaves 26 current HP. A decrease of 3 maximum HP while current HP is 2 leaves 1 current HP.

This replaces the earlier clamp-only HP policy: shrinking maximum HP does not simply set current HP to the new maximum, and increasing maximum HP can increase current HP. Calculated-stat consistency remains immediate in either case.

### `BattleSlot` (`include/Battle/battleState.h`)

Authoritative source for the **transient state of one active battle position**. It owns `mDamageFormulaModifiers`, `mVolatileStatuses`, `mStatStages`, `mChoiceLockedMove`, `mPosition`, `mSleepCounter`, `mToxicCounter`, `mProtectionCounter`, `mIsProtected`, `mIsFlinched`, `mIsGrounded`, and `mFaintProcessed`. Volatile statuses live here and only here; `BattleSlot::addVolatileStatus` rejects any status the registry classifies as non-volatile, keeping the volatile/non-volatile boundary with `Pokemon` clean.

- **Non-owning reference:** `mPokemon` points to the party member currently occupying this position, or is `nullptr` when the position is empty. The slot does not own the Pokemon.

### `BattleState` (`include/Battle/battleState.h`)

Authoritative source for the **whole-field battle state**. It owns the `BattleSlot` values in `mSideA` and `mSideB`, plus `mWeatherIDs`, `mTerrainIDs`, `mRuleset`, `mSpikesPartyA`, `mSpikesPartyB`, `mToxicSpikesPartyA`, `mToxicSpikesPartyB`, `mStealthRockPartyA`, `mStealthRockPartyB`, and `mBattleStarted`.

- **Non-owning references:** `mPartyA` and `mPartyB` are vectors of `Pokemon *` into a roster owned outside `BattleState`. `BattleState` never owns or destroys the Pokemon objects it references.

## Duplicated-value audit

Every value that appears in more than one structure is classified below.

| Value                        | Owner (authoritative)                         | Also appears in                                          | Classification of the copy                                                                   |
| ---------------------------- | --------------------------------------------- | -------------------------------------------------------- | -------------------------------------------------------------------------------------------- |
| Display name                 | `PokemonMeta::mName` (owning `std::string`)   | `Pokemon::mName` (`std::string_view`)                    | Non-owning reference into the meta-owned string; not a copy.                                 |
| Species base stats           | `PokemonMeta::mBaseStats`                     | `Pokemon::mBaseStats`                                    | Intentional per-instance snapshot taken at construction.                                     |
| Species type IDs             | `PokemonMeta::mTypeIDs`                       | `Pokemon::mTypeIDs`                                      | Intentional per-instance snapshot taken at construction.                                     |
| Species level                | `PokemonMeta::mLevel`                         | `Pokemon::mLevel`                                        | Intentional per-instance snapshot; instance level is authoritative for stat calculation.     |
| Species identifier           | `PokemonMeta::mPokemonID`                     | `Pokemon::mPokemonID`                                    | Intentional per-instance snapshot of the stable ID.                                          |
| Nature stat multipliers      | `NatureMeta::mStatMultipliers`                | `Pokemon::mNatureMultipliers`                            | Intentional per-instance snapshot applied to calculated stats.                               |
| Move PP defaults             | `MoveMeta::mPPDefaultAmount` / `mPPMaxAmount` | `Pokemon::mMaxPP` / `mCurrentPP`                         | `mMaxPP` is a materialized snapshot; `mCurrentPP` is authoritative battle-live remaining PP. |
| Ability set                  | `PokemonMeta::mAbilityPool` (legal pool)      | `Pokemon::mAbilityIDs` (chosen)                          | Distinct semantics, not a duplicate: pool = legal options, instance = chosen abilities.      |
| Learnable moves              | `LearnsetMeta::mEntries` (legal moves)        | `Pokemon::mMoveIDs` (chosen)                             | Distinct semantics, not a duplicate: learnset = legal options, instance = equipped moves.    |
| Active Pokemon pointer       | roster owned outside `BattleState`            | `BattleState::mPartyA`/`mPartyB`, `BattleSlot::mPokemon` | Non-owning references; the slot pointer selects one party member into a position.            |

No duplicated value falls outside the two permitted classes. Every copy above is either a non-owning reference or an intentional snapshot/cache with a single authoritative source.

## Rules

1. Definition data is owned by the `*Meta` structures. A `Pokemon` instance materializes the values it needs at construction and owns those snapshots independently; readers of live instance state resolve it from `Pokemon`, not from a `*Meta`.
2. Derived values (`mCalculatedStats`, `mLevelDamageFactor`) are written only by their recompute functions. Callers must never set them directly; they change an input and let the value regenerate.
3. Non-volatile status is owned only by `Pokemon`; volatile status is owned only by `BattleSlot`. `BattleSlot::addVolatileStatus` enforces this split.
4. `Pokemon` objects are owned outside `BattleState`. `BattleState::mPartyA`, `BattleState::mPartyB`, and `BattleSlot::mPokemon` are non-owning references and must not outlive the roster.
5. A new duplicated value is only permitted if it is filed in the audit above as a non-owning reference or an intentional snapshot/cache with one named authoritative owner.

## Follow-ups

- **Instance/species snapshot drift (tracked, intentional):** `Pokemon` snapshots `mBaseStats`, `mTypeIDs`, and `mLevel` from `PokemonMeta` at construction and keeps no live link back to the meta. This decoupling is intentional (a `Pokemon` can outlive or diverge from its registered species), but it means editing a `PokemonMeta` after instances exist does not update those instances. If live species updates are ever required, add an explicit re-materialization step keyed on `Pokemon::mPokemonID` rather than a second owning field. No code change is required for this ticket.
