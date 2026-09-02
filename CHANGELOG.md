# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/2.0.0/), and this project adheres to _vX.Y.Z_ versioning where _X_ represents an _edition_, _Y_ represents an _update_, and _Z_ represents an _addendum_.

## [0.12.17] - 2026-09-02

## [0.12.16] - 2026-09-02

### Added

- Added the generic `Interaction<ID>` metadata record and `InteractionAction` policy with `Coexist`, `ReplaceCurrent`, `RemoveCurrent`, and `BlockIncoming` outcomes. Interaction declarations are evaluated from an incoming identifier against identifiers already active in the same domain.
- Added reusable constexpr interaction algorithms for matching declarations, detecting blockers, replacing or removing active identifiers, stably compacting fixed-capacity identifier ranges, and applying an incoming identifier through the complete policy. Empty and duplicate identifiers are ignored; blocking occurs before mutation; only the first replacement retains the incoming identifier; removals preserve the relative order of survivors; and insertion is skipped when no slot remains.
- Added `MAX_ACTIVE_WEATHERS_ON_FIELD` with a fixed capacity of five and added `WeatherMeta::mWeatherInteractions`, allowing registered weather metadata to use the same interaction policy as statuses.
- Added deterministic `Random::findSeed()` test coverage for requested integral and floating-point outcomes.

### Changed

- Replaced `BattleState::mWeatherID` with `mWeatherIDs`, a five-element active-weather array. Battle-state test fixtures, weather consumers, and weather-change detection now operate on the complete array; an effect flagged as weather-mutating dispatches `WeatherChanged` when any array element changes.
- Changed the rain, harsh-sunlight, and sandstorm setters from unconditional scalar assignment to interaction-aware insertion through the configured `WeatherRegistry`. With the built-in weather metadata in this revision declaring no interactions, distinct setters coexist in insertion order until the five-weather array is full, while duplicate applications leave the array unchanged.
- Changed `WeatherHandler` to detect rain and harsh sunlight by membership in the active-weather array. Air Lock and Cloud Nine still force a neutral weather multiplier; when rain and harsh sunlight coexist, rain retains precedence because it is evaluated first, and Hydro Steam retains its harsh-sunlight exception when rain is absent.
- Generalized status metadata from `StatusInteraction` and `StatusInteractionAction` to `Interaction<StatusID>` and `InteractionAction`. `Pokemon::addStatus()` now delegates to the shared policy while preserving the built-in rules: Freeze blocks incoming Paralysis, Burn, Sleep, Poison, and Toxic; incoming Freeze removes Burn, Sleep, and Paralysis; and Toxic replaces Poison.
- Simplified burn detection to search the Pokemon's active status array directly, preserving the existing physical-move, Guts, and Facade exclusions.

### Removed

- Removed the status-specific `statusHelpers.h` algorithms and the `StatusInteraction` and `StatusInteractionAction` types after their behavior moved to the generic interaction layer.
- Removed the empty tracked `pokemon.log` artifact.

## [0.12.15] - 2026-09-01

### Fixed

- Updated Pokemon health-boundary and stream-output tests to pass constructor statistics in the health-first order introduced in 0.12.14. The corrected fixtures again initialize the intended maximum health, Attack, Defense, Special Attack, Special Defense, and Speed values, preserving the tests' health-clamping and serialized-stat expectations.

### Removed

- Removed the unused `RegisterMoveTestData` aggregate from the shared move-registry test helper and dropped its now-unneeded `<vector>` include. Test move registration continues to accept `MoveMeta` directly, assign the registry's next stable `MoveID`, copy every metadata field, and advance the registered-count and next-ID state.

## [0.12.14] - 2026-09-01

### Added

- Added equality support for effect-test assertions: `DamageContext::operator==` compares recoil ratios with the existing absolute-relative floating-point comparison and compares all damage flags and values exactly; `EffectContext::operator==` compares the complete context, including active multipliers and their built-in position table.
- Added shared EffectHandler test fixtures for critical-hit, STAB, targets, terrain, Pokemon, and move-registry setup. The helpers construct valid battle slots, contexts, registries, and metadata without duplicating setup in individual scenarios.

### Changed

- Reordered both `Pokemon` constructors to accept base stats as health, Attack, Defense, Special Attack, Special Defense, and Speed. Construction continues to initialize current and maximum health from the health argument, retain fixed move, ability, item, and type arrays, and derive the level damage factor from the supplied level.
- Changed `DamageContext::mRecoilRatio` from `float` to `double`; recoil validation and damage calculation now use the double-precision value throughout while retaining non-finite, non-positive, zero-damage, and missing-user early exits.
- Moved `EffectContext::getActiveMultipliers()` into the header as a `constexpr` accessor, preserving its ordered, non-owning read-only span contract.
- Changed `BaseDamageHandler` to clear `mDamage` when a user or target is absent, move power is zero, or the computed attack statistic is non-finite or non-positive. Target defense is now clamped to at least `1.0` before division, allowing zero and negative defense modifiers to produce bounded damage instead of rejecting the calculation.
- Renamed the built-in terrain enumerator from `Grass` to `Grassy` and updated terrain registration, terrain-effect matching, and the example program to use the corrected identifier; the stable enum position and `Grassy Terrain` metadata remain unchanged.
- Tightened `TargetsHandler`'s registry precondition by directly reading the resolved move metadata after provider lookup; valid providers and registered move IDs retain the existing spread-target multiplier behavior.
- Rewrote the base-damage, critical-hit, handler-interface, STAB, targets, and terrain handler suites around the shared fixtures. The scenarios now cover physical and special calculations, critical-stage rules, missing or invalid battle state, single versus spread targets, grounded and flying targets, forced grounding, and each supported terrain effect.

## [0.12.13] - 2026-08-31

### Added

- Added `Random::setSeed()` for reseeding the shared `std::mt19937` and constrained `Random::findSeed()` to search seed values until a nullary callable returns a requested result.
- Added reusable test helpers for battle states, effect contexts, tagged-ID comparability, and Pokemon construction so unit suites can share valid domain fixtures.
- Added `testInclude` to test-build and editor include paths so the shared test helpers resolve in unit suites and language tooling.

### Changed

- Changed accuracy calculation to use one clamped accuracy-stage multiplier indexed by the user's accuracy stage minus the target's evasion stage; accuracy remains clamped to 0 through 100 before the inclusive 1 through 100 roll determines a miss.
- Default-initialized every `RegistryProvider` registry pointer to `nullptr`, allowing value initialization while retaining its non-owning dependency contract.
- Changed fixed-registry batch registration to preserve the forwarding-reference factory rather than copying it before repeated invocation.
- Simplified built-in item, move, multiplier, nature, Pokemon, terrain, type, and weather ID headers by importing the shared `ub` alias into their domain namespaces.
- Rewrote the accuracy-check scenarios around deterministic seeded rolls and stage-difference behavior, and enabled dark-mode HTML coverage reports.

### Removed

- Removed the separate evasion-stage multiplier table and its constants because evasion is now represented by the combined accuracy-stage difference.

## [0.12.12] - 2026-08-30

### Fixed

- Restored missing direct standard-library and registry-error includes, plus required namespace aliases, in registry-configuration, weather and burn handler, and configuration-test translation units after the shared `RegistryError` extraction.

## [0.12.11] - 2026-08-30

### Added

- Added inherited registration-count access to registry facades that previously did not expose `FixedMetadataRegistry::getAmountRegistered()` directly.
- Added domain terminology to the Codebook spelling allowlist and raised rumdl's MD013 line-length limit from 200 to 450 characters.

### Changed

- Renamed `Effect/effectSourceAndSuppresion.h` to `Effect/effectSourceAndSuppression.h` and updated all battle, helper, and trigger includes.
- Renamed `EffectTrigger::mSuppresionRuleCount` to `mSuppressionRuleCount` and updated ability, item, nature, and move trigger execution to use the corrected member.
- Documented nature and Pokemon configuration and registry contracts, including fixed-capacity ownership, stable-ID allocation, atomic batch rollback, case-sensitive non-owning names, and update/remove behavior.
- Removed redundant configuration-constants includes from the nature and Pokemon configuration implementations.

## [0.12.10] - 2026-08-28

### Changed

- Documented `BattleEngine` construction and registry lifetimes, battle start and turn-validation requirements, deterministic event ordering, effect and suppression dispatch, per-target and per-hit trigger behavior, damage clamping, faint processing, and cleanup semantics. This revision made no runtime behavior changes.

## [0.12.9] - 2026-08-28

### Added

- Added the out-of-line `EffectRegistryConfiguration` mutation implementation. The facade can add one effect or an atomic span of effects, rename an entry, replace metadata by name or `EffectID` while retaining the stored stable ID, and remove an entry by name or ID.
- Documented battle-action validation, built-in effect identities, suppression matching, concrete effect-handler preconditions and outcomes, and status interaction, replacement, removal, and compaction rules.

### Removed

- Removed the unused `protectEffects()` and `fieldEffectEffects()` declarations from move metadata.
- Removed unused built-in-effect includes and aliases from multiplier, status, terrain, and weather registries.

## [0.12.8] - 2026-08-27

### Changed

- Marked `EffectContext::getActiveMultipliers()` as `ATTR_PURE` and made the `AccuracyCheckHandler` API's access section explicit.
- Simplified domain configuration accessors to return their inherited registry through `getRegistry()` and clarified cache dimensions, configuration constants, type-registry behavior, and effect-processing contracts. No runtime behavior changed.

## [0.12.7] - 2026-08-26

### Changed

- Replaced trivial registration-count forwarding methods with direct `using Base::getAmountRegistered` declarations across registry and configuration facades.
- Documented battle-state data carriers, generic registry error handling, stable-ID semantics, move, multiplier, status, terrain, and weather configuration operations, effect-handler inputs and outcomes, and status metadata. These documentation and API-surface cleanups did not change runtime behavior.

## [0.12.6] - 2026-08-26

### Added

- Added the shared `EffectTrigger` record with fixed suppression-rule storage, an ordered effect-ID vector, battle event and role selectors, and a logical suppression-rule count.

### Changed

- Replaced `AbilityEffectTrigger`, `ItemEffectTrigger`, `MoveEffectTrigger`, and `NatureEffectTrigger` with `EffectTrigger` in metadata, configuration APIs, battle dispatch, and tests. All four effect sources now use one trigger representation without changing trigger order or matching behavior.

### Removed

- Removed the four duplicated domain-specific trigger structure definitions and their now-redundant suppression, effect-ID, constants, and typedef includes.

## [0.12.5] - 2026-08-26

### Added

- Added `TypeMeta`, which stores one type's offensive matchup row, non-owning name, and stable `TypeID` in a single metadata record.

### Changed

- Changed `TypeRegistry` from separate `TypeEntry` metadata and type-chart storage to a `FixedMetadataRegistry<TypeMeta, TypeID, ...>` specialization whose registered metadata owns each matchup row.
- Changed type configuration, registry lookup, weather and terrain handlers, type-effectiveness handling, Pokemon metadata consumers, and tests to read and mutate `TypeMeta` records and their embedded matchups.
- Consolidated built-in type names and matchup rows under the type domain and updated registry capacity and namespace references to match the new metadata model.

### Removed

- Removed `TypeEntry` and the registry's standalone type-chart array, eliminating the parallel indexing requirement between metadata and matchup storage.

## [0.12.4] - 2026-08-25

### Changed

- Changed built-in type registration to construct a `TypeEntry` and call `FixedMetadataRegistry::addBuiltin()` directly.

### Removed

- Removed `TypeRegistry`'s redundant indexed `getEntry()` forwarding method; inherited fixed-registry access remains available to the type-specific implementation.

## [0.12.3] - 2026-08-25

### Changed

- Documented ability and item metadata ownership, registry and configuration lookup/mutation contracts, generic fixed-registry configuration behavior, stable tagged-ID semantics, and `EffectContext` multiplier handling.
- Removed redundant configuration-constants includes from the ability and item configuration implementations. This revision made no runtime behavior changes.

## [0.12.2] - 2026-08-24

### Changed

- Changed `EffectContext`'s built-in multiplier-position table size to derive directly from the one-past-the-end `BuiltinMultiplierID::FinalMultiplier` sentinel instead of `Stab + 1`.
- Documented ability trigger storage, battle-state ownership and non-owning Pokemon lifetimes, effect-context dispatch and multiplier semantics, and the complete Pokemon constructor, accessor, mutation, status, and stream-output contracts.

## [0.12.1] - 2026-08-23

### Added

- Added `Registry/registryError.h` as the shared home for `RegistryError`, `UnspecifiedMatchup`, `RegistryErrorInfo`, and `NO_LOGGING_FAILURE`; registry configuration and tests now consume errors from the registry namespace rather than configuration constants.
- Added rumdl Markdown line-length configuration and a Codebook project dictionary for repository-specific terminology.

### Changed

- Moved `MAX_STAT_TYPES` into the nature domain and updated its consumers, reducing unrelated registry and nature dependencies on configuration constants.
- Changed selected non-owning `std::string_view` parameters in Pokemon, registry helpers, tests, and formatting utilities to pass by const reference, and removed obsolete configuration-constants includes exposed by the error extraction.
- Expanded Doxygen contracts for registry errors, type configuration and lookup, logging, concepts, attributes, timers, input, random generation, and related utility APIs.

## [0.12.0] - 2026-08-22 (Nature Registry Update)

### Added

- Added the out-of-line `NatureRegistryConfiguration` mutation implementation. The facade can add one nature or an atomic span of natures, rename an entry, replace metadata by name or `NatureID` while retaining the stored stable ID, and remove an entry by name or ID; the query and runtime-registry accessors remain inherited from the declaration introduced in 0.11.6.
- Added the complete `PokemonRegistryConfiguration` facade over a configuration-owned `PokemonRegistry`:
  - Query APIs expose metadata, name/ID lookup, registered spans, registration counts, containment checks, and const access to the runtime registry.
  - Mutation APIs add one Pokemon or an atomic span, rename an entry, replace metadata by name or `PokemonID` while retaining the stored stable ID, and remove an entry by name or ID.
  - Duplicate names report `DuplicatePokemon`, missing names or IDs report `PokemonNotFound`, and additions enforce the `MAX_POKEMON == 10'000` fixed capacity. Names remain case-sensitive, non-owning `std::string_view` values whose backing storage must outlive their registrations.
  - Successful custom additions consume monotonically increasing IDs. Removal does not reuse IDs, and a failed batch erases entries appended by that batch and restores the prior registration count and next-ID snapshot.

### Changed

- Added a non-owning `const PokemonRegistry *pokemonRegistry` member to `RegistryProvider`. The example now constructs `PokemonRegistryConfiguration` and supplies its runtime registry through the provider; effect-handler test aggregates add a `nullptr` for the new field.
- Changed the `PokemonRegistry` built-in-count constructor expression to cast the final built-in ID before adding one, without changing the registered catalog or runtime behavior.
- Updated the generated Zed compilation database for the nature-configuration source and new Pokemon-configuration translation unit. This patch did not add dedicated nature- or Pokemon-configuration tests.

## [0.11.6] - 2026-08-22

### Added

- Added the nature domain:
  - `NatureID` is an open tagged identifier with `NO_NATURE_ID`; `BuiltinNatureID` defines `None`, the 25 standard natures, and the `FinalNature` sentinel, with corresponding display-name constants and conversion to the open ID.
  - `NatureMeta` stores six stat multipliers, an owned `EffectTrigger` vector, a non-owning name, a `BattleTargetID`, and its stable `NatureID`. Configuration constants define `MAX_NATURES == 1'000`, `MAX_NATURES_PER_POKEMON == 1`, and neutral, boosted, and weakened stat factors of `1.0`, `1.1`, and `0.9`.
  - `NatureRegistry` registers `None` plus all 25 standard natures. Hardy, Docile, Serious, Bashful, and Quirky are neutral; every other built-in applies the standard boost and weakness to its canonical attack, defense, special-attack, special-defense, or speed pair. The registry exposes metadata/name/ID queries, registered spans and counts, containment checks, and the fixed-registry mutation primitives used by configuration code.
  - Declared `NatureRegistryConfiguration` and its duplicate/not-found policy, query facade, single and batch additions, rename, stable-ID-preserving update, removal, and runtime-registry access. Its out-of-line mutation definitions are added in 0.12.0.
- Added the Pokemon metadata domain:
  - `PokemonID` is an open tagged identifier with `NO_POKEMON_ID`; `BuiltinPokemonID` defines `None`, the Bulbasaur, Charmander, and Squirtle evolutionary families, and `FinalPokemon`.
  - `PokemonMeta` contains four move IDs and four maximum-PP values, two type IDs, a ten-entry ability pool, one item ID, six base stats, level, non-owning name, stable `PokemonID`, and logical ability-pool count. Constants define `MAX_POKEMON == 10'000` and `MAX_ABILITY_POOL_PER_POKEMON == 10`.
  - `PokemonRegistry` registers `None` and the nine starter-family species with their built-in moves, PP, types, ability pools, item, levels, and base stats. It exposes metadata/name/ID lookup, registered spans and counts, containment checks, next-ID state, and the fixed-registry mutation primitives; a Pokemon configuration facade is not added until 0.12.0.
- Added nature-aware battle dispatch. `EffectContext` carries a `NatureID`; `EffectSource` adds `Nature`; suppression rules can constrain `mTargetNatureID`; and `BattleEngine::executeNatureTrigger()` executes matching nature effects through the same target and suppression pipeline used by abilities and items. Slot events dispatch abilities, then items, then each equipped nature, and nature sources participate in self-suppression exclusion and source restoration.

### Changed

- Changed runtime `Pokemon` storage from scalar ability and item IDs to fixed arrays of one ability, one item, and one nature. Constructors, whole-array and indexed accessors, and setters now operate on those arrays while retaining the existing four move, PP, two type, status, stat, level, and health state.
- Changed numeric and registry-resolved Pokemon stream output to print plural ability and item sections plus nature IDs/names. Name resolution uses the corresponding provider registry and retains the `<unregistered>` fallback for missing metadata or null registry pointers.
- Added a non-owning `const NatureRegistry *natureRegistry` to `RegistryProvider`; no Pokemon-registry provider member exists in this revision. The example constructs `NatureRegistryConfiguration`, constructs `PokemonRegistry` directly, equips runtime Pokemon with ability/item/nature arrays, and wires only the nature registry into the provider.
- Added `Final...` built-in sentinels across the existing ability, effect, item, move, multiplier, status, terrain, type, and weather domains where required by the registry initialization pattern. Updated affected handlers, tests, and generated Zed compilation-database data for the new identifiers and array-based Pokemon API.

## [0.11.5] - 2026-08-21

### Added

- Added `MultiplierApplicationPolicy::RoundHalfDown` before `Floor` and `Other`. It becomes the `MultiplierMeta` default and the fallback for an unregistered multiplier ID.

### Changed

- Assigned built-in multiplier policies to match their calculation stages: Ability and Item use `Other`; Critical Hit, Randomization, and Type Effectiveness use `Floor`; Targets, Population Bomb, Weather, STAB, Burn Damage Reduction, and the remaining defaulted entries use `RoundHalfDown`.
- Refined `EffectContext::applyMultiplier()`:
  - A reusable helper rounds values above one half up and exact or tolerance-close halves down. `RoundHalfDown` applies that helper directly, while `Floor` applies `std::floor` directly.
  - `Other` multipliers accumulate into a fixed-point value initialized to `4096`, rounding each accumulated product. After all active multipliers, the accumulator is divided by the `4096` denominator and applied once with half-down rounding.
  - Non-finite inputs still normalize to `1.0`, negative inputs still clamp to `0.0`, and damage remains at least one after each active multiplier and after the final deferred application.
- Updated multiplier tests for the new built-in assignments; in particular, Ability multiplier `11 * 1.6` changes from the initial floor result `17` to the deferred half-down result `18`.

## [0.11.4] - 2026-08-21

### Fixed

- Added the previously omitted `src/Pokemon/pokemon.cpp`, resolving the missing out-of-line stream definitions declared and tested in 0.11.2.
- Defined `operator<<` to serialize Pokemon state with numeric ability, item, type, status, and move IDs; level and level-damage factor; health and stats; and current/maximum PP for every move slot.
- Defined `printPokemonWithNames()` to resolve ability, item, type, status, and move names through `RegistryProvider`, emit both stable IDs and names, preserve PP output, and use `<unregistered>` when metadata is absent or the corresponding registry pointer is null. GCC-only diagnostic guards suppress the inapplicable reference-return `returns_nonnull` suggestion.

## [0.11.3] - 2026-08-21

### Changed

- Changed `RandomizationHandler::applyRandomization()` to obtain const user and target slots through `IEffectHandler` helpers, then log the user side/index/name, target side/index/name, and generated random multiplier with `Logger::info` before storing the Randomization multiplier in the effect context. The `RegistryProvider` parameter remains unused.
- Cleaned the handler declaration/includes and applied clang-tidy-driven const qualification and formatting in `BattleEngine`, `EffectContext`, and the touched ability-configuration and Pokemon tests. No additional battle behavior changed.

## [0.11.2] - 2026-08-21

### Added

- Added the initial `MultiplierApplicationPolicy` with `Floor` and `Other`, and added `MultiplierMeta::mApplicationPolicy` defaulting to `Floor`.
- Declared out-of-line Pokemon stream APIs `operator<<` and `printPokemonWithNames()` and added stream tests specifying numeric state, identifiers, move slots, and PP output. This revision did not include `src/Pokemon/pokemon.cpp`, so the declarations were not yet backed by linked definitions; the missing translation unit is supplied in 0.11.4.

### Changed

- Changed `EffectContext::applyMultiplier()` to require a `const MultiplierRegistry &` and resolve the application policy for every active multiplier. Missing metadata falls back to `Floor`; non-finite values normalize to `1.0`; negative values clamp to `0.0`; and the result is clamped to at least one after each active multiplier.
- Implemented the initial policy behavior: `Floor` computes `floor(damage * normalizedMultiplier)`, while `Other` computes the revision's `4096` fixed-point value and half-down rounds `damage * fixedPointValue` immediately. Tests specify `11 * 1.6 == 17`, `11 * 1.5 == 16`, `100 * 0.85 == 85`, and a zero multiplier applied to zero base damage clamping to one.
- Changed `BattleEngine` damage application to pass the provider's multiplier registry into `applyMultiplier()`.
- Expanded the example battle with richer Pokemon names, move/PP sets, typings, opposing identities, and post-turn health output. The Pokemon stream implementation was moved to the intended out-of-line API contract, but no Pokemon source file was present in this patch.

## [0.11.1] - 2026-08-21

### Added

- Added `EffectMeta::mMayChangeStatus` and `EffectMeta::mMayChangeWeather`, both defaulting to `false`. Built-in metadata marks Status Apply and Status Remove as status mutators, and Set Sandstorm, Set Sun, and Set Rain as weather mutators. Status Turn Skip, Status Tick, and the remaining built-in effects retain the defaults in this patch.
- Added effect-driven battle notifications in `BattleEngine::executeEffect()`:
  - For flagged effects, the engine captures the prior weather and, when the context identifies an existing target Pokemon, that Pokemon's status array before invoking the registered handler.
  - A changed weather ID dispatches `WeatherChanged` with role `Any` to every healthy active slot on both sides. A changed target status array dispatches `StatusChanged` with role `Target` to that target.
  - A missing target slot or null target Pokemon does not prevent the handler from running, but it leaves no status snapshot to compare and therefore emits no `StatusChanged` event. Weather change detection remains independent of target availability.

### Changed

- Changed ability, item, and move trigger effect vectors from closed `BuiltinEffectID` values to open `EffectID` values. `BattleEngine::executeEffect()`, `executeEffects()`, and targeted-effect dispatch now accept `EffectID` directly and resolve it without reconverting a built-in ID.
- Changed static move effect arrays and their span-returning helpers to `EffectID`, converting each built-in entry with `toEffectID()`. Built-in ability and move metadata use the same conversion, allowing registry-defined effects to share the existing dispatch path.
- Changed Cheri Berry and Chesto Berry from `TurnEnd` triggers to `StatusChanged` with role `Target`; both continue to run Status Remove, now stored as an `EffectID`.

## [0.11.0] - 2026-08-21 (BattleEngine Update)

### Added

- Added three private `BattleEngine` execution helpers:
  - `executeWeightHitCountPolicy()` runs the target-role `BeforeHit` move trigger once per target for `WeightedHitCount`, with `mHitAttemptIndex == 1`. A miss or cleared `mShouldContinue` skips that target; otherwise, suppressions activated by `BeforeHit` become the per-hit baseline.
  - `executeFixedHitCountPolicy()` runs the target-role `BeforeHit` move trigger for every `FixedHitCount` attempt and stops the target's hit loop when that trigger marks a miss. The enclosing loop, rather than this helper, checks `mShouldContinue`.
  - `executeDamageApplication()` applies damage only when `mShouldApplyDamage` is true and base damage is nonzero. It calls `EffectContext::applyMultiplier()`, subtracts the resulting `us` value, and clamps lethal or excess damage to zero health.

### Changed

- Completed `BattleEngine::executeMove()`:
  - Re-resolves the acting Pokemon, move metadata, and legal targets from the validated action; missing metadata or failed target resolution returns without consuming PP.
  - Clears prior suppressions, consumes one PP, resolves the hit count, and dispatches `MoveUse` to the user's ability/item sources with role `User`, then to the move's role-`User` effects. Clearing `mShouldContinue` ends the move.
  - Preserves move-wide suppressions for every target. Each existing target receives a copied context with updated coordinates and a role-`Target` `MoveUse` dispatch; suppressions produced there become the target baseline.
  - Restores the target suppression baseline for each hit, assigns a one-based attempt index, dispatches `DamageCalculation` to user then target slots, executes the move's role-`Target` `Hit` effects, applies damage and recoil, and processes faints.
  - After a non-miss, dispatches the user's role-`User` `Hit` event and then the move's role-`Target` `AfterHit` effects. A cleared `mShouldContinue` ends the target loop; otherwise, the next attempt inherits the context but resets damage state and active multipliers.
  - Skips vanished targets, stops attempts when a target disappears, and clears all active suppressions when execution finishes or is cancelled.

## [0.10.11] - 2026-08-21

### Changed

- Changed the CodeQL workflow build command from `make` to `make -j$(nproc)`.
- Changed the test workflow build command from `make build_tests` to `make -j$(nproc) build_tests`.

## [0.10.10] - 2026-08-21

### Changed

- Changed the declaration and definition of `hasReserve()` from `const std::span<Pokemon *> &` to `const std::span<Pokemon *const> &`.
- The span now exposes const pointer elements, so pointers cannot be reassigned through this view. The pointed-to `Pokemon` objects remain mutable because the pointee type is still `Pokemon`, not `const Pokemon`.

## [0.10.9] - 2026-08-21

### Changed

- Added Doxygen contracts to the existing `BattleEngine` constructor, `executeTurn()`, `startBattle()`, `getState()`, and `switchPokemon()` APIs, including ownership, lifetime, phase, error, and action-count behavior.
- Added Doxygen contracts to existing battle helpers covering side selection, active-slot and party access, health and adjacency queries, target existence, context construction, recoil and hit-count resolution, party validation and assignment, result/reserve calculation, move-target resolution, move/switch validation, action visitation, and move prioritization.
- Changed `hasReserve()` in this documentation pass from `const std::vector<Pokemon *> &` to `const std::span<Pokemon *> &` in both declaration and definition; its algorithm did not change.
- Applied formatting and include-order changes to battle headers, the historically named `effectSourceAndSuppression.h`, move metadata, and the move-registry configuration test. No other runtime behavior changed.

## [0.10.8] - 2026-08-18

### Added

- Added `BattleEventID : ub` with ordered values `BattleStart`, `SwitchIn`, `TurnEnd`, `DamageCalculation`, `MoveUse`, `Hit`, `AfterHit`, `Faint`, `WeatherChanged`, and `StatusChanged`.
- Added `BattleEventRole : ub` with `Any`, `User`, and `Target`, separating participant perspective from event identity.
- Added `mRole` to `AbilityEffectTrigger`, `ItemEffectTrigger`, and `MoveEffectTrigger`. It defaults to `BattleEventRole::Any`; built-in metadata assigns explicit roles where an old trigger encoded user or target perspective.

### Changed

- Replaced `BattleTriggerID` throughout metadata, suppression rules, engine dispatch, helpers, and tests with the `BattleEventID` plus `BattleEventRole` pair.
- Collapsed perspective-specific damage-calculation, move-use, and successful-hit triggers into shared event IDs carrying `User` or `Target`; life cycle events use `Any` unless metadata requires a participant role.
- Changed suppression matching and activation APIs to receive both event ID and role, so a rule must match the event, role, source kind, and any optional source ID before suppressing dispatch.
- Changed move-trigger and slot-trigger dispatch signatures to carry roles; trigger metadata must match both fields before effects execute.
- Added required standard-library includes to battle helpers and replaced repeated fully qualified registry-domain names with local `using` declarations. Registry behavior did not change.

### Removed

- Removed `BattleTriggerID` and its perspective-specific enumerators after all call sites moved to `BattleEventID` and `BattleEventRole`.

## [0.10.7] - 2026-08-18

### Added

- Added public `BattleEngine::executeTurn(std::span<const BattleAction>) -> std::expected<void, BattleEngineError>`:
  - Rejects un-started, finished, or replacement-waiting battles and requires exactly one action for every healthy active slot.
  - Validates each move or switch through `getValidationResult()`, rejects duplicate acting slots, and partitions valid actions into switches and moves.
  - Executes switches in submitted order, then shuffles and stable-sorts moves by descending signed priority and descending effective speed so shuffle order breaks exact ties.
  - Re-validates each move immediately before execution, skips an action if its actor is no longer healthy, executes remaining moves, runs turn-end triggers for each healthy slot in side-A/side-B and slot order, processes faints, and refreshes battle phase.
- Added `getValidationResult()` to visit `BattleAction`, `getBattleTarget()` to derive the acting slot from either variant, and `handleMovePrioritization()` to perform randomized tie-breaking followed by priority/speed ordering.
- Added initial private `executeMove()` behavior: resolve metadata and targets, consume PP, build one context per target, execute move `BeforeHit`, ability/item user and target damage-calculation triggers, move `OnHit`, recoil and faint processing, user successful-hit triggers, and move `AfterHit`.
- Added `executeEndTurnTrigger()` to dispatch `OnTurnEnd` to every healthy active slot.
- Marked `FixedMetadataRegistry::getMetadata()` `ATTR_NOINLINE` in its declaration and definition.
- Added a `main.cpp` example that creates configured registries and an effect registry, starts a battle, submits a move action, and reads state.

### Removed

- Removed the unused `configFlags.h` header.

## [0.10.6] - 2026-08-17

### Changed

- Added the GNU-style `ATTR_CONST` optimization attribute to the `BattleEngine::getState()` declaration and definition. This compiler function attribute does not change C++ const qualification, ownership, or the returned reference.
- Wrapped the GCC-only definition with diagnostic push/pop directives suppressing `-Wsuggest-attribute=returns_nonnull`, whose suggested attribute cannot be applied to a reference return. Runtime behavior is unchanged.

## [0.10.5] - 2026-08-17

### Added

- Added `BattleEngine::getState() const noexcept`, returning a read-only reference to engine-owned `BattleState` valid for the engine's lifetime.
- Added private `switchPokemon()` implementation:
  - During `AwaitingReplacements`, rejects any action not naming a slot in `mRequiredReplacements` with `ReplacementRequired`.
  - Reuses `validateSwitchAction()` and returns its error without mutation.
  - Replaces the active slot with a freshly initialized `BattleSlot`, preserving only battlefield position and selecting the party pointer at `mPartyIndex`; occupant-specific stages, counters, modifiers, flags, and faint state reset.
  - Dispatches `OnSwitchIn` followed by `OnHazardSwitchIn`, then processes resulting faints and refreshes phase/replacement state.

## [0.10.4] - 2026-08-17

### Added

- Declared private `BattleEngine::executeMove()`, `executeMoveTrigger()`, and `executeMoveEffects()` integration points. In this revision, `executeMove()` remains a placeholder and `executeMoveEffects()` has no implementation; only trigger dispatch is implemented.
- Implemented `executeMoveTrigger()` to process matching move-trigger records in declaration order, activate each record's suppression rules before suppression evaluation, execute unsuppressed built-in effects, and restore the caller's prior `EffectSource`.
- Added `getMoveTargets()` to validate acting/move slots, resolve current move metadata, and delegate selector/range expansion to `resolveTargets()`.
- Added `validateMoveAction()` to reject an un-started or finished phase, invalid/fainted users, invalid move slots, zero PP, missing move metadata, and invalid target resolution. Accuracy is not validated here.
- Added `validateSwitchAction()` to reject invalid active or party indices, null/fainted incoming Pokemon, and Pokemon already active on that side.

### Changed

- Moved target resolution from `BattleEngine` into battle helpers. `canTarget()` enforces live occupancy and optional adjacency; `appendSide()` preserves active-slot order; `resolveTargets()` implements all five target selectors and returns `InvalidTarget` for absent, illegal, or empty selections.

## [0.10.3] - 2026-08-14

### Added

- Added the first `BattleEngine` API, constructed from non-owning `RegistryProvider *` and `EffectRegistry *`, with engine-owned `BattleState`, phase, active suppression records, and required replacements.
- Added `startBattle()` returning `std::expected<void, BattleEngineError>`:
  - Requires non-null provider/effect registry and all provider registry pointers; rejects repeated starts, null/duplicate party pointers, empty parties, active counts outside `1..3`, insufficient healthy Pokemon, and parties smaller than the active count.
  - Copies both pointer spans into engine-owned party vectors, assigns the first healthy members to active slots, marks the battle started, sets the action phase, dispatches battle-start and switch-in ability/item triggers, then processes faints.
- Added ability/item effect dispatch with metadata-order execution, mutable shared `EffectContext`, effect-registry lookup, null metadata/function checks, ability-before-item ordering, metadata target resolution, and restoration of outer context coordinates/source.
- Added suppression scopes:
  - Active records retain each `SuppressionRule`, owning slot, and source kind.
  - Rule counts are clamped to physical spans; matching checks source, trigger, optional ability/item/move ID, and excludes a source from suppressing itself on its owning slot.
  - Standalone slot triggers clear suppressions before and after dispatch; ability and item rules activate before either source executes.
- Added faint processing that marks `mFaintProcessed` before dispatch to prevent duplicate recursive events, runs fainted ability/item effects, and recomputes battle phase and required replacements.
- Added phase refresh: un-started battles become `NotStarted`, decided results become `Finished`, and otherwise unhealthy active slots are requested only when that side has a healthy reserve; phase becomes `AwaitingReplacements` or `AwaitingActions`.
- Added party/state helpers for null/duplicate detection, healthy counts, deterministic active assignment, target eligibility, side appending, result calculation, and healthy non-active reserve detection.
- Added mutable and const `getRuntimeRegistry()` accessors to each domain registry configuration, exposing configuration-owned runtime registries without transferring ownership.
- Added explicit default construction and out-of-line destruction for `BattleState`; copy and move operations are deleted.
- Updated `main.cpp` to construct configuration-owned registries, form a `RegistryProvider`, construct an explicit `EffectRegistry`, and pass both pointers to `BattleEngine`.

### Changed

- Replaced repeated qualification in multiplier-registry configuration with a local namespace `using` declaration; behavior is unchanged.

### Fixed

- Added GCC-only diagnostic guards around reference-returning `activeSlots()` overloads to suppress an inapplicable `returns_nonnull` suggestion.

## [0.10.2] - 2026-08-13

### Added

- Added `battleAction.h` as the shared action model:
  - `BattleTarget` stores `Side mSide` and zero-based `ub mSlotIndex` and has default equality.
  - `MoveAction` stores an optional selected target, acting side, zero-based active user slot, and zero-based move slot.
  - `SwitchAction` stores side, zero-based active slot to replace, and zero-based incoming party index.
  - `BattleAction` is `std::variant<MoveAction, SwitchAction>`.
- Added `BattleEngineError : ub` values `BattleAlreadyStarted`, `BattleNotStarted`, `BattleFinished`, `InvalidParty`, `InvalidActiveSlot`, `InvalidPartyIndex`, `InvalidMoveSlot`, `InvalidTarget`, `DuplicateAction`, `ReplacementRequired`, `PokemonFainted`, `PokemonAlreadyActive`, `MoveNotFound`, `AbilityNotFound`, `ItemNotFound`, `NoPP`, and `MissingRegistry`.
- Added `BattleResult : ub` values `NotStarted`, `InProgress`, `SideAWon`, `SideBWon`, and `Draw`, plus `BattlePhase : ub` values `NotStarted`, `AwaitingActions`, `AwaitingReplacements`, and `Finished`.

### Changed

- Moved the existing `BattleTarget` and `MoveAction` definitions from `battleHelpers.h` into `battleAction.h`; their data layout and defaults are unchanged.

## [0.10.1] - 2026-08-13

### Changed

- Applied formatting-only changes to the fixed-metadata registry benchmark; benchmark operations and ranges are unchanged.

### Removed

- Removed the `EffectRegistry *effectRegistry` member and associated forward declarations/usings from `RegistryProvider`.
- Removed the extra effect-registry field from every test `RegistryProvider` aggregate initializer. Tests no longer provide an effect registry through the provider; explicit `BattleEngine` wiring is introduced in 0.10.3.

## [0.10.0] - 2026-08-12 (Effect Registry Update)

### Added

- Added open `EffectID`, with zero-valued `NO_EFFECT_ID`, and `BuiltinEffectID : ub` containing 22 values: `None`, `CriticalHit`, `BaseDamage`, `Targets`, `Weather`, `Terrain`, `PopulationBomb`, `Randomization`, `Stab`, `TypeEffectiveness`, `BurnDamageReduction`, `Flinch`, `Recoil`, `StatusApply`, `StatusRemove`, `StatusTurnSkip`, `StatusTick`, `AccuracyCheck`, `SetSandstorm`, `SetSun`, `SetRain`, and `PsychicTerrainPriorityBlock`.
- Added `EffectMeta` with non-owning `std::string_view mName`, `EffectFunction mApply`, and stable `EffectID mEffectID`. `EffectFunction` is `void (*)(BattleState &, EffectContext &, const RegistryProvider &)`. No effect flags exist in this revision.
- Added `EffectRegistry`, privately based on `FixedMetadataRegistry` with `MAX_EFFECTS == 1'000`. Construction registers all 22 built-ins; public wrappers expose metadata/name/ID lookup, registered spans, containment, index lookup, counters, mutation primitives, and next-ID management.
- Added `EffectRegistryConfiguration` with configuration-owned storage and add, batch-add, rename, metadata replacement, remove-by-name/ID, lookup, containment, count, and registered-span APIs through `std::expected`; errors add `EffectAlreadyExists` and `EffectNotFound` branches.
- Added free inline handler entry points used as `EffectFunction` values for existing class handlers and new flinch, recoil, psychic-terrain priority block, weather-setting, and status apply/remove/tick/turn-skip implementations.
- Added `EffectRegistry *` to `RegistryProvider` in this revision, updated provider aggregates in tests, and demonstrated effect configuration/lookup in `main.cpp`. Version 0.10.1 removes this provider member.

### Changed

- Replaced closed `EffectTypeID` throughout ability/item/move trigger vectors, registries, handlers, and tests with `BuiltinEffectID`; runtime registry APIs use open `EffectID` values.
- Changed built-in metadata and tests to use registered handler functions directly.

### Removed

- Removed `effectType.h` after its enum moved to `builtInEffectID.h` and `effectID.h`.
- Removed predefined move-effect helpers `baseAttackEffects()`, `baseAttackWithRecoil()`, `baseAttackWithStatus()`, `baseAttackWithFlinch()`, `statusChangeEffects()`, `protectEffects()`, and `fieldEffectEffects()`, together with their tests; built-in registries now define effect lists directly.

## [0.9.19] - 2026-08-07

### Changed

- Changed only mutable `activeSlots(BattleState &, Side)` and `party(BattleState &, Side)` declarations and definitions from `ATTR_PURE` to GNU-style `ATTR_CONST` compiler attributes.
- This is an optimization contract, not C++ `const` qualification and not a change to the mutability of returned references.

## [0.9.18] - 2026-08-07

### Changed

- Changed only the const `activeSlots(const BattleState &, Side)` and `party(const BattleState &, Side)` overload declarations and definitions from the GNU-style `ATTR_PURE` compiler attribute to `ATTR_CONST`.
- This changes the compiler optimization contract, not C++ `const` qualification or the mutability represented by the returned references.

## [0.9.17] - 2026-08-07

### Changed

- Added the GNU-style `ATTR_PURE` compiler attribute to the declarations and definitions of 13 battle-helper overloads/functions: both `activeSlots()` overloads, `contextSlot()`, both `party()` overloads, both `isHealthy()` overloads, `isActive()`, `isAdjacent()`, `targetExists()`, `sideHasHealthyPokemon()`, `getEffectiveSpeed()`, and `makeMoveContext()`.
- No function bodies changed. In particular, the mutable `activeSlots()` and `party()` overloads still return mutable references, and `contextSlot()` still returns a mutable pointer; this entry records the compiler attributes applied by the patch rather than asserting that mutation through those results is side-effect-free.

## [0.9.16] - 2026-08-07

### Added

- Added `BattleTarget` to `battleHelpers.h` with a `Side`, zero-based `ub` slot index, and default equality, and added `MoveAction` with an optional selected target, acting side, zero-based user-slot index, and zero-based move-slot index.
- Added five battle helpers:
  - `contextSlot()` returns a mutable slot pointer for a valid side/index pair or `nullptr` when the index is outside that side's active-slot vector.
  - `getEffectiveSpeed()` returns zero for an empty slot; otherwise it multiplies the Pokemon's speed by the cached speed-stage multiplier and the slot's speed modifier.
  - `makeMoveContext()` copies move identity, type, power, accuracy, range, hit index, source, participant coordinates, and damage category into a move-sourced `EffectContext`.
  - `applyRecoil()` ignores missing users, zero damage, and non-finite or non-positive recoil ratios; otherwise it floors damage times the recoil ratio, clamps recoil to `1..us::max`, and prevents health underflow.
  - `resolveHitCount()` returns fixed counts directly and performs weighted selection over finite positive outcomes, returning zero for an invalid total weight.
- Added `BattleSlot::mFaintProcessed`, defaulting to `false`.
- Added a documentation-only `battleEngine.cpp` translation unit.

### Changed

- Changed the stage, accuracy, and evasion multiplier constants, generated cache arrays, and cache temporaries from `float` to `double`; changed accuracy-handler calculations and base-damage stage multipliers to `double` accordingly.
- Changed `EffectContext::applyMultiplier()` to normalize each active multiplier with `std::max(multiplier, 1.0)` before fixed-point application. In this revision, zero, negative, and fractional multipliers below one therefore act as `1.0`; after each active multiplier, damage is still clamped to at least one.
- Moved `AccuracyCheck` out of the common move-effect arrays and added it as a separate `BeforeHit` trigger for Pound and Karate Chop. Their existing damage sequence remains an `OnHit` trigger.
- Gave the remaining common effect arrays explicit element types and sizes: base attack has 10 effects; recoil, status, and flinch variants have 11; and status change has two.
- Updated effect-context, move-effect-list, and move-registry tests for multiplier normalization, the shorter arrays, removed helper lists, and the two-trigger built-in move metadata.
- Applied formatting-only changes to ability, item, and move registry-configuration declarations, one ability-configuration definition, and Pokemon constructor initializers.

### Removed

- Removed `BattleSlot::mSpeedBoost`; `getEffectiveSpeed()` now uses `mDamageFormulaModifiers.mSpeedModifier` with the Pokemon's speed and cached stat-stage multiplier.
- Removed `AccuracyCheck` from `BASE_ATTACK_EFFECTS`, `BASE_ATTACK_WITH_RECOIL`, `BASE_ATTACK_WITH_STATUS`, `BASE_ATTACK_WITH_FLINCH`, and `STATUS_CHANGE_EFFECTS`; the effect type itself remains available.
- Removed the `PROTECT_EFFECTS` and `FIELD_EFFECT_EFFECTS` arrays, their `protectEffects()` and `fieldEffectEffects()` accessors, and their move-metadata test cases.

## [0.9.15] - 2026-08-05

### Added

- Added a Pokemon health-boundary test that verifies construction retains the initial maximum health and `setHealth()` clamps a value above that maximum.

### Changed

- Changed `MoveMeta::mPriority` from `ub` to `sb`, allowing metadata to represent negative as well as nonnegative priorities.
- Changed `EffectContext::applyMultiplier()` to return zero immediately when it encounters an active multiplier less than or equal to `0.0`; positive multipliers continue through the existing fixed-point rounding and minimum-one clamp.

## [0.9.14] - 2026-08-05

### Added

- Added battle-level `BattleTriggerID`, `BattleTargetID`, and `BattleRangeID` enums in `battleTargetsAndTriggers.h` for shared ability, item, and move metadata.
- Added `battleHelpers.h` and `battleHelpers.cpp` with `getOppositeSide()`, `getSideOrder()`, mutable and const `activeSlots()` and `party()` overloads, `isHealthy()` overloads for slots and Pokemon pointers, `isActive()`, `isAdjacent()`, `targetExists()`, and `sideHasHealthyPokemon()`.
- Added Pokemon maximum-health state initialized from constructor health, `getMaximumHealth()`, and `setMaximumHealth()`. Health assignments and maximum-health reductions clamp current health to the maximum.

### Changed

- Changed `AbilityEffectTrigger`, `ItemEffectTrigger`, and `MoveEffectTrigger` to store `BattleTriggerID`; the three metadata record types remain distinct. Changed ability, item, and move targets to `BattleTargetID`, and changed move range and effect-context range override to `BattleRangeID`.
- Changed `SuppressionRule::mTargetTrigger` from a variant of the three domain trigger enums to one `BattleTriggerID`.
- Updated built-in registries, registry-configuration APIs, target handling, and their tests to use the shared battle IDs.
- Changed `MoveMeta::mPower` from `ub` to `us`.

### Removed

- Removed `abilityTargetsAndTriggers.h`, `itemTargetsAndTriggers.h`, and `moveTargetsAndTriggers.h` after their trigger, target, and range enums moved to the battle-level header.

## [0.9.13] - 2026-08-04

### Added

- Added `MAX_SUPPRESSION_RULES_PER_TRIGGER` with value `2`.
- Added `SuppressionRule` with optional `mTargetAbilityID`, `mTargetItemID`, and `mTargetMoveID` constraints, a `std::variant<AbilityTriggerID, ItemTriggerID, MoveTriggerID>` named `mTargetTrigger`, and an `EffectSource` named `mTargetSource`.
- Added a fixed `std::array<SuppressionRule, MAX_SUPPRESSION_RULES_PER_TRIGGER>` and the historically spelled `ub mSuppressionRuleCount` to `AbilityEffectTrigger`, `ItemEffectTrigger`, and `MoveEffectTrigger`.

### Changed

- Moved `AbilityEffectTrigger`, `ItemEffectTrigger`, and `MoveEffectTrigger` from their domain target/trigger headers into `abilityMeta.h`, `itemMeta.h`, and `moveMeta.h`, respectively.
- Moved `EffectSource` from `effectContext.h` into the new, historically misspelled `effectSourceAndSuppression.h` alongside `SuppressionRule`.
- Applied formatting-only changes to `typeRegistryConfiguration.cpp`.

## [0.9.12] - 2026-08-04

### Added

- Added `AbilityTriggerID::OnSuccessfulHit`.

### Changed

- Changed the built-in Stench ability's `Flinch` effect trigger from `AbilityTriggerID::OnDamageCalc` to `AbilityTriggerID::OnSuccessfulHit` and updated its registry test expectation.

## [0.9.11] - 2026-08-04

### Added

- Added `MAX_STATUSES_PER_POKEMON` with value `5`; the existing `MAX_STATUSES` registry capacity remains `1'000`.
- Added `StatusInteractionAction` values `Coexist`, `ReplaceCurrent`, `RemoveCurrent`, and `BlockIncoming`, plus `StatusInteraction` records and `StatusMeta::mStatusInteractions`.
- Added status helpers `hasInteraction()`, `willBlockIncoming()`, `statusAlreadyExists()`, `statusReplaceHandler()`, `statusRemoveHandler()`, and `shiftAndGetNextAvailableStatus()` for metadata lookup, blocking, duplicate detection, replacement, removal, and stable compaction.
- Added `Pokemon::addStatus(StatusID, const StatusRegistry &)`. It ignores the no-status ID and duplicates, applies blocking before mutation, performs replacements and removals, compacts active IDs, clears trailing slots, and appends only when no replacement occurred and capacity remains.
- Added built-in interaction metadata for Freeze blocking/removal relationships and Toxic replacing Poison.
- Added Pokemon tests for blocking precedence, single and multiple replacement, removal with compaction, and a full array with no applicable interaction.

### Changed

- Replaced Pokemon's single `StatusID` with `std::array<StatusID, MAX_STATUSES_PER_POKEMON>`. Added `getStatusesArray()` returning a const array reference, `setStatusesArray()`, and indexed `getStatusID(us)`, which asserts that the index is in range; this revision does not add a span-returning status accessor.
- Changed burn detection to search the fixed status array through `statusAlreadyExists()`.
- Marked `FixedMetadataRegistry::addBuiltin()` `ATTR_NOINLINE`.
- Marked the built-in Cheri Berry and Chesto Berry metadata as consumable.
- Renamed `BattleSlot::mForceGrounded` to `mIsGrounded` and reordered `mPosition` before the slot counters.

### Removed

- Removed `BattleSlot::mItemConsumed`.
- Removed `Pokemon::setStatus()` with the single-status field.

## [0.9.10] - 2026-08-04

### Added

- Added exactly two built-in ability enum values, `Levitate` and `Elevate`, and the built-in item enum value `AirBalloon`; this patch does not add registry metadata or grounding behavior for those IDs.
- Added `BattleSlot::mForceGrounded`, defaulting to `false`.
- Added terrain constants `ELECTRIC_BUFF_IN_TERRAIN_BASE_DAMAGE_VALUE`, `GRASS_BUFF_IN_TERRAIN_BASE_DAMAGE_VALUE`, and `PSYCHIC_BUFF_IN_TERRAIN_BASE_DAMAGE_VALUE`, each `1.3`, plus `DRAGON_DEBUFF_IN_TERRAIN_BASE_DAMAGE_VALUE` at `0.5`.
- Added helper predicates `battleSlotHasType()`, `battleSlotHasAbilityByName()`, `battleSlotHasAbilityByID()`, `battleSlotHoldsItemByName()`, `battleSlotHoldsItemByID()`, `isBattleSlotUngrounded()`, and `isBattleSlotGrounded()`, plus const user/target slot accessors on `IEffectHandler`.
- Added `TerrainHandler`. It returns when the target is ungrounded; otherwise matching Electric, Grass, Psychic, or Misty/Dragon combinations multiply the user's attack modifier by the corresponding terrain constant.
- Added terrain-handler tests for grounded and Flying targets under Electric terrain, a Psychic-terrain boost, and the `mForceGrounded` override.

### Changed

- Changed all six `DamageFormulaModifiers` members from `float` to `double` and changed base-damage intermediate modifiers to `double`.
- Changed `IEffectHandler::apply()` and all existing overrides from `const BattleState &` to `BattleState &`; changed `getTeamConst()` to accept mutable `BattleState &`, while read-only handler paths use the new const slot accessors.
- Defined `isBattleSlotUngrounded()` in this revision as Flying type unless `mForceGrounded` is true. `Levitate`, `Elevate`, and `AirBalloon` are not consulted.
- Changed the existing ability-name constants to `inline constexpr`.

## [0.9.9] - 2026-08-04

### Fixed

- Added the missing `#include <vector>` directive to `src/EffectHandler/weatherHandler.cpp`.

## [0.9.8] - 2026-08-04

### Added

- Added built-in ability IDs `AirLock` and `CloudNine` and the built-in move ID `HydroSteam`.
- Added `Move::FixedHitCount`, `WeightedHitCountOutcome`, and `WeightedHitCount`; `HitCountPolicy` is a `std::variant` of the fixed and weighted policies. Fixed counts store a `ub`, weighted outcomes store a `ub` hit count and `double` weight, and weighted policies own a `std::vector` of outcomes.
- Added `MoveTriggerID::BeforeHit`, `MoveTriggerID::OnHit`, and `MoveTriggerID::AfterHit`.
- Added `BuiltinMultiplierID::PopulationBomb`, display metadata for the `Targets` and `Population Bomb` multipliers, and built-in registry entries for both.
- Added Population Bomb constants of `1.0` for hit-attempt index 1 and `0.5` for later attempts. `PopulationBombHandler` writes the corresponding multiplier for indices 1 and greater and leaves index 0 unchanged.
- Added weather constants for nullification (`1.0`), Water in Rain (`1.5`), Fire in Rain (`0.5`), Fire in Harsh Sunlight (`1.5`), and Water in Harsh Sunlight (`0.5`).
- Added `WeatherHandler`. Air Lock or Cloud Nine on any Pokemon in either party sets the Weather multiplier to `1.0` and returns; otherwise Rain boosts Water and weakens Fire, while Harsh Sunlight boosts Fire or Hydro Steam and weakens other Water moves.

### Changed

- Replaced `MoveMeta::mHitDistribution`, a `std::vector<std::pair<ub, float>>` defaulting to one certain hit, with `mHitCountPolicy`, which defaults to `FixedHitCount{1}`.
- Replaced `EffectContext::mTotalHits` and `mCurrentHit` with the zero-initialized `ub mHitAttemptIndex` field.
- Renamed `BattleState::mWeather` and `mTerrain` to `mWeatherID` and `mTerrainID`.
- Renamed the `Types` enum to `BuiltinTypeID` and updated registry, configuration, Pokemon, handler, and test call sites.
- Changed Pound, Karate Chop, and test move definitions from `OnTarget` to `OnHit`.
- Changed both `MoveRegistryConfiguration::updateMove()` overloads to copy each `MoveMeta` member explicitly, including the new hit-count policy.

### Fixed

- Added the `ATTR_NOINLINE constexpr` `Detail::cloneMetadata()` helper and routed generic metadata mutation copies through it to avoid copy-related compiler inlining diagnostics.
- Corrected the braced initialization in `randomizationHandler.cpp` and added the missing Catch2 `catch_matchers.hpp` include to `random.test.cpp`.
- Updated move, type, registry, Pokemon, and configuration tests and includes for the trigger and `BuiltinTypeID` API changes.

### Removed

- Removed `MoveTriggerID::OnTarget`.

## [0.9.7] - 2026-08-03

### Added

- Added zero-initialized `ub` fields `EffectContext::mTotalHits` and `mCurrentHit` for multi-hit tracking.
- Added `MoveMeta::mHitDistribution` as `std::vector<std::pair<ub, float>>`, initialized to `{{1, 1.0F}}`.
- Added the floating-point `Random::get<T>(min, max)` overload, implemented with `std::uniform_real_distribution<T>` over `[min, max)`.
- Added tests that bound a `double` draw to `[0.45, 0.63)` and verify deterministic floating-point output after reseeding the shared generator.

### Changed

- Value-initialized `MoveMeta::mTriggers` with `{}`.

## [0.9.6] - 2026-08-03

### Added

- Added `ub` randomization bounds `RANDOMIZATION_MULTIPLIER_MIN_VALUE{85}` and `RANDOMIZATION_MULTIPLIER_MAX_VALUE{100}`.
- Added the `Randomization` built-in multiplier ID, display name, and registry metadata.
- Added `RandomizationHandler`, which calls `Random::get<ub>(85, 100)`, divides the inclusive integer result by `100`, and stores the resulting `0.85` through `1.0` Randomization multiplier.

### Changed

- Renamed `CRITICAL_HIT_MULTIPLIER`, `STAB_HIT_MULTIPLIER`, and `TARGETS_HIT_MULTIPLIER` with the `_VALUE` suffix and updated handlers and tests.

### Removed

- Removed the unused `Core/attributeMacros.h` includes and `ATTR_MAYBE_UNUSED` provider annotation from the type-effectiveness handler.

## [0.9.5] - 2026-08-03

### Added

- Added the `Guts` built-in ability ID and display name and the `Facade` built-in move ID.
- Added the `Burn` built-in multiplier ID, display name, registry metadata, and `BURN_MULTIPLIER_VALUE{0.5}`.
- Added `BurnDamageHandler`. It returns for a missing user Pokemon and applies the `0.5` Burn multiplier only when the user is burned, the move is not special, the user does not have Guts, and the move is not Facade.

## [0.9.4] - 2026-08-03

### Added

- Added `NOT_VERY_EFFECTIVE_VALUE{0.5}`, `EFFECTIVE_VALUE{1.0}`, `SUPER_EFFECTIVE_VALUE{2.0}`, and `NO_EFFECTIVE_VALUE{0.0}`.
- Added `getEffectivenessValue(TypeEffectiveness)`: `NVE` maps to `0.5`, `E` to `1.0`, `SE` to `2.0`, and both `NE` and `NOT_DEFINED` to `0.0`.

### Changed

- Changed the base, critical-hit, STAB, targets, and fixed-point multiplier constants from `float` to `double`.
- Changed `EffectContext::setMultiplier()` to accept `double`, changed active multiplier storage to `std::vector<std::pair<MultiplierID, double>>`, and changed `getActiveMultipliers()` to return the corresponding `std::span`.
- Changed `TypeEffectivenessHandler` to accumulate a `double` and multiply each defined target-type matchup by `getEffectivenessValue()` before storing the result.
- Updated multiplier tests and literals for the `double` interfaces.

### Fixed

- Added missing `Types/builtInTypeID.h` includes to the type-registry configuration source and its configuration and registry tests.

## [0.9.3] - 2026-08-03

### Changed

- Restored the `TypeEffectiveness` enum and `Types/typeEffectiveness.h`, with `NOT_DEFINED`, `NE`, `NVE`, `E`, and `SE` values.
- Changed type-chart storage, built-in matchup constants, type-registry configuration APIs, and tests from `TypeEffectivenessID` values back to the enum.
- Changed `TypeEffectivenessHandler` to read enum chart cells and skip `NOT_DEFINED`; numeric registry lookup and multiplication were removed, leaving the multiplier at its `1.0F` initial value until numeric enum conversion was added in 0.9.4.

### Removed

- Removed `TypeEffectivenessRegistry`, `TypeEffectivenessRegistryConfiguration`, their source files, and the `RegistryProvider::typeEffectivenessRegistry` pointer.
- Removed `TypeEffectivenessID`, `BuiltInTypeEffectivenessID`, `TypeEffectivenessMeta`, `NO_TYPE_EFFECTIVENESS_ID`, and `TYPE_EFFECTIVENESS_MAX_AMOUNT`.
- Removed type-effectiveness provider initialization from handler tests and updated type-registry tests for enum-valued matchups.

## [0.9.2] - 2026-08-03

### Added

- Added the open `TypeEffectivenessID`, its `NO_TYPE_EFFECTIVENESS_ID` sentinel, and built-in IDs `NE=0`, `NVE=1`, `E=2`, and `SE=3`.
- Added `TypeEffectivenessMeta` with a non-owning name, `float` multiplier value, and stable ID.
- Added the fixed-capacity `TypeEffectivenessRegistry` and `TYPE_EFFECTIVENESS_MAX_AMOUNT{MAX_TYPES_PER_POKEMON * 2}`, which is four. Construction fills all four slots; `NE` explicitly stores `0.0F`, while `NVE`, `E`, and `SE` retain the metadata field's `0.0F` default.
- Added exact display names `Not Effective`, `Not Very Effective`, `Effective`, and `Super Effective`, plus `DuplicateTypeEffectiveness` and `TypeEffectivenessNotFound` registry errors and their string mappings.
- Added `TypeEffectivenessRegistryConfiguration` lookup, containment, add, batch-add, rename, update, and remove operations. The exact plural APIs are `getRegisteredTypeEffectivenesss()` and `addTypeEffectivenesss()`.
- Added the type-effectiveness registry pointer to `RegistryProvider` and initialized it in handler tests.
- Added the `TypeEffectiveness` built-in multiplier ID, display name, and multiplier-registry metadata.
- Added `TypeEffectivenessHandler`. For valid Pokemon, a damaging move, and available type and effectiveness registries, it multiplies registered effectiveness metadata across defined target types and stores the result as the Type Effectiveness multiplier.

### Changed

- Renamed built-in ID headers for abilities, items, moves, multipliers, statuses, terrains, and weather from `builtin...` to `builtIn...`, updating all includes.
- Renamed `Types/types.h` to `Types/builtInTypeID.h`, replaced `Types/typeEffectiveness.h` with `Types/builtInTypeEffectivenessID.h`, and updated their consumers.
- Moved `toTypeID()` from `typeID.h` to `builtInTypeID.h`, changed the `typeID.h` guard, and renamed its detail tag from `TypeIDTag` to `typeID`.
- Changed type-chart rows, configuration operations, built-in matchup constants, and tests from `TypeEffectiveness` enum values to stable `TypeEffectivenessID` values. Unspecified neutral matchups use the built-in `E` ID; not-defined matchups use `NO_TYPE_EFFECTIVENESS_ID`.

### Removed

- Removed `ccache` from the Makefile compiler command, dependency installation script, and CodeQL and testing workflows.
- Removed obsolete attribute and type-header includes from `typeID.h` after moving `toTypeID()`.

## [0.9.1] - 2026-08-03

### Added

- Documented `FixedMetadataRegistry` ID-index internals, lookup and maintenance helpers, fixed storage, registered/indexed counts, and next-ID state.
- Documented `RegistryProvider` as a bundle of non-owning registry pointers, including lifetime and null-pointer requirements.
- Documented the `Types` enum, stable-value role, and underlying `ub` storage.

### Changed

- Modernized file-level Doxygen syntax and descriptions in exactly seven headers: `Core/attributeMacros.h`, `Core/configCat.h`, `Registry/fixedMetadataRegistry.h`, `Registry/registryProvider.h`, `Types/types.h`, `Utility/Containers/ContiguousSequence/contiguousSequence.h`, and `Utility/OverflowProtection/overflowProtection.h`.
- Made no runtime behavior changes.

## [0.9.0] - 2026-08-03 (Type Registry with Fixed Metadata Registry Update)

### Added

- Added an optional `NameMember` pointer-to-member template parameter to `FixedMetadataRegistry`, defaulting to `&Metadata::mName`; name lookup and comparison now operate through that member pointer so metadata records with an explicitly selected name field can use the generic registry.
- Added protected mutable and const `getRegistry()` accessors to `FixedMetadataRegistryConfiguration`, giving derived domain configurations a single path to the registry owned by the generic configuration base.
- Added mutable and const indexed-entry span helpers in `FixedMetadataRegistry`; ID-index insertion, removal, rebuilding, and lookup reuse these bounded views instead of reconstructing the active range at each access.

### Changed

- Changed `TypeEntry` fields from `name` and `typeID` to `mName` and `mTypeID`, and updated registry, configuration, and test call sites to the prefixed layout.
- Changed `TypeRegistry` to privately specialize `FixedMetadataRegistry<TypeEntry, TypeID, MAX_TYPES, &TypeEntry::mTypeID, &TypeEntry::mName>`:
  - Built-in type metadata is registered through the generic base while the type-effectiveness chart remains type-domain storage.
  - The type registry selectively re-exports generic lookup, registered-span, containment, counter, entry mutation, erasure, and ID-management operations through `using Base::...` declarations.
  - Type-specific methods continue to expose type names, IDs, metadata, and matchup rows without exposing the base-class relationship publicly.
- Changed `TypeRegistryConfiguration` to privately inherit the generic fixed-metadata configuration specialization and delegate ordinary add, batch-add, rename, update, remove, lookup, containment, and registered-entry operations to that base; matchup-row and defensive-column operations remain type-specific.
- Changed type configuration internals, including row clearing and source definitions, to obtain the registry through `getRegistry()` rather than a duplicate direct member.
- Changed `FixedMetadataRegistry::getName()` and name-based search to dereference `NameMember` instead of assuming a field named `mName`.
- Applied the exact optimization annotations introduced by this patch:
  - `ATTR_PURE` on `statStageCacheIndex()`, fixed-registry `findIndexByID()` and `findEntryIndexByID()`, and `EffectContext::getActiveMultipliers()`.
  - `ATTR_NOINLINE` on fixed-registry `eraseEntry()` and name search, plus the `TypeRegistry` constructor and its span-based built-in registration helper.
- Updated registry configuration namespace aliases, formatting, include ordering, LCOV/clang-tidy suppressions, and affected tests to match the delegated APIs and renamed type fields; no concurrency guarantee was added.

### Removed

- Removed `TypeRegistryConfiguration`'s direct `TypeRegistry` member; the inherited fixed-metadata configuration now owns the registry used by both generic and type-chart operations.
- Removed `TypeRegistry`'s duplicate metadata array, registration count, next-ID counter, and hand-written generic name/ID lookup implementations in favor of the fixed-metadata base implementation.

## [0.8.7] - 2026-08-03

### Added

- Added `BENCHMARK_WARNINGS`, derived from the normal warning set with `-Winline` and `-Wsuggest-attribute=pure` filtered out, and excluded `main.cpp` from benchmark source discovery.
- Added three benchmark translation units:
  - `EffectContext` benchmarks for setting every built-in multiplier, applying the active multiplier sequence, and updating built-in multipliers.
  - `MoveMeta` benchmarks for constructing the predefined effect-list variants.
  - A fixed-metadata registry ID-lookup benchmark registered with `DenseRange` to measure indexed lookup across registry occupancy.
- Added `statStageCacheIndex()`, which clamps signed stages to `[-6, 6]` and translates them to `[0, 12]`; stat, accuracy, and evasion cache users now share the same signed-stage-to-array-index rule.
- Added `FixedMetadataRegistry::IDIndexEntry`, a fixed-capacity sorted array of `{stable ID, entry index}` records, and an indexed-entry count:
  - Stable-ID lookup uses `std::lower_bound`, making successful and unsuccessful ID searches logarithmic in the registered entry count.
  - Insertion preserves stable-ID then entry-index ordering, removal compacts the index, and rebuilding regenerates mappings after entry replacement or count restoration.
- Added `eraseEntry()` to the generic registry and re-exported it from the ability, item, move, multiplier, status, terrain, and weather registries; erasure compacts metadata and rebuilds the stable-ID index.
- Added a one-based built-in-ID-to-vector-position array to `EffectContext`:
  - First insertion records the sparse-vector position for a built-in multiplier.
  - Later updates of that built-in use direct indexed access, while custom multiplier IDs retain linear search and append behavior.
  - `getActiveMultipliers()` exposes the active sparse vector as a read-only span, and reset clears both the vector and position map.
- Added shared `IEffectHandler` helpers for resolving the user and target `BattleSlot` from `BattleState` and `EffectContext`.
- Added 15 focused Catch2 suite files covering five registry-configuration domains, `EffectContext`, the handler interface and five concrete handlers, move-effect metadata, and the input and random utilities; expanded the existing ability, item, and type configuration suites plus registry, type-chart, contiguous-sequence, and logger coverage.

### Changed

- Changed all built-in move effect-list factories to return `std::span<const EffectTypeID>` over static `constexpr` arrays; the query functions are `ATTR_CONST` and `noexcept`, avoiding per-call vector construction for the predefined sequences.
- Changed fixed-registry life cycle hooks so built-in registration and amount increments insert index records, decrements remove them, and `setEntry()`/`setAmountRegistered()` rebuild the index when metadata positions can change.
- Changed fixed-registry configuration removal to delegate compaction and index maintenance to `eraseEntry()`.
- Changed accuracy, base-damage, and STAB handlers to use the shared user/target slot helpers instead of repeating side-vector selection and index checks.
- Changed base-damage intermediates and validation to `double`; invalid non-finite or negative attack values and non-finite or non-positive defense values leave the existing damage unchanged, while valid results are clamped to `[1, std::numeric_limits<us>::max()]` before conversion.
- Changed contiguous-sequence utilities to reject invalid start/length inputs through explicit early returns before arithmetic or iterator traversal.
- Changed move-registry built-in effect copying to use span iterators, and applied associated include, formatting, documentation, LCOV, and test-maintenance updates.

### Fixed

- Fixed `addMetadataBatch()` rollback without copying the entire registry: it records the prior registered count and next stable ID, erases each partially appended entry after a failure, restores the next-ID counter, and returns the original error.
- Fixed both name- and ID-selected metadata mutation paths to reject a changed name that already belongs to another entry before committing the replacement.
- Fixed type matchup row and defensive-column updates so a failed partial update restores the previous chart state and returns the failure instead of leaving partial matchup changes.
- Fixed fixed-registry lookup boundary assertions so the registered-count sentinel returned for "not found" is valid, including empty and full-capacity searches.
- Fixed critical-hit probability bounds by changing the random maximum from 100 to 99 and the threshold comparison from `<=` to `<`, yielding a 100-value roll domain with the configured threshold count.
- Fixed `EffectContext::applyMultiplier()` to half-down round `damage * fixedPointValue` for each active multiplier rather than rounding the multiplier before multiplication; every step remains clamped to at least one.
- Fixed base-damage overflow and invalid arithmetic by retaining floating-point intermediates until the final bounded conversion.
- Fixed the constrained input template's element-type expression by removing the invalid redundant `T::value_type` qualification.
- Fixed signed, overflow-prone, omitted-length, and out-of-range paths in contiguous-sequence calculations by validating bounds before computing the requested interval.

## [0.8.6] - 2026-07-31

### Removed

- Removed `DamageContext::mIsProtected`.
- Removed the complete `EffectResult` structure and `EffectContext::mResult`; status application, stat-change counters, and sleep-turn output are no longer stored in this context result object.

## [0.8.5] - 2026-07-31

### Added

- Added `DamageFormulaModifiers` to each `BattleSlot`, with independent health, attack, defense, special-attack, special-defense, and speed `float` fields initialized to `1.0F`.
- Added damage-formula integration for attack and defense modifiers:
  - Physical damage multiplies the user's Attack and target's Defense after applying their cached stat-stage multipliers.
  - Special damage analogously applies the user's Special Attack and target's Special Defense modifiers.
  - Health and Speed modifiers are stored for later consumers but are not read by `BaseDamageHandler` in this revision.

### Changed

- Changed `EffectContext::mActiveMultipliers` to use an explicit empty default member initializer.

## [0.8.4] - 2026-07-31

### Changed

- Moved every `Pokemon` setter into the class definition and made it `constexpr`, including name, move/PP arrays and indexed elements, type arrays and indexed elements, all six base stats, level and cached level-damage factor, ability, item, and status.
- Preserved index assertions for move, PP, and type slot setters while making those mutation paths available during constant evaluation.
- Updated Pokemon unit construction to provide the level argument introduced in 0.8.3 and verify both default and explicit type-ID construction paths.

### Removed

- Removed the now-empty `pokemon.cpp` implementation unit after all setter definitions moved to `pokemon.h`.

## [0.8.3] - 2026-07-31

### Added

- Added fixed-point multiplier constants with numerator and denominator `4096.0F`, a half-down threshold of `0.5`, and a `1e-9` tolerance.
- Added `EffectContext::applyMultiplier(baseDamage)`, which visits active multipliers in sparse-vector order, converts each configured float through the 4096/4096 scale, applies half-down rounding, and clamps each intermediate result to at least one. In this initial implementation the scaled multiplier itself is rounded before multiplication; 0.8.7 corrects the operation to round `damage * fixedPointValue`.
- Added `BuiltinMultiplierID::Weather`, its display-name constant, and the corresponding built-in `MultiplierRegistry` entry.
- Added a required level argument to both `Pokemon` constructors; each constructor calls `setLevel()` so `mLevel` and the cached level-damage factor are initialized together.

### Changed

- Changed `setMultiplier()` to replace an existing sparse-vector value or append a new `{MultiplierID, value}` pair without maintaining a product; `resetMultipliers()` now clears only the sparse vector.

### Removed

- Removed `EffectContext::mCombinedMultiplier` and the divide/multiply bookkeeping previously performed when a multiplier was replaced.

## [0.8.2] - 2026-07-30

### Added

- Added compile-time stat-stage tables for the full `-6..+6` range:
  - General stats use `(2 + stage) / 2` for nonnegative stages and `2 / (2 - stage)` for negative stages.
  - Accuracy uses the analogous 3/3 formulas; evasion uses `3 / (3 + stage)` for nonnegative stages and `(3 - stage) / 3` for negative stages.
  - `MAX_STAT_STAGES` is 6 and `MAX_STAGE_CACHE` is 13; the table generators store stage `s` at offset `s + 6`.
- Added level-damage-factor constants `2`, `5`, and `2`; `Pokemon::setLevel()` stores `floor((2 * level) / 5) + 2` in `mLevelDamageFactor`, exposed by a new getter.
- Added `RegistryProvider`, a non-owning aggregate of const pointers to the ability, move, item, type, status, weather, terrain, and multiplier registries.
- Added `RegistryProvider` to the pure-virtual `IEffectHandler::apply()` contract and to the AccuracyCheck, BaseDamage, CriticalHit, STAB, and Targets implementations.
- Added `EffectContext::setMultiplier()` and `resetMultipliers()` for the new sparse representation; replacement divides out the old contribution and multiplies in the new value, append multiplies in the new contribution, and reset restores the combined value to `1.0F`.
- Added `MoveTriggerID::OnTarget` and assigned both Pound and Karate Chop's base-attack sequence to that trigger.

### Changed

- Changed `EffectContext` multiplier storage from a prepopulated `std::unordered_map` to an initially empty `std::vector<std::pair<MultiplierID, float>>` plus a cached combined product; this revision still performs linear ID searches.
- Changed accuracy and base-damage handlers to consume the precomputed stage tables, and changed base damage to use the Pokemon's cached level factor rather than recomputing it per application.
- Changed CriticalHit, STAB, and Targets handlers to call `setMultiplier()` rather than mutate multiplier storage directly.
- Changed `TargetsHandler` to resolve move metadata through `RegistryProvider::moveRegistry`; the handler no longer owns a `MoveRegistryConfiguration`, and an unknown move ID now returns without adding a multiplier.
- Changed `BattleSlot::mPokemon` from a pointer-to-const to a mutable `Pokemon *` and updated the Pound registry test to expect `OnTarget`.

### Removed

- Removed `BattleSlot::mCurrentHp`.
- Removed the `EffectContext` constructor that created a multiplier configuration and prepopulated every built-in multiplier at `1.0F`.
- Removed `TargetsHandler`'s owned `MoveRegistryConfiguration` member.

## [0.8.1] - 2026-07-30

### Added

- Added the open tagged `MultiplierID` domain, `std::hash` support for tagged IDs, `MultiplierMeta`, display-name constants, the closed built-in catalog, and conversion to stable IDs.
- Added a fixed-capacity multiplier registry and configuration facade with `DuplicateMultiplier`/`MultiplierNotFound` errors and add, atomic batch-add, rename, update, remove, lookup, registered-span, containment, count, and next-ID operations.
- Registered six initial built-in multipliers: None, Ability, Item, Targets, Critical, and Stab. Configuration values are `1.0F` for the base multiplier, `1.5F` for critical hits, `1.5F` for STAB, and `0.75F` for spread targets.
- Added an `EffectContext` constructor that enumerates the multiplier registry and initializes an `std::unordered_map<MultiplierID, float>` entry at the base value for every built-in multiplier.
- Added `EffectTypeID::Targets` and `EffectTypeID::Stab`, and inserted both into the base attack pipeline, increasing Pound's expected effect count from 9 to 11.
- Added `StabHandler`, which assigns the STAB multiplier when the move type is present in the user's type array.
- Added `TargetsHandler`, initially backed by an owned `MoveRegistryConfiguration`, which assigns the spread-target multiplier unless move metadata targets a single opponent or self.
- Added `Pokemon` status storage with getter/setter and `isFainted()`, which derives fainting from zero health.
- Added the previously declared terrain and weather registry-configuration source implementations for add/batch, rename, update, and remove operations by name or stable ID.
- Added `MultiplierRegistryConfiguration` construction to the custom-configuration path in `main()`.

### Changed

- Changed `CriticalHitHandler` to store `1.5F` in the context's Critical multiplier entry rather than a dedicated critical-multiplier field.
- Changed battle ownership so `BattleSlot::mPokemon` is mutable; item, status, and fainted state are now read from the pointed-to `Pokemon` rather than duplicated in the slot.
- Reordered effect IDs and the base attack sequence to include Targets and STAB, and corrected the status constants header description.

### Removed

- Removed `BattleSlot::mItemID`, `mStatusID`, and `mIsFainted`, along with the dedicated `EffectContext::mCriticalMultiplier` field.

## [0.8.0] - 2026-07-30 (Weather and Terrain Registry Update)

### Added

- Added tagged open `WeatherID` and `TerrainID` types with no-value sentinels, closed built-in catalogs, stable-ID converters, display-name constants, and name/ID metadata records.
- Added `MAX_WEATHERS` and `MAX_TERRAINS`, each set to 1,000, plus duplicate/not-found registry error kinds and string conversion support.
- Added header-defined fixed-capacity runtime registries:
  - Weather registers 11 built-ins: None, Harsh Sunlight, Rain, Sandstorm, Hail, Snow, Fog, Extremely Harsh Sunlight, Heavy Rain, Strong Winds, and Shadowy Aura.
  - Terrain registers five built-ins: None, Electric, Grass, Misty, and Psychic.
  - Both expose domain metadata/name/ID lookup, registered spans, containment by name or ID, index lookup, registration counts, and next-ID mutation through the fixed-registry base.
- Added weather and terrain configuration facade declarations for one-at-a-time and atomic batch registration, rename, full metadata update, and removal by name or stable ID. Their out-of-line definitions are not part of this commit and arrive in 0.8.1.
- Added terrain and weather configuration construction to the custom-configuration path in `main()`.

### Changed

- Changed `BattleState` from individual weather/terrain boolean flags to stable `WeatherID` and `TerrainID` fields, preserving environmental state as registry identities.

### Removed

- Removed the legacy `Weathers` enum header after weather identity moved to the tagged ID and registry model.

## [0.7.7] - 2026-07-29

### Changed

- Integrated the parallel status-registry first-parent line with the branch containing the generic fixed-registry Catch2 suite, ccache build setup, signed stat stages, Pokemon level storage, the effect-handler interface, base-damage and critical-hit handlers, accuracy handling, CI fallback installation, Pound effect-count correction, and `ATTR_NOINLINE` on `setEntry()`.
- Preserved the status registry's open ID domain, seven built-ins, configuration facade, and `BattleState`/`EffectResult` integration while bringing the battle-handler and build/test work into one history line.
- This merge adds no standalone implementation or conflict-resolution behavior; the shared configuration constants file simply reflects the integrated second-parent values.

## [0.7.6] - 2026-07-29

### Changed

- Applied `ATTR_NOINLINE` to `FixedMetadataRegistry::setEntry()` to prevent compiler inlining; the method's assertion and metadata assignment behavior are otherwise unchanged.

## [0.7.5] - 2026-07-29

### Added

- Added accuracy constants with a 3/3 stage basis and an inclusive random roll range of 1 through 100.
- Added `AccuracyCheckHandler::apply(const BattleState &, EffectContext &)`, which:
  - Resolves user and target slots from their sides and indices.
  - Computes `move accuracy * user accuracy-stage multiplier * target evasion-stage multiplier` and clamps the result to `[0, 100]`.
  - Marks `mIsMiss`, clears `mShouldApplyDamage`, and clears `mShouldContinue` when the 1-100 roll is greater than the clamped accuracy; a hit leaves those fields unchanged.

### Changed

- Renamed `EffectContext::moveAccuracy` to `mMoveAccuracy` and added the type/context includes required by the damage and critical-handler sources.

### Fixed

- Fixed `BaseDamageHandler` to derive the target defense multiplier from `targetDefenseStage`; the initial implementation incorrectly reused `userAttackStage` for both sides of the attack/defense ratio.

## [0.7.4] - 2026-07-29

### Changed

- Changed the cache-hit fallback package installation in both CodeQL and test workflows to install `ccache` alongside Catch2 and Google Mock, ensuring the `ccache g++` compiler command remains available when dependency caches are restored.

## [0.7.3] - 2026-07-29

### Fixed

- Corrected the test-only Pound metadata expectation from 10 effects to 9 after `EffectTypeID::StatStage` was removed from the base attack sequence in 0.7.2; production move metadata is unchanged by this commit.

## [0.7.2] - 2026-07-29

### Added

- Added `IEffectHandler`, a non-copyable/non-movable polymorphic interface with pure virtual `apply(const BattleState &, EffectContext &) const`, and added mutable `getTeam()` plus read-only `getTeamConst()` side-selection helpers.
- Added `BaseDamageHandler`:
  - Returns without modifying damage when either Pokemon pointer is null or move power is zero.
  - Selects Attack/Defense or Special Attack/Special Defense, applies signed stat-stage multipliers with a 2/2 basis, and on a critical hit ignores negative user attack stages and positive target defense stages.
  - Computes `floor((floor(2 * level / 5) + 2) * round(power * attack / defense) / 50) + 2`, then stores at least one damage. At this revision the target multiplier is mistakenly calculated from the user attack stage; 0.7.5 corrects it.
- Added `CriticalHitHandler` with constants 0 and 100 for the inclusive random range, threshold 5, and multiplier `1.5F`; a roll `<= 5` sets `mIsCritical` and changes the context's initially `1.0F` critical multiplier to `1.5F`.
- Added `BattleSlot::mPosition`, `EffectContext::mRangeOverride`, and Pokemon level storage with getter/setter.

### Changed

- Changed all `StatStages` fields from `us` to signed byte `sb`, allowing negative Attack, Defense, Special Attack, Special Defense, Speed, Accuracy, and Evasion stages.
- Changed `EffectResult::mStatusToApply` to value-initialize the new open `StatusID` and moved it ahead of the byte counters.
- Reordered CriticalHit and BaseDamage in both `EffectTypeID` and the base attack sequence so critical state is available before damage calculation.

### Removed

- Removed `EffectTypeID::StatStage` and its base attack pipeline element, reducing that sequence from 10 effects to 9.

## [0.7.1] - 2026-07-29

### Added

- Added `ccache` to the dependency installation package list.

### Changed

- Changed the Makefile compiler command from `g++` to `ccache g++`; no production source or runtime behavior changed.

## [0.7.0] - 2026-07-29 (Status Registry Update)

### Added

- Added the status registry implementation independently on the battle-handler branch:
  - `MAX_STATUSES` is 1,000, with `DuplicateStatus` and `StatusNotFound` errors integrated into registry error reporting.
  - `StatusID` is an open tagged `IDInterface` identity with `NO_STATUS_ID`; `BuiltinStatusID`, `toStatusID()`, display-name constants, and `StatusMeta` define the closed built-in catalog and stored record.
  - `StatusRegistry` registers None, Paralysis, Burn, Sleep, Freeze, Poison, and Toxic, and exposes status metadata/name/ID lookup, registered spans, containment, index lookup, registration-count mutation, and next-ID control through the fixed-registry base.
  - `StatusRegistryConfiguration` supports one-at-a-time and atomic batch registration plus rename, full-record update, and removal by name or stable ID.
- Added status configuration construction to the custom-configuration path in `main()`.
- Added a Catch2 test registry over `FixedMetadataRegistry`, covering built-in stable IDs, name/metadata lookup, absent names and IDs, registered spans, containment, amount restoration, next-ID restoration, and next-ID incrementing.

### Changed

- Changed `EffectResult::mStatusToApply` from `StatusID::None` to value initialization of the open `StatusID` and moved it before the byte result counters; affected aggregate layout follows the new identity type.
- Corrected the `StatusID` documentation to describe user-defined statuses rather than user-defined abilities.

## [0.6.4] - 2026-07-29

### Added

- Added the initial status registry on the parallel status branch:
  - `MAX_STATUSES` is 1,000, and registry error reporting recognizes `DuplicateStatus` and `StatusNotFound`.
  - The former closed `StatusID` enum becomes an open tagged `IDInterface` identity with `NO_STATUS_ID`; a separate `BuiltinStatusID` catalog, `toStatusID()` converter, seven display-name constants, and `StatusMeta` retain the built-in vocabulary.
  - `StatusRegistry` pre-registers None, Paralysis, Burn, Sleep, Freeze, Poison, and Toxic, then exposes metadata/name/ID lookup, registered spans, containment by name or ID, index lookup, count mutation, entry replacement, and next-ID operations.
  - `StatusRegistryConfiguration` provides one-at-a-time and atomic batch addition, rename, complete metadata update, and removal by name or stable ID through the shared fixed-registry configuration.
- Added `StatusRegistryConfiguration` construction to `main()`'s custom-configuration path.
- Added a generated Zed `compile_commands.json` compilation database containing the then-current compiler invocations and flags.

### Changed

- Changed `BattleSlot` and `EffectResult` member ordering and corresponding aggregate initialization to accommodate the open status identity and revised layout; no status-registry tests are introduced in this branch commit.

## [0.6.3] - 2026-07-28

### Added

- Added a test-only `MoveRegistry` branch that verifies the shared fixed-registry mutators through the move-domain wrapper:
  - `incrementAmountRegistered()` changes the count from 3 to 4 and `decrementAmountRegistered()` restores it to 3.
  - `getEntry(0)` returns the None move metadata.
  - Copying that entry, changing its name to `CustomMove`, and calling `setEntry(0, replacement)` makes the new name observable through `getEntry(0)`.

### Changed

- Expanded the existing MoveRegistry scenario from read-only lookup/counter coverage to explicit amount and entry mutation assertions; no production code changed.

## [0.6.2] - 2026-07-28

### Added

- Added LCOV constructor exclusions around built-in registration in the ability, item, and move registries; these are coverage annotations only and do not alter constructor execution.
- Added test assertions across the three registries for direct registered-count restoration, direct next-stable-ID restoration, and next-ID increment behavior.
- Added containment assertions by stable ID and centralized name, and strengthened registered-span checks with exact built-in size plus first/last metadata names.

### Changed

- Changed ability, item, and move tests from hardcoded display-name literals to their domain constants where available.
- Renamed the misleading Pound test from a Chesto Berry status-removal description to "Pound retains its psychic terrain priority block metadata," and changed generic span wording to state the exact built-in-span contract.
- Changed local metadata copies in ability tests from `const` to mutable test values; no production behavior changed.

## [0.6.1] - 2026-07-28

### Fixed

- Fixed only the Pound built-in metadata assertion in `moveRegistry.test.cpp`: the first effect remains `PsychicTerrainPriorityBlock`, not `StatusRemove`. No production metadata or runtime behavior changed.

## [0.6.0] - 2026-07-28 (Move Registry Update)

### Added

- Added _MoveRegistryConfiguration_ as the move-domain facade over _FixedMetadataRegistryConfiguration_, preserving the fixed-capacity registry model while exposing move-specific operations:
  - Read metadata by _MoveID_, resolve IDs from names and names from IDs, inspect the registered metadata span and count, and test containment by name or ID
  - Register one _MoveMeta_ or an atomic batch with monotonically assigned stable IDs and snapshot rollback when validation fails
  - Replace trigger vectors and target IDs by name or ID, update an entire metadata record, rename entries, and remove entries with compacted storage
  - Report policy-specific _DuplicateMove_ and _MoveNotFound_ errors through the shared registry error interface
- Added ability and item configuration operations to set target IDs and replace complete metadata records by either user-readable name or stable ID.
- Added _MoveRangeID_ with _Unrestricted_ and _Adjacent_ values so target selection and legal reach are represented independently.
- Added move registry unit coverage for built-in registration, ID/name lookup, metadata, absent lookups, registered spans, and containment; marked defensive fixed-registry branches as LCOV exclusions.
- Added _MoveRegistryConfiguration_ construction to the custom-configuration path in _main.cpp_.

### Changed

- Changed ability, item, and move registration APIs to accept their domain metadata records directly; _AbilityMeta_ and _ItemMeta_ now serve as both registration input and stored representation.
- Renamed _MoveMeta::mMoveTargetID_ to _mTargetID_ and added _mRangeID_ to distinguish the selected battler set from positional range constraints.
- Completed built-in move records:
  - _None_ retains default metadata
  - _Pound_ is Normal-type, 40 power, physical, 100 accuracy, priority 0, targeting _SingleOpponent_ at _Adjacent_ range
  - _Karate Chop_ is Fighting-type, 50 power, physical, 100 accuracy, priority 0, targeting _SingleOpponent_ at _Adjacent_ range
- Changed _Drizzle_, _Cheri Berry_, and _Chesto Berry_ metadata to target _Self_.
- Changed the _Stench_ trigger from _OnUse_ to _OnDamageCalc_ so its effect participates in damage calculation rather than move invocation.
- Revised registry documentation and normalized formatting and static-analysis conformance in the touched effect, item-target, move-registry, and metadata sources.

### Removed

- Removed the separate _AbilityDefinition_ and _ItemDefinition_ registration structures and their conversion paths in favor of direct metadata registration.

## [0.5.3] - 2026-07-27

### Added

- Added _MAX_MOVES_ with a fixed registry capacity of 1000 entries.
- Added combat fields to _MoveMeta_: _TypeID_, unsigned-short power, move target, accuracy, priority, and the physical/special classification flag.
- Added _MoveRegistry_ as a private specialization of _FixedMetadataRegistry_ for _MoveMeta_, _MoveID_, and _MAX_MOVES_:
  - Registers _None_, _Pound_, and _Karate Chop_ as built-in entries
  - Exposes move-domain metadata, ID, name, span, count, index, and containment queries while retaining controlled base mutators for configuration use
- Added reusable move-effect pipeline builders:
  - Base attacks execute _PsychicTerrainPriorityBlock_, _AccuracyCheck_, _BaseDamage_, _CriticalHit_, _StatStage_, _BurnDamageReduction_, _Weather_, _Terrain_, _TypeEffectiveness_, and _Randomization_ in order
  - Dedicated builders append recoil, status application, or flinch behavior to the base attack pipeline
  - Status, protect, and field-effect builders provide non-damaging trigger sequences without duplicating effect lists
- Added _PsychicTerrainPriorityBlock_ to the effect type catalog.
- Added target fields to _AbilityMeta_ and _ItemMeta_; introduced the item _OnMoveUseTarget_ trigger and _SingleOpponent_ target.
- Added _BuiltinMoveID::None_ as the explicit built-in catalog entry for the null move.

### Changed

- Changed _setUpGCC()_ to link the system _libstdc++.so.6_ directly to the selected compiler installation's _libstdc++.so.6_, avoiding dependence on a specific GCC patch-level filename.

### Removed

- Removed hardcoded handling for _libstdc++.so.6.0.32_ and _libstdc++.so.6.0.35_ from the dependency setup script.

## [0.5.2] - 2026-07-27

### Added

- Added the open _MoveID_ type as an _IDInterface_ specialization with a move-specific tag and default value 0, plus the _NO_MOVE_ID_ sentinel.
- Added the closed _BuiltinMoveID_ catalog for _Pound_ and _Karate Chop_, a constexpr conversion to stable _MoveID_, and canonical names for _None_, _Pound_, and _Karate Chop_.
- Added the initial _MoveMeta_ record containing owned trigger definitions, a non-owning name, and a stable ID.
- Added _MoveTriggerID_ values _OnUse_ and _OnHazardSwitchIn_, and _MoveTargetID_ values _SingleOpponent_, _AllOpponents_, _AllAllies_, _Self_, and _AllExceptSelf_.
- Added _MoveEffectTrigger_ to associate one move trigger with its ordered effect vector.

### Changed

- Replaced the closed _MoveID_ enum with the tagged, registry-assigned identifier so user-defined moves no longer require extending a compile-time enum.
- Changed _Pokemon_ move-array initialization from the former _MoveID::None_ enumerator to the fully qualified _PocketCore::Move::NO_MOVE_ID_ sentinel and normalized the touched header formatting.

## [0.5.1] - 2026-07-27

### Changed

- Made access contracts explicit with public sections on registry policy and metadata structures, including _AbilityRegistryConfigurationPolicy_, _ItemRegistryConfigurationPolicy_, _AbilityMeta_, _ItemMeta_, _MatchupPair_, _TypeDefinition_, and _TypeEntry_.
- Reordered members in _AbilityMeta_, _ItemMeta_, ability and item trigger records, _BattleSlot_, _EffectContext_, _RegistryErrorInfo_, and _Pokemon_; updated aggregate initializers to preserve behavior under the new layouts.
- Replaced metadata lookup results based on optional reference wrappers with nullable pointers to const metadata in the fixed, ability, and item registries.
- Strengthened type-registry const correctness by returning const entries, cells, rows, and references from read-only accessors.
- Changed _std::string_view_ and _std::span_ parameters to const references across type, ability, item, and generic fixed-registry configuration interfaces to make the API contract uniform.
- Changed protected built-in registration to accept an rvalue metadata record.
- Changed _errorKindToString_ to take an explicit _RegistryError_ and marked it _ATTR_NODISCARD_; applied the same result-use attribute to _toTypeID_.
- Consolidated local type aliases and revised documentation and formatting across the touched registry and configuration headers without changing registry behavior.

## [0.5.0] - 2026-07-27 (Item and FixedMetadata Registry Update)

### Added

- Added _FixedMetadataRegistry_, a generic fixed-capacity array store parameterized by metadata, stable-ID type, and capacity:
  - Tracks registered count and the next monotonic ID, resolves entries by name, ID, or array index, and exposes metadata, name, ID, span, count, index, and containment queries
  - Restricts state mutation to controlled setters and protected built-in registration so domain registries retain registry invariants
- Added _FixedMetadataRegistryConfiguration_, parameterized by a domain policy that supplies configuration/entity names and duplicate/not-found errors:
  - Validates capacity, duplicate names and IDs, and missing entries with contextual _RegistryErrorInfo_ logging
  - Adds one record or an atomic batch with snapshot rollback and monotonic ID assignment
  - Applies callback-based copy-modify-write metadata mutation, rename, removal, and contiguous-array compaction
- Added _ItemRegistryConfiguration_ operations over _ItemDefinition_ input records and trigger spans, including registration, atomic batch registration, trigger replacement, rename, and removal by name or _ItemID_, with _DuplicateItem_ and _ItemNotFound_ policy errors.
- Added focused item-registry and item-configuration tests and integrated ability/item configuration paths into the main executable.

### Changed

- Changed _AbilityRegistry_ and _ItemRegistry_ into private _FixedMetadataRegistry_ specializations, exposing domain APIs while keeping generic storage mutation internal.
- Changed _AbilityRegistryConfiguration_ to delegate validation, lookup, registration, batch rollback, metadata mutation, rename, and removal to the policy-driven fixed configuration base.
- Changed item registry configuration to use the same shared mechanics and updated _Pokemon_ tests to rely on default item-ID initialization.
- Changed registry test index and counter expectations from unsigned byte to unsigned short to match widened stable-ID storage.

### Fixed

- Corrected colliding header guards in the ability and item constants headers so both domains can be included in one translation unit.

### Removed

- Removed duplicated ability/item storage, index resolution, counters, built-in insertion, entry mutation, compaction, and rollback mechanics now owned by the fixed registry layers.

## [0.4.1] - 2026-07-27

### Added

- Added _IDInterface<Tag, DefaultValue>_, providing strong ordering, unsigned-short storage, explicit value construction, _getValue()_, and a configurable default sentinel while preventing cross-domain ID comparison.
- Added the complete item identity and metadata model:
  - Tagged _ItemID_, _NO_ITEM_ID_, _BuiltinItemID_, constexpr conversion, canonical names, _ItemMeta_, and item trigger/target records
  - _None_, _Cheri Berry_, and _Chesto Berry_ built-ins, with berry trigger metadata that removes status conditions
- Added _ItemRegistry_ with capacity 1000, built-in registration, metadata/name/ID/index lookup, containment queries, and controlled entry/count mutation.
- Added _MAX_ABILITIES_PER_POKEMON_, _MAX_ITEMS_PER_POKEMON_, and _MAX_ITEMS_; added _StatusRemove_ to the effect type catalog.
- Added unit coverage for default, explicit, ordered, and domain-separated _IDInterface_ behavior.

### Changed

- Generalized _AbilityID_ and _TypeID_ from hand-written wrappers to tagged _IDInterface_ aliases; introduced _ItemID_ on the same abstraction and marked built-in conversions as result-bearing APIs.
- Widened ability/type registry counters, indexes, and related return values from unsigned byte to unsigned short.
- Increased _MAX_ABILITIES_ from 64 to 1000 and reorganized registry and per-Pokemon limits around _MAX_ABILITIES_PER_POKEMON_, _MAX_ITEMS_PER_POKEMON_, and _MAX_ITEMS_.
- Normalized touched ability configuration formatting and explicit namespace qualification.

### Fixed

- Corrected namespace qualification for registry storage and ability identifiers across the ability registry, configuration implementation, and tests.

### Removed

- Removed the obsolete explicit _Pokemon::mItemID_ initializer because default construction of the tagged _ItemID_ now produces the no-item sentinel.

## [0.4.0] - 2026-07-27 (Ability Registry Update)

### Added

- Added the open, hand-written _AbilityID_ wrapper with a no-ability sentinel, the closed _BuiltinAbilityID_ catalog, and constexpr conversion between built-in and stable identities.
- Added _AbilityRegistry_ with fixed capacity 64 and built-in _None_, _Stench_, and _Drizzle_ records; exposed metadata/name/ID/index lookup, containment, count/next-ID queries, and controlled mutators.
- Added _AbilityRegistryConfiguration_ and _AbilityDefinition_ as the user-facing configuration layer:
  - Registers one ability or an atomic batch with stable monotonic IDs
  - Replaces trigger definitions, renames entries, removes entries with compaction, and reports duplicate/not-found/capacity errors with context
- Added the open, hand-written _TypeID_ wrapper, _NO_TYPE_ID_, and built-in type conversion.
- Added _MAX_TYPES_PER_POKEMON_ at 2 and _MAX_MOVES_PER_POKEMON_ at 4; _Pokemon_ now stores a fixed type-ID array and exposes whole-array and indexed type accessors.
- Added focused ability registry/configuration, Pokemon type-storage, and stable type-ID tests.

### Changed

- Replaced the original closed _AbilityID_ enum with the open stable-ID wrapper, separating extensible runtime identity from the built-in catalog.
- Changed _TypeRegistry_ entries and APIs from raw byte identifiers to _TypeID_, including no-type validation and corresponding configuration/test updates.
- Changed _EffectContext::mMoveTypeID_ from an unsigned byte to _TypeID_.
- Changed _Pokemon_ construction and type access to use fixed arrays of _TypeID_; replaced literal move-array extents with _MAX_MOVES_PER_POKEMON_.
- Revised documentation, formatting, and static-analysis conformance across the touched ID, effect, Pokemon, registry, timer, weather, test, and integration sources.

### Removed

- Removed the switch-based _getAbilityMetadata_ factory; ability metadata now resides in the runtime registry and is resolved through registry lookup.

## [0.3.2] - 2026-07-27

### Added

- Added `*.log` to ignored build artifacts.
- Added the private constexpr _clearRows_ helper to clear both the offensive row and defensive column for a registered type index.
- Added LCOV branch exclusions for defensive paths proven unreachable by preceding registry checks.

### Changed

- Changed both _resetMatchups()_ overloads to delegate chart clearing to _clearRows()_, keeping row/column reset semantics in one implementation.
- Simplified _renameType_ to consume the guaranteed internal index directly after successful ID resolution and normalized the touched implementation formatting.

### Removed

- Removed the unreachable second not-found branch in _renameType_; successful name-to-ID resolution guarantees that the corresponding internal array index exists in the same registry state.

## [0.3.1] - 2026-07-26

### Removed

- Removed the _commit-msg_ hook and its mandatory `[feature]`, `[tweak]`, or `[bugfix]` subject-prefix policy.
- Removed the _pre-commit_ hook, including checks for TODO markers, sensitive keywords, and executable script bits, plus its release build, formatting, Flawfinder, cppcheck, unit-test, and clang-tidy gates.

## [0.3.0] - 2026-07-26 (Pokemon, Battle, and Effect Model)

### Added

- Added enum-backed identifiers for the initial built-in battle data:
  - _AbilityID_ defines None, Stench, and Drizzle
  - _ItemID_ defines None, Cheri Berry, and Chesto Berry
  - _MoveID_ defines None, Pound, and Karate Chop
  - _StatusID_ defines None, Paralysis, Burn, Sleep, Freeze, Poison, and Toxic
- Added the initial ability metadata model:
  - _AbilityTriggerID_ identifies battle start, switch-in, turn-end, damage-calculation, move-use, faint, weather, and status events
  - _AbilityTargetID_ identifies self, all allies, all opponents, and all battlers except self
  - _AbilityEffectTrigger_ associates one trigger with a dynamic list of _EffectTypeID_ values
  - _AbilityMeta_ stores an ability name and trigger list; _getAbilityMetadata()_ maps Drizzle to SetRain on switch-in, Stench to Flinch on move use, and unknown/None IDs to empty metadata
  - Ability name constants provide the display names for None, Drizzle, and Stench
- Added the _Pokemon_ entity with two construction paths:
  - Stores a non-owning name, six unsigned-short combat stats, one ability, one item, and four move slots
  - Tracks maximum and current PP independently for each move slot
  - Provides getters and setters for every stored property, with index assertions on individual move and PP access
  - _usePP()_ decrements current PP only when the selected slot is valid and has PP remaining
- Added foundational battle-state structures:
  - _StatStages_ stores attack, defense, special attack, special defense, speed, accuracy, and evasion stages
  - _BattleSlot_ associates a const Pokemon pointer with current HP, speed multiplier, item/status/choice-lock IDs, sleep/toxic/protection counters, and protected/fainted/flinched/item-consumed flags
  - _BattleState_ stores active slots and parties for both sides, Spikes/Toxic Spikes/Stealth Rock state, battle-start state, Rain/Sun/Sandstorm flags, and Electric/Grassy/Misty/Psychic terrain flags
- Added the effect execution data model:
  - _EffectTypeID_ enumerates damage, critical-hit, stat-stage, burn, weather, terrain, type-effectiveness, randomization, flinch, recoil, status, accuracy, and weather-setting effect categories
  - _Side_ and _EffectSource_ identify participants and whether an effect came from a move, ability, item, or hazard
  - _DamageContext_ carries recoil, accumulated damage, hit/protection/critical state, and pipeline continuation/application controls
  - _EffectResult_ carries attack/defense stage changes, sleep duration, and a status to apply
  - _EffectContext_ combines damage/results with multipliers, user/target indices and sides, move properties, move/ability IDs, source type, and physical/special classification
- Added a _Weathers_ enum for Harsh Sunlight, Rain, Sandstorm, Hail, Snow, Fog, Extremely Harsh Sunlight, Heavy Rain, Strong Winds, and Shadowy Aura.
- Added _.zed/compile_commands.json_ with the generated compiler invocations used by Zed language tooling.

### Changed

- Renamed the repository-wide root namespace from _Pokemon_ to _PocketCore_ across configuration, core utilities, type registry, logging, tests, and the executable entry point; updated aliases and Doxygen references to the new namespace.
- Updated _main.cpp_ to use _PocketCore_ namespace aliases and renamed its local configuration object to _typeRegistryConfig_.
- Made optional test-integration, test-mock, and benchmark source discovery tolerant of absent directories by passing their _wildcard_ expansions to _find_.
- Added _COMPILER_VERSION_ to both clang-tidy Make targets so analysis parses sources using the same C++ standard as compilation.
- Expanded the clang-tidy allowed-macro expression from debug-only names to both \_ATTR**and \_DEBUG** prefixes.
- Reordered _TypeRegistry_ storage so type entries precede the type chart; public behavior and capacity remain unchanged.
- Updated affected configuration and registry tests for the _PocketCore_ namespace without changing their asserted behavior.

### Removed

- Removed the committed sample messages from _pokemon.log_, leaving the log file empty.

## [0.2.19] - 2026-05-01

### Added

- Added a 24-scenario Catch2 suite for _TypeRegistryConfiguration_ covering:
  - Cell, row, column, name, ID, span, count, and containment queries
  - Positional and name-keyed row/column mutation
  - Single and batch add/remove operations, enum and stable-ID removal, rename, and matchup reset
  - Capacity, duplicate, missing-reference, rollback, round-trip, and _RegistryErrorInfo_ behavior
- Added targeted LCOV branch/region exclusions around defensive optional-access and bounds-checked paths whose failure branches are prevented by registry invariants.
- Added explicit not-found logging to the name-based _resetMatchups()_ overload; both overloads now preserve the missing name or stringified stable ID in _RegistryErrorInfo::mContext_.

### Changed

- Standardized type-identifier spelling from _Id_/_id_ to _ID_ across _TypeEntry_, _TypeRegistry_, _TypeRegistryConfiguration_, assertion messages, implementations, and tests:
  - Renamed APIs including _getTypeID()_, _getNextTypeID()_, _setNextTypeID()_, _findIndexByTypeID()_, and _incrementNextTypeID()_
  - Renamed parameters, locals, and _TypeEntry::typeID_ to match the public API
- Changed the defensive default mapping in _RegistryErrorInfo::errorKindToString()_ from _UnknownRegistryError_ to _UnknownError_.
- Simplified _resolveIndex()_ to return _findIndexByTypeID(...).value()_ after _getTypeID()_ succeeds, documenting and excluding the guaranteed optional-access path.
- Updated the comment-writing instructions to use direct member-variable Doxygen comments without redundant _@var_ tags.

### Removed

- Removed the unused _RegistryError::BatchMismatch_ value and its string-conversion branch.
- Removed duplicate-type validation from the positional _addType()_ path where prior validation already guarantees uniqueness.
- Removed the defensive registered-count clamp in _rollbackEntries()_; callers already maintain the registry-capacity invariant.
- Removed the second not-found branch from _resolveIndex()_ because a successful name-to-ID lookup guarantees that the same registry contains the corresponding internal index.

## [0.2.18] - 2026-05-01

### Added

- Added _-Winline_ back to GCC warning-as-error builds so failed compiler inlining requests are reported.
- Added the portable _ATTR_NOINLINE_ macro, which expands to `__attribute__((noinline))` when supported and otherwise expands to nothing; its documentation records code-size, profiling, debugging, constexpr, and hot-path trade offs.
- Added C++26 diagnostic reasons to the deleted _Timer_ copy/move operations and all deleted _Logger_ construction, assignment, and destruction operations.

### Changed

- Applied _ATTR_NOINLINE_ to _TypeRegistry::getTypeId()_ to keep the linear name lookup as a discrete call and avoid the GCC 16 inline-growth build failure.

### Fixed

- Corrected the documented GCC reflection flag from _-frelection_ to _-freflection_.

### Removed

- Removed the unused _sfloat_ alias for `_Float16` from _typedefs.h_.

## [0.2.17] - 2026-04-30

### Added

- Added the _IsEnum_ concept as a named wrapper over _std::is_enum_v_ for compile-time enum constraints.
- Added the _sfloat_ alias for `_Float16`.
- Added disabled prototype code for a reflection-based enum-to-string utility, with notes explaining that GCC can parse it but clangd does not yet support the required reflection flag.

### Changed

- Changed the Makefile language option from the provisional _-std=c++2c_ spelling to the finalized _-std=c++26_ spelling.

### Removed

- Removed _-Winline_ from GCC warning-as-error builds while investigating compiler inline-growth failures.

## [0.2.16] - 2026-04-30

### Changed

- Updated _setUpGCC()_ to copy _libstdc++.so.6.0.35_ from the custom compiler installation instead of _libstdc++.so.6.0.34_, matching GCC 16.1.0's runtime artifact.

## [0.2.15] - 2026-04-30

### Changed

- Updated the requested custom GCC version in _makefileDependencies.sh_ from 15.2.0 to 16.1.0 and changed its _update-alternatives_ priority from 15 to 16.

## [0.2.14] - 2026-04-29

### Added

- Added _bear_ to the full developer-tool package installation in _makefileDependencies.sh_, enabling generation of compilation databases from Make builds.

## [0.2.13] - 2026-04-29

### Removed

- Removed the CodeQL build step's _PATH_, _LIBRARY_PATH_, and _LD_LIBRARY_PATH_ overrides for _/usr/local/gcc-latest_; replacing _PATH_ prevented the runner from locating _make_.

## [0.2.12] - 2026-04-29

### Added

- Added custom GCC environment overrides to the CodeQL build step:
  - Prepended _/usr/local/gcc-latest/bin_ to _PATH_
  - Set _LIBRARY_PATH_ and _LD_LIBRARY_PATH_ to the compiler's _lib64_ directory
- Added an always-running safeguard that creates the C/C++ CodeQL TRAP tarball directory when the temporary CodeQL database exists.

## [0.2.11] - 2026-04-29

### Added

- Set CodeQL initialization to _build-mode: manual_, explicitly associating analysis with the workflow's existing _make_ build rather than auto build inference.

## [0.2.10] - 2026-04-29

### Changed

- Updated the CodeQL workflow's repository checkout from _actions/checkout@v4_ to _actions/checkout@v6_, restoring the action version used before the advanced-template conversion.

## [0.2.9] - 2026-04-29

### Changed

- Moved _github/codeql-action/analyze@v4_ immediately after the manual _make_ step and before all dependency-cache persistence and permission-repair steps.
- Preserved the language-specific CodeQL result category; no cache keys or cached artifacts changed.

## [0.2.8] - 2026-04-29

### Changed

- Converted _codeql-analysis.yml_ to GitHub's advanced CodeQL workflow structure:
  - Renamed the workflow to _CodeQL Advanced_ and the job to include the matrix language
  - Changed the language matrix from _cpp_ to _c-cpp_ and added a language-specific analysis category
  - Added package-read permission and retained action, content, and security-event permissions
  - Moved CodeQL initialization after dependency/toolchain setup and before the manual project build
  - Temporarily changed repository checkout from v6 to the template's v4
- Changed both CodeQL and test jobs from the fixed _ubuntu-24.04_ image to _ubuntu-latest_.

## [0.2.7] - 2026-04-29

### Changed

- Updated CodeQL database initialization from _github/codeql-action/init@v3_ to _init@v4_.

## [0.2.6] - 2026-04-29

### Changed

- Updated the analysis upload/execution step from _github/codeql-action/analyze@v2_ to _analyze@v4_.

## [0.2.5] - 2026-04-29

### Added

- Added explicit creation of _/usr/local/lib/cmake/spdlog_ before restoring cached Spdlog CMake package files in the CodeQL and test workflows.

### Changed

- Changed Spdlog CMake restoration to copy the cached directory contents into the pre-created destination rather than copying the directory itself, preventing an unintended nested _spdlog/spdlog_ path.

## [0.2.4] - 2026-04-29

### Added

- Added the Catch2 package to the cache-hit fallback installation in both CodeQL and test workflows so Catch2 remains available when the full dependency script is skipped.

### Changed

- Renamed the fallback step from _Install Google Mock_ to _Install Testing Frameworks_ and retained Google Mock installation alongside Catch2.

## [0.2.3] - 2026-04-29

### Fixed

- Added _sudo_ to _/usr/local/lib/pkgconfig_ directory creation in the CodeQL and test workflows, allowing the cache restore step to create the root-owned system path.

## [0.2.2] - 2026-04-29

### Added

- Added creation of _/usr/local/lib/pkgconfig_ before cached Spdlog pkg-config metadata is restored in the CodeQL and test workflows.

## [0.2.1] - 2026-04-29

### Changed

- Changed cached Spdlog restoration in both CodeQL and test workflows from _/usr_ to the _/usr/local_ prefix used by the source installation:
  - Libraries restore to _/usr/local/lib_
  - CMake and pkg-config metadata restore beneath _/usr/local/lib_
  - Headers restore to _/usr/local/include_
- Changed the test workflow's GCC cache restoration from a non-recursive copy to _cp -r_, preserving the cached compiler directory hierarchy.

## [0.2.0] - 2026-04-29 (Type Registry Conversion)

### Changed

- Renamed the generic type-registry configuration module to make its ownership explicit:
  - Renamed _Configuration/configuration.h_ to _Configuration/typeRegistryConfiguration.h_
  - Renamed _Configuration/configuration.cpp_ to _Configuration/typeRegistryConfiguration.cpp_
  - Renamed the public _Configuration_ class to _TypeRegistryConfiguration_ and updated every declaration, definition, diagnostic message, include, and construction site
  - Updated _main.cpp_ to include the renamed header and instantiate _Configuration::TypeRegistryConfiguration_
- Reorganized _TypeRegistryConfiguration_ without changing its registry-management contract:
  - Grouped lookup APIs, mutation APIs, higher-level operations, and private helpers into dedicated sections
  - Reordered source definitions to follow the public header and retained add, remove, rename, matchup, reset, rollback, and lookup behavior
  - Updated Doxygen for _MatchupPair_, _TypeDefinition_, all public operations, and internal rollback/index-resolution helpers with parameters, return values, defaults, and failure behavior
- Clarified type-registry configuration metadata in _Configuration/constants.h_:
  - Added a local _ub_ alias and used it for _MAX_TYPES_, _RegistryError_, and _UnspecifiedMatchup_
  - Documented each registry error category, the non-owning context carried by _RegistryErrorInfo_, and neutral versus undefined handling for omitted matchups
  - Reordered _UnspecifiedMatchup_ beside the other helper enum and grouped constants, enums, error data, and private conversion logic
- Renamed _TypeRegistry_ section labels from utility functions to member functions to match their actual ownership.
- Added targeted _bugprone-unchecked-optional-access_ suppressions after successful _REQUIRE_ checks in _typeRegistry.test.cpp_, preserving the existing runtime assertions while removing false-positive clang-tidy reports.

## [0.1.14] - 2026-04-29

### Added

- Added a complete Spdlog cache life cycle to the CodeQL and test workflows:
  - Created cache storage for libraries, CMake package metadata, pkg-config metadata, and headers under _cache/spdlog_
  - Added an _actions/cache@v5_ entry keyed by the runner OS and the _makefileDependencies.sh_ hash, with an OS-level restore prefix
  - Restored _libspdlog\*_, the _spdlog_ CMake package, pkg-config files, and headers into their system locations on a cache hit
  - Saved the corresponding artifacts from _/usr/local_ after dependency installation

### Changed

- Added the Spdlog cache result to the workflow dependency-installation and Google Mock fallback conditions so an incomplete cache set triggers setup.
- Reordered the Spdlog installation copy operations in _makefileDependencies.sh_ to install the library, CMake metadata, pkg-config metadata, and headers in the same order used by cache restoration.

## [0.1.13] - 2026-04-28

### Changed

- Moved ConfigCat detection and installation ahead of the custom GCC setup in _makefileDependencies.sh_, while retaining the existing _libconfigcat.a_ presence check.
- Registered the custom compiler's _c++_ executable with _update-alternatives_ alongside _g++_, _gcc_, and _gcov_, using the same installation prefix and priority.
- Restored the CodeQL and test workflow files to _.github/workflows/_ and removed the temporary standalone ConfigCat workflow introduced during the intermediate CI experiments; those experiments produced no other lasting workflow behavior.

## [0.1.12] - 2026-04-28

### Changed

- Updated _actions/checkout_ from v4 to v6 in the CodeQL, test, and Semgrep workflows.
- Updated all _actions/cache_ uses from v3 to v5 in the CodeQL and test workflows, covering apt packages, apt lists, Google Test, ConfigCat, Curl, hash-library, and GCC caches.
- Replaced interactive-oriented _apt_ calls with _apt-get_ for package update, upgrade, and Rust/Cargo removal operations in _makefileDependencies.sh_.
- Normalized _semgrep.yml_ list and step indentation while preserving its pull-request, branch, path, and scheduled triggers.

## [0.1.11] - 2026-04-28

### Added

- Added ConfigCat, Curl, and hash-library caches to the CodeQL workflow:
  - Created separate library and include cache directories for each dependency
  - Added _actions/cache@v3_ entries keyed by runner OS, dependency name, and the _makefileDependencies.sh_ hash
  - Restored static libraries to _/usr/lib_ and headers to dependency-specific directories under _/usr/include_ on cache hits
  - Saved _libconfigcat.a_, _libcurl.a_, _libhash-library.a_, and their headers after dependency installation

### Changed

- Expanded CodeQL dependency installation conditions to account for ConfigCat, Curl, hash-library, and GCC cache misses.
- Expanded the CodeQL Google Mock fallback condition and the equivalent test-workflow conditions to account for the GCC cache result.

## [0.1.10] - 2026-04-28

### Added

- Added an idempotent ConfigCat installation guard to _makefileDependencies.sh_:
  - Checks for _/usr/lib/libconfigcat.a_ before invoking _setUpConfigCat()_
  - Skips the vcpkg-based ConfigCat, Curl, and hash-library installation when the cached/static ConfigCat library is already present
  - Emits an explicit status message for both the installed and setup paths

## [0.1.9] - 2026-04-28

### Added

- Added a _Find latest GCC_ step to the CodeQL and test workflows that locates the highest versioned _/usr/local/gcc-\*_ directory and exports it as _LATEST_GCC_.

### Changed

- Changed GCC cache saving to copy from _LATEST_GCC_ instead of the hard-coded _/usr/local/gcc-15_ path.
- Changed automated dependency setup to build and register the requested GCC version through _setUpGCC()_ whenever the active compiler version does not match.

### Removed

- Removed the automated _g++-13_ package-installation and _update-alternatives_ fallback from _makefileDependencies.sh_.

## [0.1.8] - 2026-04-28

### Added

- Added GCC caching to _codeql-analysis.yml_ and _testing.yml_:
  - Created _cache/gcc-latest_ and cached it with _actions/cache@v3_ using the runner OS and _makefileDependencies.sh_ hash
  - Restored cached compiler files into _/usr/local/gcc-latest_
  - Registered cached _g++_, _gcc_, and _gcov_ binaries through _update-alternatives_ at priority 100
  - Saved the locally built GCC installation into the cache after each workflow build

### Changed

- Removed the explicit _COMPILER_STANDARD=13_ arguments from CodeQL and test build commands so both workflows use the compiler standard selected by the Makefile.

## [0.1.7] - 2026-04-24

### Changed

- Applied _shellcheck_ and _shellfmt_ remediation throughout _makefileDependencies.sh_:
  - Replaced single-bracket conditions with _[[...]]_ expressions and made compound automated-install conditions explicit
  - Quoted positional parameters, paths, command substitutions, version values, and environment-variable expansions to prevent unintended word splitting and glob expansion
  - Added _cd ... || exit_ guards so failed directory transitions stop the active setup routine
  - Switched interactive input to _read -r_ and guarded optional command/version probes with fallbacks
  - Normalized parallel-build invocations and allowed selected optional setup commands to continue after failure where the script already treated them as non-fatal
- Reformatted the dependency script consistently without changing its set of supported dependency installers.

## [0.1.6] - 2026-04-24

### Changed

- Completed the TypeRegistry test migration from Google Test to Catch2 BDD syntax:
  - Replaced the Google Test and Catch2 matcher headers with _catch_test_macros.hpp_
  - Replaced the shared Google Test fixture and individual _TEST_F_ cases with one _SCENARIO_ organized by _GIVEN_ and _THEN_ sections
  - Converted fatal preconditions to _REQUIRE_/_REQUIRE_FALSE_ and independent assertions to _CHECK_/_CHECK_FALSE_
  - Retained coverage for built-in registration, type-chart cells and rows, name/ID lookups, mutation APIs, counters, spans, case sensitivity, and undefined slots
  - Added the cognitive-complexity lint suppression required by the nested BDD structure

## [0.1.5] - 2026-04-24

### Added

- Added _-D${TEST_STANDARD}_ to both the serial _clang-tidy_ and parallel _run-clang-tidy_ invocations so static analysis follows the selected test framework's preprocessor path.

### Changed

- Changed the default _TEST_STANDARD_ from _googletest_ to _catch2_.
- Rewrote the repository test-generation instructions around Catch2 BDD conventions:
  - Standardized on _SCENARIO_, _GIVEN_, _WHEN_, and _THEN_
  - Defined _REQUIRE_ versus _CHECK_ usage, matcher guidance, exception assertions, comparison-expression wrapping, and table-driven scenarios
  - Updated the example test and lint-suppression guidance for nested BDD cases
- Migrated the following suites from Google Test cases and fixtures to Catch2 BDD sections while preserving their tested behavior:
  - _cconcepts.test.cpp_
  - _timer.test.cpp_
  - _contiguousSequence.test.cpp_
  - _logger.test.cpp_
  - _floatUtility.test.cpp_
  - _overflowProtection.test.cpp_

## [0.1.4] - 2026-04-22

### Added

- Added _-D${TEST_STANDARD}_ to _COMPILER_FLAGS_TEST_, exposing the selected framework as a test-build preprocessor symbol.

### Changed

- Moved _TEST_STANDARD_ beside the compiler configuration so it is available while test compiler flags are assembled.
- Made _testMain.cpp_ framework-aware:
  - Includes _catch_session.hpp_ and runs _Catch::Session_ when _catch2_ is defined
  - Includes _gtest/gtest.h_, initializes Google Test, and runs _RUN_ALL_TESTS()_ when _googletest_ is defined
  - Produces a compile-time error when neither supported framework symbol is defined
  - Standardized the entry-point argument declaration to _char \*argv[]_

## [0.1.3] - 2026-04-22

### Added

- Added the _TEST_STANDARD_ Makefile selector with _googletest_ as the default.
- Added framework-specific link variables:
  - _GOOGLE_TEST_LIBRARIES_ selects project libraries, coverage, Google Test, Google Mock, and pthread linkage
  - _CATCH2_LIBRARIES_ selects Catch2 linkage
- Added framework-specific execution variables:
  - _GOOGLE_TEST_EXECUTION_FLAGS_ supplies repeat, brief-output, and optional break-on-failure arguments
  - _CATCH2_EXECUTION_FLAGS_ supplies compact reporting and optional break-on-failure arguments

### Changed

- Changed _TEST_LIBRARIES_ and _TEST_EXECUTION_FLAGS_ into aggregate variables whose non-empty values are selected by _TEST_STANDARD_, allowing the existing test targets to drive either framework.

## [0.1.2] - 2026-04-22

### Added

- Added Catch2 to the Ubuntu dependency set and linked _libCatch2_ into test builds alongside the existing Google Test libraries.
- Added _LCOV_EXCLUDE_ASSERT_ to treat assertion branches and _LCOV_EXCL_BR_ annotations as coverage exclusions.
- Added TypeRegistry coverage annotations around built-in registration and bounds-checked lookup paths whose exception branches are unreachable after validated preconditions.
- Added the _clean_coverage_ target to remove accumulated _.gcda_ counters and made _coverage_ depend on it as well as generated HTML output.
- Added _clean_coverage_ and _run_tidy_ to the Make target documentation in _README.md_ and _makefileDescriptions.rst_.
- Strengthened TypeRegistry and Logger tests:
  - Covered built-in entries, stable IDs, matchup values, undefined slots, lookup failures, mutation round trips, and registration counters
  - Covered logger defaults, output-file changes, logger replacement, all severity methods, and logging-failure messages

### Changed

- Applied the assertion exclusion rule consistently during capture, test-file removal, and HTML generation.
- Simplified coverage and profiling output paths from escaped relative paths to _coverage_ and _profiling_.
- Corrected TypeRegistry API documentation by removing _std::out_of_range_ claims from methods whose index validity is enforced as a precondition.
- Replaced inferred test variables with explicit project or standard-library types and introduced local namespace aliases where they improve the expected type contract.
- Changed the Logger test helper _readLogFile()_ to accept an optional file-name override and return the file contents directly, allowing tests to verify output after changing log destinations.
- Updated Make target documentation to explain coverage-counter cleanup and the serial versus parallel clang-tidy workflows.

### Removed

- Removed redundant escaped current-directory prefixes from _GENHTML_OUTPUT_FOLDER_ and _PROFILE_FOLDER_.

## [0.1.1] - 2026-04-20

### Fixed

- Replaced the inherited _project-name_ README heading with _pocketcore_.
- Replaced Sphinx template placeholders with the _pocketcore_ HTML title and the repository URL _<https://github.com/Phaysik/pocketcore>_, enabling the documentation repository button to target the correct project.

## [0.1.0] - 2026-04-20 (Type Configuration Update)

### Added

- Imported the initial project foundation from [CPPBase](https://github.com/Phaysik/CPPBase/commit/34d5cc39654bd807c3f62a8af207e07d3f971f1d), comprising 65 source, test, build, workflow, editor, hook, and documentation files.
- Added the initial Pokemon type model:
  - _Types_ enumerates the 18 standard types plus Stellar with stable underlying byte values
  - _TypeEffectiveness_ represents undefined, no-effect, not-very-effective, neutral, and super-effective chart states
  - Built-in constants define display names and offensive matchup rows for every registered type
- Added the fixed-capacity _TypeRegistry_:
  - Initializes all built-in types and their matchup data in a 20-entry registry
  - Stores stable type IDs separately from internal array indices and tracks both the registered count and next custom ID
  - Exposes entry, chart-cell, chart-row, name, ID, count, and read-only registered-span queries
  - Exposes controlled entry/chart mutation, count/ID counter updates, internal-index lookup, and name/ID containment checks
- Added the user-facing _Configuration_ API for custom type management:
  - _MatchupPair_ and _TypeDefinition_ describe name-keyed offensive and defensive matchup input
  - Supports adding one type or an atomic batch, with neutral or undefined defaults for omitted matchups
  - Supports removal by name, built-in enum, stable ID, or atomic name batch; renaming retains IDs and matchup data
  - Supports setting individual matchups, complete offensive rows, and complete defensive columns from ordered values or name-keyed pairs
  - Supports resetting a type's row and column, querying cells/rows/columns, and forwarding registry identity and count queries
  - Reports capacity, duplicate, missing-type, matchup, and batch failures through _std::expected<..., RegistryErrorInfo>_ with optional logging context
- Added reusable core and utility modules:
  - Compiler-attribute macros, concepts, typedefs, and ConfigCat integration
  - Timing, contiguous-sequence access, logging, floating-point comparison, overflow protection, input, and random-number helpers
  - Configuration flags and assertion-message constants used by registry preconditions
- Added build and developer tooling:
  - Make targets for release/development builds, tests, coverage, clang-tidy, cppcheck, flawfinder, formatting, Doxygen/Sphinx documentation, Valgrind, Google Benchmark, Tracy, and gprof
  - Dependency setup functions for GCC, LLVM tools, Doxygen, Sphinx, Tracy, ConfigCat, Vulkan, Google Benchmark, and Spdlog
  - Clang format/tidy/language-server configuration, VS Code build and debug tasks, repository initialization, and commit hooks
- Added continuous integration and security analysis through test, CodeQL, and Semgrep GitHub Actions workflows.
- Added Doxygen and Sphinx project documentation, including setup guidance and Make/dependency reference tables.
- Added Google Test coverage for concepts, TypeRegistry, timer, contiguous sequence, logger, floating-point utilities, and overflow protection.

[0.12.17]: https://github.com/Phaysik/pocketcore/commit/
[0.12.16]: https://github.com/Phaysik/pocketcore/commit/e622e64a29215e50a70788359c7ce87b9387c0f6
[0.12.15]: https://github.com/Phaysik/pocketcore/commit/026b606e93777a7709c87bbc42a9918a492c2672
[0.12.14]: https://github.com/Phaysik/pocketcore/commit/20da569d8a90c8d9486006cae57aa8c82622e22a
[0.12.13]: https://github.com/Phaysik/pocketcore/commit/9f14ed4b0c53ee56dc191ce1028e87be11d6707a
[0.12.12]: https://github.com/Phaysik/pocketcore/commit/bf7af55f51a6326138ba5e53a8926a4ee0781b60
[0.12.11]: https://github.com/Phaysik/pocketcore/commit/9dff04af8ac262f716e5150476df7cb4329f0a58
[0.12.10]: https://github.com/Phaysik/pocketcore/commit/94964584ab90c2e64135f53c6907354c224a5d59
[0.12.9]: https://github.com/Phaysik/pocketcore/commit/0b71f6b558826e6e528e5f60540efd68ad62868a
[0.12.8]: https://github.com/Phaysik/pocketcore/commit/34ed13d54861723d51c86523673c40aea6e22ddf
[0.12.7]: https://github.com/Phaysik/pocketcore/commit/5980aac45384316d35e56a2781689f5ad3124bce
[0.12.6]: https://github.com/Phaysik/pocketcore/commit/dd488ac0cca068daa07c3442e3a40e7a9509974d
[0.12.5]: https://github.com/Phaysik/pocketcore/commit/8a892e0b9f8cde8e79bad4f417f3f1d2710f906f
[0.12.4]: https://github.com/Phaysik/pocketcore/commit/bab3639025ac8e7c495182b026dcdb4e544d28b6
[0.12.3]: https://github.com/Phaysik/pocketcore/commit/63d8b22e3aec39119874deb941cd8d715cbbd3f0
[0.12.2]: https://github.com/Phaysik/pocketcore/commit/2371e413f2ceb47cf76a16904fbaae7a8dcdfdb8
[0.12.1]: https://github.com/Phaysik/pocketcore/commit/d31881e3683fd8f812932de37ac794def4132fac
[0.12.0]: https://github.com/Phaysik/pocketcore/compare/v0.11.0...v0.12.0
[0.11.6]: https://github.com/Phaysik/pocketcore/commit/2fe6a59f393c725a303ce452ceae2f683a455971
[0.11.5]: https://github.com/Phaysik/pocketcore/commit/f1078de88e81dc9e124dba0542a50a57ad53fd84
[0.11.4]: https://github.com/Phaysik/pocketcore/commit/9794e874571dd81592e23917b0045984a9bf85e0
[0.11.3]: https://github.com/Phaysik/pocketcore/commit/385dcd5ed5e5db8eafe2fbcbb78ba6de3d080165
[0.11.2]: https://github.com/Phaysik/pocketcore/commit/b547c616f310a4f8364b435354a63eb474f67d20
[0.11.1]: https://github.com/Phaysik/pocketcore/commit/91db1f0a76b93ef5912afe8f631a89d211801365
[0.11.0]: https://github.com/Phaysik/pocketcore/compare/v0.10.0...v0.11.0
[0.10.11]: https://github.com/Phaysik/pocketcore/commit/fe357929f7fcf30060aceb503af6c4a5de730375
[0.10.10]: https://github.com/Phaysik/pocketcore/commit/e49201937a986b442e2c3b492f916c48d2c6f4fc
[0.10.9]: https://github.com/Phaysik/pocketcore/commit/bca45ef07e5b06df0dc9a89457b7f630dce445aa
[0.10.8]: https://github.com/Phaysik/pocketcore/commit/1dc2b2bdcea311a75b3824ea4b21b55530fe2789
[0.10.7]: https://github.com/Phaysik/pocketcore/commit/49be44401043c4bdb76f60c02ee0419a31e41404
[0.10.6]: https://github.com/Phaysik/pocketcore/commit/7e48ebe8f83422c81e32dd7048167f5832495b3a
[0.10.5]: https://github.com/Phaysik/pocketcore/commit/a48456afbe1eec4b08a4e6c4136b3beef0d68cbe
[0.10.4]: https://github.com/Phaysik/pocketcore/commit/7335cd39196ef3c407f10fddaf5eb5e50233779a
[0.10.3]: https://github.com/Phaysik/pocketcore/commit/3eb890451f52df895fded53c66b1e657e57be4fc
[0.10.2]: https://github.com/Phaysik/pocketcore/commit/a76e6814e9c4a56eaf14f1d61781ccf7f3b49f01
[0.10.1]: https://github.com/Phaysik/pocketcore/commit/19c7b46aa69982f5661553c89926e3e4ca9304b6
[0.10.0]: https://github.com/Phaysik/pocketcore/compare/v0.9.0...v0.10.0
[0.9.19]: https://github.com/Phaysik/pocketcore/commit/17f208e5da35a1f896f83aea363b066148a6f9d5
[0.9.18]: https://github.com/Phaysik/pocketcore/commit/ce47285096501ba3f140e275c0bcbf1ebcd55533
[0.9.17]: https://github.com/Phaysik/pocketcore/commit/2daa16b939a4ad52c06231d9a6acb9b84e056db3
[0.9.16]: https://github.com/Phaysik/pocketcore/commit/594e950ef9b32c56f6cb4c0cf11bc442054e0dea
[0.9.15]: https://github.com/Phaysik/pocketcore/commit/6ddf41c5e2e4d9f15b34ad480de3a65293c57438
[0.9.14]: https://github.com/Phaysik/pocketcore/commit/7a51a0f1d8a20cf45cbcb51a2f194668883f1757
[0.9.13]: https://github.com/Phaysik/pocketcore/commit/24ef5d4744da3f1075f92f01f6135975e9aba23a
[0.9.12]: https://github.com/Phaysik/pocketcore/commit/8da0816d88ac05452b3f00727772ee40896a398d
[0.9.11]: https://github.com/Phaysik/pocketcore/commit/7515a65c76bf698377dedc48197e19b4155974c2
[0.9.10]: https://github.com/Phaysik/pocketcore/commit/bc44bf391b1e59888334bf17a61f906e4051649e
[0.9.9]: https://github.com/Phaysik/pocketcore/commit/702111e3376598faab34a308c2363e78ec97b758
[0.9.8]: https://github.com/Phaysik/pocketcore/commit/7732ddca583a17ba61b0ed3bc586b437ff909635
[0.9.7]: https://github.com/Phaysik/pocketcore/commit/2118859498a40801db0e578e2909aa4da7ce2b3d
[0.9.6]: https://github.com/Phaysik/pocketcore/commit/9f1dee5dcdac493830bf090fba78d0bf14a3a458
[0.9.5]: https://github.com/Phaysik/pocketcore/commit/1e3a43c5607b964a5ea9f790f816a83a5f5f9326
[0.9.4]: https://github.com/Phaysik/pocketcore/commit/bb397bf5fff716b858e13c166fb7befece6c3c2f
[0.9.3]: https://github.com/Phaysik/pocketcore/commit/b60130d6d82bb2029d4d1d72bd67946e55f85cae
[0.9.2]: https://github.com/Phaysik/pocketcore/commit/ed70413b4dc5f616cc717d07d0010577d6d394df
[0.9.1]: https://github.com/Phaysik/pocketcore/commit/08c8f792bc9147b861155a0f7027deccb8daa3bf
[0.9.0]: https://github.com/Phaysik/pocketcore/compare/v0.8.0...v0.9.0
[0.8.7]: https://github.com/Phaysik/pocketcore/commit/2ce0d1b35250c945834fb7734ce2c1affac56dc0
[0.8.6]: https://github.com/Phaysik/pocketcore/commit/12734b59dc609938ef9ab95b3ba459f873e8cc64
[0.8.5]: https://github.com/Phaysik/pocketcore/commit/ac24d49144d29e4e0008efa3520dd2cbdc98b17c
[0.8.4]: https://github.com/Phaysik/pocketcore/commit/b892dace57b8774f4ba43f5c7619a8e7633ecca4
[0.8.3]: https://github.com/Phaysik/pocketcore/commit/4ee1c39b8754d3f769f8aa3032e0cc3b56d283bf
[0.8.2]: https://github.com/Phaysik/pocketcore/commit/eedbf1173975c37d583fa486088ac91eeb873ccb
[0.8.1]: https://github.com/Phaysik/pocketcore/commit/6b29bca60d66011181ba0fe56a0eb9f1f600564b
[0.8.0]: https://github.com/Phaysik/pocketcore/compare/v0.7.0...v0.8.0
[0.7.7]: https://github.com/Phaysik/pocketcore/commit/6332560ff031fb7a2f1935120d893a1a8afb1b57
[0.7.6]: https://github.com/Phaysik/pocketcore/commit/075a71559f202ea7da534fcfe96b255d44b4e071
[0.7.5]: https://github.com/Phaysik/pocketcore/commit/221bce1911d7ced2b3c5c20db2a4c23df5c53e7e
[0.7.4]: https://github.com/Phaysik/pocketcore/commit/79b756182540ad86248f64b44be62e0d228cb8db
[0.7.3]: https://github.com/Phaysik/pocketcore/commit/ecefcf3e5eedd3e7bac2e714eaace30e4390656c
[0.7.2]: https://github.com/Phaysik/pocketcore/commit/82ce0d3040412baf345fec3bd5b92fa5154d03af
[0.7.1]: https://github.com/Phaysik/pocketcore/commit/58e84b19d5a665bcae6d61c51fe2865629cbcbea
[0.7.0]: https://github.com/Phaysik/pocketcore/compare/v0.6.0...v0.7.0
[0.6.4]: https://github.com/Phaysik/pocketcore/commit/79e8eb889ef4b2f7c7f7513cc093fa06192ee097
[0.6.3]: https://github.com/Phaysik/pocketcore/commit/d5bd4b247aa96c01045a2ee9ee4f61ab0f07c4b8
[0.6.2]: https://github.com/Phaysik/pocketcore/commit/31271d07d7a4a6b2d58971a3f19b8e6d72fe78e7
[0.6.1]: https://github.com/Phaysik/pocketcore/commit/3b13b86153e35c999b149f04019d12c7076c8ba7
[0.6.0]: https://github.com/Phaysik/pocketcore/compare/v0.5.0...v0.6.0
[0.5.3]: https://github.com/Phaysik/pocketcore/commit/dd5311ed0a5ea1e424875123dfc6211855667232
[0.5.2]: https://github.com/Phaysik/pocketcore/commit/8646188fbf35c3903b29d0713e3354b0ce5df12a
[0.5.1]: https://github.com/Phaysik/pocketcore/commit/f234e7c066c190e35cf0a83e0651a258f545b810
[0.5.0]: https://github.com/Phaysik/pocketcore/compare/v0.4.0...v0.5.0
[0.4.1]: https://github.com/Phaysik/pocketcore/commit/adf96f1bd73412520a65d92804be2ae17274b964
[0.4.0]: https://github.com/Phaysik/pocketcore/compare/v0.3.0...v0.4.0
[0.3.2]: https://github.com/Phaysik/pocketcore/commit/4e63ffb001c199804f40a73736977c4ac16a6193
[0.3.1]: https://github.com/Phaysik/pocketcore/commit/5b3ec606ce94ca29d9a8bb8f08d74af50f06a525
[0.3.0]: https://github.com/Phaysik/pocketcore/compare/v0.2.0...v0.3.0
[0.2.19]: https://github.com/Phaysik/pocketcore/commit/b592768a73efdd33f5853434784c6f09e569cc1d
[0.2.18]: https://github.com/Phaysik/pocketcore/commit/4acbfe60d336ea9f9509fb55f8750c7bf8288f80
[0.2.17]: https://github.com/Phaysik/pocketcore/commit/78f4c4a14fda2100742141d2c3583a24dd190f85
[0.2.16]: https://github.com/Phaysik/pocketcore/commit/1dc841fd8c4ede663b45999b14d444ab9e3eb97a
[0.2.15]: https://github.com/Phaysik/pocketcore/commit/5cf30bbb99b8c172b7aab505b1f400a0e774abb0
[0.2.14]: https://github.com/Phaysik/pocketcore/commit/73f94c87699369b23d2477460c213a201128939f
[0.2.13]: https://github.com/Phaysik/pocketcore/commit/4900d97b490ca031d421e922638e939b401095cd
[0.2.12]: https://github.com/Phaysik/pocketcore/commit/cbf33ba16fda7326ffcb33c6d22b48089c98d618
[0.2.11]: https://github.com/Phaysik/pocketcore/commit/5c5d226faf2bfe0dd78135db68fae2e9ecf459c4
[0.2.10]: https://github.com/Phaysik/pocketcore/commit/07b8c332a427ddc6c01c41a209f4cce41081df0b
[0.2.9]: https://github.com/Phaysik/pocketcore/commit/49623a32e1ff273da0eee57e785a43d7742e5ab9
[0.2.8]: https://github.com/Phaysik/pocketcore/commit/54965b3bbd5931ad5c7a4bfdd16e7d0f87c7729b
[0.2.7]: https://github.com/Phaysik/pocketcore/commit/84e84b9c1efc05dcac3a3a88d560e38964131352
[0.2.6]: https://github.com/Phaysik/pocketcore/commit/b85a446d98b4da4d7f85b3566b6fd053a75aed17
[0.2.5]: https://github.com/Phaysik/pocketcore/commit/0a405fc3a9ad06d9d77112e0b17fe3e71d077336
[0.2.4]: https://github.com/Phaysik/pocketcore/commit/9b695359f19dc4e4dbf5af1f4475efe16df9dcb4
[0.2.3]: https://github.com/Phaysik/pocketcore/commit/7c4956adbb52c87dfcce7499c512592bb4c29747
[0.2.2]: https://github.com/Phaysik/pocketcore/commit/54f68e5b31197ec8264a1474f309009a39c5b3d8
[0.2.1]: https://github.com/Phaysik/pocketcore/commit/7bcdf6cb09123e7cc563b0e16db15cf4f4117054
[0.2.0]: https://github.com/Phaysik/pocketcore/compare/v0.1.0...v0.2.0
[0.1.14]: https://github.com/Phaysik/pocketcore/commit/85937dbd94b09ceb28d63e864690ac54904559bf
[0.1.13]: https://github.com/Phaysik/pocketcore/commit/a501cde9c1995a300eb0e431ce8fc4763798e6d5
[0.1.12]: https://github.com/Phaysik/pocketcore/commit/c91cee6843d94cb516ef8c0c148fb6d07faed84c
[0.1.11]: https://github.com/Phaysik/pocketcore/commit/c49bb6c4efecebcf5db4c8251d1e1de9fd3a0ffa
[0.1.10]: https://github.com/Phaysik/pocketcore/commit/216752345ef67b6806ce0ed51a65a8a711dfa569
[0.1.9]: https://github.com/Phaysik/pocketcore/commit/881cf898ec401883c7b2baa3ee09a4cf55327a84
[0.1.8]: https://github.com/Phaysik/pocketcore/commit/038a4b6ff46170b0fccb32be6415d4f606622203
[0.1.7]: https://github.com/Phaysik/pocketcore/commit/871621e2f404ee46f876145156b6541339734626
[0.1.6]: https://github.com/Phaysik/pocketcore/commit/0acc0dd4a3825cd294fe8d4eff601972f07471e7
[0.1.5]: https://github.com/Phaysik/pocketcore/commit/84eb92d273ed46d022339d86ba6050f050ef6995
[0.1.4]: https://github.com/Phaysik/pocketcore/commit/b73645688008b0b9235e1c79aba7c072e9c9c676
[0.1.3]: https://github.com/Phaysik/pocketcore/commit/b3f60fe4604dfabd8ec36d1c67814b74875deac8
[0.1.2]: https://github.com/Phaysik/pocketcore/commit/db6c9380722de081275a91be968e3063ff534b46
[0.1.1]: https://github.com/Phaysik/pocketcore/commit/63e8313bdc747329d441a0ec25cde1d76948ba35
[0.1.0]: https://github.com/Phaysik/pocketcore/releases/tag/v0.1.0
