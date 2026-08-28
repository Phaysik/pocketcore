# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/2.0.0/), and this project adheres to _vX.Y.Z_ versioning where _X_ represents an _edition_, _Y_ represents an _update_, and _Z_ represents an _addendum_.

## [0.11.0] - 2026-08-21 (BattleEngine Update)

### Added

- Added three critical battle execution methods to _BattleEngine_ for handling move effects and damage:
  - _executeWeightHitCountPolicy()_ - Orchestrates weighted hit distribution for multi-hit moves, managing per-attempt before-hit triggers and suppression scopes to ensure consistent effect resolution across variable hit counts
  - _executeFixedHitCountPolicy()_ - Handles fixed-count multi-hit moves with optional per-attempt trigger execution, with early termination on miss
  - _executeDamageApplication()_ - Applies accumulated context damage to target health using active multipliers, with lethal damage clamping to prevent underflow

### Changed

## [0.10.11] - 2026-08-21

### Changed

- Optimized GitHub Actions workflow build performance by utilizing maximum available CPU cores:
  - Updated _codeql-analysis.yml_ to use `make -j$(nproc)` for parallel compilation in static analysis
  - Updated _testing.yml_ to use `make -j$(nproc)` for parallel compilation in test builds
  - This reduces CI/CD pipeline execution time by allowing full parallelization on multi-core runners

## [0.10.10] - 2026-08-21

### Changed

- Strengthened const-correctness in _hasReserve()_ function signature to prevent unintended Pokemon pointer modifications:
  - Changed parameter from _std::span<Pokemon \*>_ to _std::span<Pokemon \*const>_
  - This enforces that the function receives a view of non-modifiable Pokemon pointers, improving API safety and signaling immutable intent to callers

## [0.10.9] - 2026-08-21

### Added

- Comprehensive Doxygen documentation added to 31 battle helper functions in _battleHelpers.h_ and _battleEngine.h_:
  - Core party management: _hasReserve()_, _getOppositeSide()_, _getSideOrder()_, _sideHasHealthyPokemon()_
  - Active slot utilities: _activeSlots()_, _party()_, _isActive()_, _isHealthy()_, _isAdjacent()_, _contextSlot()_
  - Battle validation: _anyPartyPokemonNull()_, _hasDuplicatePokemonPointers()_, _healthyPokemonInParty()_, _assignActiveSlots()_, _canTarget()_
  - Turn execution: _getEffectiveSpeed()_, _makeMoveContext()_, _handleMovePrioritization()_, _applyRecoil()_, _resolveHitCount()_
  - Action processing: _getMoveTargets()_, _validateMoveAction()_, _validateSwitchAction()_, _getBattleTarget()_, _getValidationResult()_, _getResult()_, _appendSide()_, _targetExists()_

### Changed

- Applied _clang-format_ code style standardization to 5 battle-related header and test files:
  - _battleEngine.h_, _battleTargetsAndTriggers.h_, _effectSourceAndSuppression.h_, _moveMeta.h_, _moveRegistryConfiguration.test.cpp_
- Enhanced _hasReserve()_ parameter type from _std::span<Pokemon \*>_ to _std::span<Pokemon \*const>_ for improved const-correctness

## [0.10.8] - 2026-08-18

### Added

- Introduced _BattleEventRole_ struct to categorize participants in battle events:
  - Distinguishes between User (active Pokemon using an ability/item), Target (receiving the effect), and Any (flexible matching)
  - Enables more granular trigger filtering and conditional effect execution based on role context
- Introduced _BattleEventID_ struct to uniquely identify trigger events in the battle system:
  - Replaces the previous _BattleTriggerID_ with an event-centric model
  - Supports ability triggers, item triggers, move triggers, and battle state transitions
- Added comprehensive documentation to _BattleTargetID_ and _BattleRangeID_ structs explaining targeting mechanics and range resolution

### Changed

- Refactored all occurrences of _BattleTriggerID_ to use the new _BattleEventID_ and _BattleEventRole_ pair:
  - Improves separation of concerns between event identification and participant roles
  - Simplifies effect dispatch logic by combining role information with event identity
- Extracted shared `PocketCore::` namespace prefixes into `using` statements for improved code clarity in:
  - _abilityRegistry.h_, _effectRegistry.h_, _itemRegistry.h_, _moveRegistry.h_, _multiplierRegistry.h_, _statusRegistry.h_, _terrainRegistry.h_, _typeRegistry.h_

### Fixed

- Added missing `#include` directives to _battleHelpers.cpp_ to resolve compilation issues

### Removed

- Removed the deprecated _BattleTriggerID_ enum as its functionality is superseded by _BattleEventID_ + _BattleEventRole_

## [0.10.7] - 2026-08-18

### Added

- Expanded _BattleEngine_ with core turn execution methods:
  - _executeTurn()_ - Main turn loop orchestrating move validation, prioritization, execution, and state refresh
  - _executeMove()_ - Dispatches a single move through its before-hit, hit/miss, and after-hit triggers
  - _executeEndTurnTrigger()_ - Processes end-of-turn effects for abilities and items
- Added battle helper functions for move validation and action processing:
  - _getValidationResult()_ - Centralizes move/switch action validation logic
  - _getBattleTarget()_ - Resolves a validated move target from raw target identifier
  - _handleMovePrioritization()_ - Sorts move actions by priority and speed, with deterministic tie-breaking by side and slot
- Added _ATTR_NOINLINE_ attribute to _getMetadata()_ method to prevent unwanted compiler inlining
- Added practical example in _main.cpp_ demonstrating basic turn execution workflow for developers

### Removed

- Removed _configFlags.h_ configuration header (functionality consolidated into other config files)

## [0.10.6] - 2026-08-17

### Added

- Added _ATTR_CONST_ attribute to _getState()_ method in _BattleEngine_:
  - Signals to the compiler that the method produces consistent output based on object state alone, enabling optimization
  - Improves static analysis and allows compiler optimizations like common subexpression elimination

### Fixed

- Fixed clang-tidy warnings on _getState()_ in _battleEngine.cpp_:
  - Wrapped compiler-specific code in GCC diagnostic push/pop pragmas to suppress false positives
  - Maintains code cleanliness while acknowledging unavoidable compiler-specific patterns

## [0.10.5] - 2026-08-17

### Added

- Expanded _BattleEngine_ with two essential battle mechanics methods:
  - _getState()_ - Provides read-only access to the current battle state for query operations
  - _switchPokemon()_ - Handles active Pokemon substitution with validation of reserve availability

## [0.10.4] - 2026-08-17

### Added

- Added _executeMoveEffects()_ method to dispatch and resolve move-triggered effects through the effect system
- Added move and switch validation helper functions:
  - _getMoveTargets()_ - Resolves valid targets for a move based on its targeting rules and range
  - _validateMoveAction()_ - Validates move selection against active Pokemon's move set and accuracy
  - _validateSwitchAction()_ - Validates switch target exists in reserve and is not fainted

### Changed

- Refactored _resolveTargets()_ function from _battleEngine.h_ to _battleHelpers.h_:
  - Consolidates all target resolution logic in the helpers module for better code organization

## [0.10.3] - 2026-08-14

### Added

- Implemented core _BattleEngine_ infrastructure for turn-based battle orchestration:
  - _startBattle()_ - Initializes battle state with two Pokemon parties, assigns initial active Pokemon with ability/item trigger activation
  - Suppression scope management for abilities and items with proper RAII cleanup
  - Trigger dispatch system for ability and item effects
  - Faint detection and processing with automatic reserve substitution
  - Battle phase tracking and refresh mechanisms
- Added comprehensive battle helper functions for party and slot management:
  - Party validation: _anyPartyPokemonNull()_, _hasDuplicatePokemonPointers()_, _healthyPokemonInParty()_
  - Slot assignment: _assignActiveSlots()_, _canTarget()_, _appendSide()_
  - Reserve checking: _hasReserve()_
  - Battle state queries: _getResult()_
- Implemented default and deleted constructors/destructors for _BattleState_ to enable proper object lifecycle management
- Added _getRuntimeRegistry()_ method to all registry configuration classes:
  - Delegates to internal _getRegistry()_ for runtime access to the underlying registry instance
- Created _battleState.cpp_ with default destructor to resolve linker warnings from inline definitions
- Integrated _RegistryProvider_ into _main.cpp_ to demonstrate instantiation and initialization of all registries with a working _BattleEngine_

### Changed

- Extracted `PocketCore::Configuration::` namespace prefix into `using` statement in _multiplierRegistryConfiguration.h_ for cleaner code

### Fixed

- Fixed clang-tidy warnings on _activeSlots()_ in _battleHelpers.cpp_ by wrapping platform-specific code in GCC diagnostic pragmas

## [0.10.2] - 2026-08-13

### Added

- Introduced _SwitchAction_ struct to encapsulate Pokemon switching decisions during battle turns
- Created comprehensive _battleValidation.h_ header with battle state management enums:
  - _BattleEngineError_ - Defines all possible errors during battle execution (invalid switches, faulty moves, invalid targets, etc.)
  - _BattleResult_ - Tracks battle outcome (ongoing, side A won, side B won) for win condition detection
  - _BattlePhase_ - Enumerates battle progression stages (setup, move execution, end-of-turn) for state machine management

### Changed

- Reorganized action structs for improved module separation:
  - Moved _BattleTarget_ and _MoveAction_ from _battleHelpers.h_ to new dedicated _battleAction.h_ header
  - Consolidates all action-related types in single location for better code organization

## [0.10.1] - 2026-08-13

### Changed

- Applied _clang-format_ code style standardization to _fixedMetadataRegistry.benchmark.cpp_ for consistent formatting

### Removed

- Removed the effect registry from _RegistryProvider_:
  - Effect registry is no longer auto-provided; must be explicitly passed to _BattleEngine_
  - Improves separation of concerns between registry management and battle engine initialization
- Removed nullptr effect registry mock instances from all tests that use a mocked _RegistryProvider_:
  - Tests must now provide valid effect registry instances or explicit null handling

## [0.10.0] - 2026-08-12 (Effect Registry Update)

### Added

- Implemented comprehensive effect registry system for extensible battle effect management:
  - Configuration constant `MAX_EFFECTS` to control registry capacity limits
  - Duplicate effect and effect-not-found error codes with proper `errorKindToString()` branch coverage
  - New effect registry header with constructor initializing all builtin effects and query methods
  - Registry provides:
    - Effect metadata lookup (effect function, name, stable ID)
    - Registry queries (effect exists by name or ID, get all effects, get next effect ID)
    - Effect ID to array index mapping for efficient lookup
- Created effect registry configuration classes for runtime effect management:
  - Add/remove effects by name or stable ID
  - Rename existing effects
  - Update effect metadata
  - Comprehensive error handling for duplicate/invalid operations
- Introduced effect handler infrastructure with inline method bindings for _EffectMeta_:
  - Weather effect handlers: _setRainHandler_, _setSandstormHandler_, _setSunHandler_
  - Status handlers: _statusApplyHandler_, _statusRemoveHandler_, _statusTickHandler_, _statusTurnSkipHandler_
  - Action handlers: _flinchHandler_, _psychicTerrainPriorityBlockHandler_, _recoilHandler_
- Integrated effect registry into _RegistryProvider_ for centralized registry access
- Added mock effect registry instances in all test suites using _RegistryProvider_
- Added effect registry initialization examples in _main.cpp_

### Changed

- Renamed all occurrences of _EffectTypeID_ to _BuiltinEffectID_ for clarity and consistency with other builtin ID types

### Removed

- Removed _effectType.h_ header (functionality consolidated into builtin effect ID files)
- Removed _statusChangeEffects()_ utility function (superseded by individual effect handlers)

## [0.9.19] - 2026-08-07

### Changed

- Refined compiler optimization hints by changing _ATTR_PURE_ to _ATTR_CONST_ on non-const overloads:
  - Non-const _activeSlots()_ - Signals method doesn't depend on object state, enabling more aggressive optimizations
  - Non-const _party()_ - Improves const-ness semantics for non-const view accessors

## [0.9.18] - 2026-08-07

### Changed

- Refined compiler optimization hints by changing _ATTR_PURE_ to _ATTR_CONST_ on const overloads:
  - Const _activeSlots()_ - Const view accessor can be marked const for better optimization
  - Const _party()_ - Const view accessor can be marked const for better optimization

## [0.9.17] - 2026-08-07

### Added

- Marked 10 battle helper functions with _ATTR_PURE_ compiler attribute to enable function specialization and optimize common subexpression elimination:
  - Active slot queries: _activeSlots()_ (both const and non-const), _contextSlot()_
  - Party access: _party()_ (both const and non-const)
  - Pokemon status checks: _isHealthy()_ (both const and non-const), _isActive()_, _isAdjacent()_, _targetExists()_
  - Battle analysis: _sideHasHealthyPokemon()_, _getEffectiveSpeed()_, _makeMoveContext()_
  - PURE attribute signals that methods depend only on object state with no side effects

## [0.9.16] - 2026-08-07

### Added

- Introduced fundamental action structures for battle turn resolution:
  - _BattleTarget_ struct - Encapsulates active Pokemon slot selection as effect target with proper identification
  - _MoveAction_ struct - Represents a Pokemon's selected move action for the turn
- Added 5 critical battle helper functions for move and combat analysis:
  - _contextSlot()_ - Maps participant role to their active Pokemon slot index
  - _getEffectiveSpeed()_ - Calculates Pokemon speed considering stat stages and ability effects
  - _makeMoveContext()_ - Constructs move effect context from Pokemon metadata and battle state
  - _applyRecoil()_ - Applies recoil damage to user after move execution
  - _resolveHitCount()_ - Determines actual hit count from move distribution (fixed or weighted)
- Added _mFaintProcessed_ flag to _BattleSlot_ to track which Pokemon have been processed in faint resolution
- Integrated accuracy check into move execution pipeline before triggering move effects
- Created empty _battleEngine.cpp_ source file to support inline header definitions

### Changed

- Applied _clang-format_ code style standardization to 5 registry configuration files for consistent formatting:
  - _abilityRegistryConfiguration.h_, _abilityRegistryConfiguration.cpp_, _itemRegistryConfiguration.h_, _moveRegistryConfiguration.h_, _pokemon.h_
- Upgraded all floating-point calculations to _double_ precision for improved numerical accuracy:
  - Changed _cache.h_ float constants to double
  - Updated base stage, accuracy, and evasion multipliers to double in _cache.h_
  - Updated accuracy calculation in _accuracyCheckHandler.cpp_ to use double
  - Updated attack and defense multiplier calculations in _baseDamageHandler.cpp_ to use double
- Enhanced _applyMultiplier()_ early return logic to normalize results to minimum multiplier of 1.0
- Updated test expectations in _moveMeta.test.cpp_ and _moveRegistry.test.cpp_ for new effect type ID constants

### Fixed

- Fixed damage clamping calculation in _effectContext.test.cpp_ to properly handle edge cases

### Removed

- Removed _mSpeedBoost_ member variable from _BattleSlot_ (speed effects now handled through ability/item triggers)
- Removed 'F' float suffix from accuracy calculations in _accuracyCheckHandler.cpp_ (now using double)
- Removed _AccuracyCheck_ from common effect type ID arrays (no longer a builtin effect)
- Removed unimplemented _protectEffects()_ and _fieldEffectEffects()_ functions with their corresponding test suites

## [0.9.15] - 2026-08-05

### Added

- Added defensive early-return in _applyMultiplier()_ to prevent zero or negative damage:
  - When any active multiplier <= 0, returns zero damage immediately
  - Prevents underflow and ensures non-negative health values
- Added comprehensive test coverage for _Pokemon_ health boundary conditions:
  - Maximum health initialization and validation
  - Minimum health (fainted) edge cases
  - Health overflow/underflow protection verification

### Changed

- Changed move priority variable type from `unsigned char` to `signed char`:
  - Enables proper handling of priority moves with negative priority values
  - Aligns with Pokemon mechanics (e.g., Trick Room reversing priority)

## [0.9.14] - 2026-08-05

### Added

- Implemented comprehensive battle helper functions for Pokemon state queries and side management:
  - Side identification: _getOppositeSide()_, _getSideOrder()_
  - Party access: _activeSlots()_, _party()_ (both const and non-const variants)
  - Individual Pokemon state queries: _isHealthy()_, _isActive()_, _isAdjacent()_
  - Battle state validation: _targetExists()_, _sideHasHealthyPokemon()_
- Unified trigger/target/range identification into shared battle structs:
  - _BattleTriggerID_ - Consolidated trigger identification across abilities, items, and moves
  - _BattleTargetID_ - Consolidated target specification for all effect sources
  - _BattleRangeID_ - Unified range specification for move/ability effects
- Extended _Pokemon_ class with health tracking:
  - Added _mMaxHealth_ member variable for storing maximum HP
  - Implemented getter and setter methods for max health
  - Integrated max health into constructor initialization

### Changed

- Unified trigger identification system by consolidating ability/item/move triggers:
  - Replaced _AbilityTriggerID_, _ItemTriggerID_, _MoveTriggerID_ with unified _BattleTriggerID_
  - Updated all call sites throughout codebase for consistent trigger handling
- Unified target identification system by consolidating ability/item/move targets:
  - Replaced _AbilityTargetID_, _ItemTargetID_, _MoveTargetID_ with unified _BattleTargetID_
  - Enables consistent target resolution across all effect types
- Consolidated range identification:
  - Renamed _MoveRangeID_ to _BattleRangeID_ for clarity and consistency
- Enhanced move metadata:
  - Upgraded move base power variable type from `unsigned char` (0-255) to `unsigned short` for greater range and future extensibility

### Removed

- Removed separate trigger effect structures (now consolidated in unified system):
  - Removed _AbilityEffectTrigger_ (functionality integrated into _BattleTriggerID_)
  - Removed _ItemEffectTrigger_ (functionality integrated into _BattleTriggerID_)
  - Removed _MoveEffectTrigger_ (functionality integrated into _BattleTriggerID_)
- Deleted separate target/trigger headers (consolidated into battle-level headers):
  - Deleted _abilityTargetsAndTriggers.h_
  - Deleted _itemTargetsAndTriggers.h_
  - Deleted _moveTargetsAndTriggers.h_
- Rationale: Unification eliminates code duplication and simplifies trigger/target routing in battle engine

## [0.9.13] - 2026-08-04

### Added

- Implemented effect suppression system for complex ability/item interactions:
  - Added _MAX_SUPPRESSION_RULES_PER_TARGET_ configuration constant to control rule limits
  - Created _SuppressionRule_ struct encapsulating per-target suppression logic with optional ability, item, or move ID constraints
  - Extended _AbilityEffectTrigger_, _ItemEffectTrigger_, _MoveMoveEffectTrigger_ with:
    - Array of suppression rules for each effect
    - Active suppression rule count tracking
- Moved effect trigger structures to their respective metadata headers for better organization:
  - _AbilityEffectTrigger_ relocated from _abilityTargetsAndTriggers.h_ to _abilityMeta.h_
  - _ItemEffectTrigger_ relocated from _itemTargetsAndTriggers.h_ to _itemMeta.h_
  - _MoveEffectTrigger_ relocated from _moveTargetsAndTriggers.h_ to _moveMeta.h_
- Relocated _EffectSource_ enum from _effectContext.h_ to dedicated _effectSourceAndSuppression.h_ header

### Changed

- Applied _clang-format_ code style standardization to _typeRegistryConfiguration.cpp_

## [0.9.12] - 2026-08-04

### Added

- Added _OnSuccessfulHit_ trigger type to _AbilityTriggerID_:
  - Allows abilities to trigger only when move successfully hits (post-accuracy check)
  - Enables hit-conditional effects like contact-damage abilities

### Changed

- Refactored Flinch ability trigger mechanics:
  - Changed from _OnDamageCalc_ to _OnSuccessfulHit_ to match Pokemon game mechanics
  - Flinch abilities now only trigger after confirmed hit (skipping damage calc entirely)
  - Updated corresponding test expectations for new trigger behavior

## [0.9.11] - 2026-08-04

### Added

- Implemented multi-status Pokemon support system:
  - Configuration constant _MAX_STATUSES_ controlling maximum simultaneous statuses per Pokemon
  - Added _getStatuses()_ accessor returning span of all current status ailments
  - Added _addStatus()_ method for applying new status with interaction handling
  - Added _ATTR_NOINLINE_ to _addBuiltin()_ to prevent inlining of bulk insertion operations
  - Integrated _CheriBerry_ and _ChestoBerry_ items as consumable status cures
- Implemented comprehensive status interaction system:
  - Status interaction metadata in each _StatusMeta_ entry with per-status interaction rules
  - _StatusInteractionAction_ enum defining interaction behavior (block, replace, stack)
  - Status interaction resolution functions:
    - _hasInteraction()_ - Checks if status pair has defined interaction
    - _willBlockIncoming()_ - Determines if incoming status is blocked
    - _statusAlreadyExists()_ - Validates duplicate status check
    - _statusReplaceHandler()_ - Handles status replacement logic
    - _statusRemoveHandler()_ - Handles status removal and interaction cleanup
    - _shiftAndGetNextAvailableStatus()_ - Compacts status array after removal
- Added comprehensive multi-status test coverage in _pokemon.test.cpp_

### Changed

- Reorganized _BattleSlot_ member variables for improved cache locality and maintainability
- Renamed _mForceGrounded_ to _mIsGrounded_ throughout codebase for clarity (all call sites updated)
- Upgraded _Pokemon_ to store array of statuses instead of single status:
  - Changed from `StatusID mStatus` to `StatusID mStatuses[MAX_STATUSES]`
  - Updated all status queries and modifications to work with array
- Refactored _getStatusID()_ to accept array index parameter for multi-status access
- Updated _burnDamageHandler.cpp_ to use new status helper functions for proper status array iteration

### Removed

- Removed _mItemConsumed_ flag from _BattleSlot_ (item consumption now tracked separately in effect context)
- Removed _setStatus()_ method (replaced by _addStatus()_ with interaction handling)

## [0.9.10] - 2026-08-04

### Added

- Added three new ability types to _builtInAbilityID_:
  - _Levitate_ - Negates ground-type immunity interactions
  - _Elevate_ - Variant levitation for specific interactions
- Added _AirBalloon_ item to _builtInItemID_ for held item grounding mechanics
- Added _mIsGrounded_ flag to _BattleSlot_ to track whether Pokemon is grounded (ignores Levitate/AirBalloon)
- Implemented terrain-based damage multiplier configuration constants:
  - _ELECTRIC_TERRAIN_BUFF_VALUE_ - Electric terrain base damage boost
  - _GRASSY_TERRAIN_BUFF_VALUE_ - Grassy terrain base damage boost
  - _PSYCHIC_TERRAIN_BUFF_VALUE_ - Psychic terrain base damage boost
  - _MISTY_TERRAIN_DRAGON_DEBUFF_VALUE_ - Misty terrain dragon move reduction
- Expanded _effectHandlerHelpers.h_ with 7 battle slot classification functions:
  - Type checking: _battleSlotHasType()_
  - Ability checking: _battleSlotHasAbilityByName()_, _battleSlotHasAbilityByID()_
  - Item checking: _battleSlotHasItemByName()_, _battleSlotHasItemByID()_
  - Grounding status: _isBattleSlotUngrounded()_, _isBattleSlotGrounded()_
- Added const battle slot accessors:
  - _getConstUserBattleSlot()_ - Read-only access to user's active Pokemon slot
  - _getConstTargetBattleSlot()_ - Read-only access to target's active Pokemon slot
- Implemented terrain damage handler (_TerrainHandler_) inheriting from _IEffectHandler_:
  - Calculates terrain-based damage modifiers according to Bulbapedia specifications
  - Integrates with effect system for automatic terrain bonus application
- Added comprehensive test suite for _terrainHandler.h_

### Changed

- Converted Ability constants to inline declarations for consistency with other registries
- Upgraded all _DamageFormulaModifiers_ floating-point members from _float_ to _double_ for increased precision
- Removed const qualification from _BattleState_ parameters in effect handler _apply()_ functions:
  - Handlers now receive mutable battle state for conditional state modifications
- Refactored _getTeamConst()_ to accept _BattleState_ by mutable reference for better flexibility
- Updated all intermediate calculations in _baseDamageHandler.cpp_ from _float_ to _double_ precision
- Refactored effect handlers to call const method variants where appropriate:
  - _accuracyCheckHandler.cpp_, _baseDamageHandler.cpp_, _stabHandler.cpp_, _typeEffectivenessHandler.cpp_

## [0.9.9] - 2026-08-04

### Fixed

- Added missing `#include` directive in _weatherHandler.cpp_ to resolve compilation errors

## [0.9.8] - 2026-08-04

### Added

- Added two air-based immunity abilities to _builtInAbilityID_:
  - _AirLock_ - Ability that disables weather effects
  - _CloudNine_ - Variant air immunity ability
- Added _HydroSteam_ to _builtInMoveID_ for weather-responsive water move
- Implemented _cloneMetadata()_ method to prevent compiler inlining warnings on metadata copies
- Expanded configuration constants for weather and multi-hit mechanics:
  - First hit multiplier for Population Bomb (initial damage)
  - Consecutive hit multiplier for Population Bomb (subsequent hits)
  - Weather nullification multiplier (weather ability override)
  - Water move effectiveness in rain (1.5x boost)
  - Fire move penalties in rain (0.5x reduction)
  - Fire move bonuses in harsh sunlight (1.5x boost)
  - Water move penalties in harsh sunlight (0.5x reduction)
- Implemented PopulationBombHandler inheriting from _IEffectHandler_:
  - Calculates accurate population bomb damage from bulbapedia specifications
  - Handles variable hit count mechanics per Bulbapedia accuracy
- Implemented WeatherHandler inheriting from _IEffectHandler_:
  - Calculates weather-based damage modifiers for all weather types
  - Integrates with effect system for automatic weather bonus/penalty application
- Added MoveHitPolicy framework for multi-hit move resolution:
  - Differentiates between fixed hit counts (e.g., Double Hit = 2) and weighted distributions (e.g., Population Bomb = 2-5)
  - Provides extensible policy interface for custom hit count behaviors
- Added three new move trigger types to _MoveTriggerID_:
  - _BeforeHit_ - Triggers before accuracy check (for ability/item interactions)
  - _OnHit_ - Triggers after successful hit confirmation
  - _AfterHit_ - Triggers after all hit effects resolve
- Added _PopulationBomb_ and updated _Targets_ multiplier definitions with user-readable names
- Integrated population bomb and targets multiplier builtin entries

### Changed

- Renamed weather and terrain ID variables in _BattleState_ with explicit _ID_ suffix for consistency
- Updated _moveMeta.h_ documentation to reflect new move trigger types and hit policies
- Refactored move hit distribution from raw array to structured _MoveHitPolicy_ for better type safety
- Updated all builtin move triggers from deprecated _OnTarget_ to _OnHit_ for consistency
- Updated corresponding move trigger test expectations
- Renamed _Types_ enum to _BuiltInTypeID_ for consistency with other builtin ID types
- Updated all type enum references throughout codebase
- Refactored _moveRegistryConfiguration.cpp_ to use explicit member initialization instead of copy construction

### Fixed

- Renamed _mCurrentHit_ variable in _EffectContext_ to _mCurrentHitAttempt_ for clarity
- Fixed clang-tidy warnings in _randomizationHandler.cpp_ by adding appropriate diagnostic pragmas
- Added missing `#include` directive in _random.test.cpp_

### Removed

- Removed _mTotalHitCount_ from _EffectContext_ (now derived from MoveHitPolicy)
- Removed deprecated _OnTarget_ move trigger ID (replaced by _OnHit_)

## [0.9.7] - 2026-08-03

### Added

- Implemented multi-hit move tracking in _EffectContext_:
  - _mTotalHitCount_ - Total number of hits for the current move execution
  - _mCurrentHit_ - Current hit number being processed
- Added _mHitDistribution_ to _MoveMeta_ for flexible hit count specification (fixed or weighted distribution)
- Implemented _getRandomDouble()_ method for decimal-precision random number generation:
  - Enables accurate probability-based move mechanics (e.g., hit variance)
- Added comprehensive test coverage for decimal random number generation

### Changed

- Applied brace initialization to _mTriggers_ member in _MoveMeta_ for consistency with modern C++ practices

## [0.9.6] - 2026-08-03

### Added

- Implemented randomization effects configuration constants:
  - Randomization minimum roll value (1/255)
  - Randomization maximum roll value (240/255)
- Added _Randomization_ multiplier to _builtInMultiplierID_ for damage variance
- Added user-readable name for Randomization multiplier
- Integrated randomization multiplier into builtin multiplier registry
- Implemented BurnDamageHandler inheriting from _IEffectHandler_:
  - Calculates burn-reduced damage according to Bulbapedia specifications (0.5x damage)
  - Applies only to physical moves and works with abilities like Guts

### Changed

- Standardized multiplier constant naming by adding _VALUE_ suffix:
  - _CRITICAL_HIT_MULTIPLIER_ → _CRITICAL_HIT_MULTIPLIER_VALUE_
  - _STAB_HIT_MULTIPLIER_ → _STAB_HIT_MULTIPLIER_VALUE_
  - _TARGETS_HIT_MULTIPLIER_ → _TARGETS_HIT_MULTIPLIER_VALUE_
  - Updated all call sites throughout codebase

### Removed

- Removed unused _ATTR_MAYBE_UNUSED_ and _attributeMacros.h_ from:
  - _typeEffectivenessHandler.h_, _typeEffectivenessHandler.cpp_
  - Macros were unnecessary for these implementations

## [0.9.5] - 2026-08-03

### Added

- Added _Guts_ ability to _builtInAbilityID_ with user-readable name:
  - Ability allowing burned/paralyzed/poisoned Pokemon to attack at normal power
- Added _Facade_ move to _builtInMoveID_:
  - Status-responsive move with bonus power when user has status ailment
- Added _Burn_ multiplier to _builtInMultiplierID_ with user-readable name
- Integrated builtin burn multiplier into registry
- Added configuration constant for burn damage reduction factor (0.5x):
  - Specifies how much burn status reduces move damage
- Implemented BurnDamageHandler inheriting from _IEffectHandler_:
  - Calculates burn-adjusted damage for moves affected by burn status
  - Follows Bulbapedia damage calculation specifications

## [0.9.4] - 2026-08-03

### Added

- Implemented type effectiveness multiplier constants for damage calculation:
  - Not Very Effective (NVE) effectiveness value (0.5x)
  - Effective (E) effectiveness value (1.0x)
  - Super Effective (SE) effectiveness value (2.0x)
  - No Effect (NE) effectiveness value (0.0x)
- Added _getEffectivenessValue()_ function to convert _TypeEffectiveness_ enum to corresponding multiplier constants

### Changed

- Upgraded all floating-point precision from _float_ to _double_ for improved numerical accuracy:
  - Configuration constants: _BASE_MULTIPLIER_VALUE_, _CRITICAL_HIT_MULTIPLIER_VALUE_, _STAB_HIT_MULTIPLIER_VALUE_, _TARGETS_HIT_MULTIPLIER_VALUE_, _FIXED_POINT_MULTIPLIER_NUMERATOR_, _FIXED_POINT_MULTIPLIER_DENOMINATOR_
  - _EffectContext_ methods and members:
    - Changed _setMultiplier()_ parameter to double
    - Changed _getActiveMultiplier()_ return type to double
    - Upgraded _mActiveMultipliers_ array to store double values
- Refactored _typeEffectivenessHandler.cpp_ to use _getEffectivenessValue()_ function instead of commented-out inline code

### Fixed

- Added missing import directives to:
  - _typeRegistryConfiguration.cpp_
  - _typeRegistryConfiguration.test.cpp_
  - _typeRegistry.test.cpp_

### Removed

- Removed float suffix 'F' from floating-point literals in test files:
  - _effectContext.test.cpp_, _criticalHitHandler.test.cpp_, _stabHandler.test.cpp_, _targetsHandler.test.cpp_
  - Unnecessary when using double precision

## [0.9.3] - 2026-08-03

### Changed

- Refactored type effectiveness system to use enum values directly instead of registry IDs:
  - Updated all references from _TypeEffectivenessID_ to _TypeEffectiveness_ enum
  - Simplifies type effectiveness queries by eliminating indirection through ID lookups
  - Updated files: _typeRegistryConfiguration.h/cpp/test.cpp_, _typeRegistry.h/test.cpp_, _Types/constants.h_
- Renamed header file _builtInTypeEffectivenessID.h_ to _typeEffectiveness.h_ for clarity
- Updated all import directives to reference new header name

### Removed

- Removed entire type effectiveness registry system:
  - Type effectiveness registry class and implementations
  - Type effectiveness registry configuration files
  - _MAX_TYPE_EFFECTIVENESS_ configuration constant (no longer needed)
  - Type effectiveness multiplier value from multiplier system
  - Builtin type effectiveness multiplier entry
  - Type effectiveness metadata files
  - Type effectiveness provider from _RegistryProvider_
  - Type effectiveness dependencies from all test suites:
    - _registryProvider.h_ and handler test files
- Rationale: Type effectiveness is now a fixed enum, eliminating need for runtime registry

## [0.9.2] - 2026-08-03

### Added

- Implemented type effectiveness registry for extensible type matchup system:
  - Configuration constant _MAX_TYPE_EFFECTIVENESS_ controlling registry capacity
  - Error codes for duplicate type effectiveness and type effectiveness not found with _errorKindToString()_ support
  - Type effectiveness registry class with constructor initializing all Pokemon-standard type matchups
  - Registry provides:
    - Type effectiveness metadata lookup (name, stable ID)
    - Registry queries (type effectiveness exists by name or ID, get all entries, get next ID)
    - ID to array index mapping for efficient lookup
- Created type effectiveness registry configuration classes for runtime type matchup management:
  - Add/remove/rename type effectiveness by name or stable ID
  - Update type effectiveness metadata
  - Comprehensive error handling
- Added _TypeEffectiveness_ multiplier value to configuration constants
- Integrated new builtin type effectiveness multiplier into multiplier registry
- Added type effectiveness provider to _RegistryProvider_ for centralized access
- Integrated type effectiveness provider into all handler test suites:
  - _registryProvider.h_, _accuracyCheckHandler.test.cpp_, _baseDamageHandler.test.cpp_, _criticalHitHandler.test.cpp_, _stabHandler.test.cpp_
  - _targetsHandler.test.cpp_
- Add builtin _TypeEffectivenessID_ values to _Types/constants.h_
- Added a metadata file for type effectiveness that holds the relevant:
  - User defined name, stable ID, and the effectiveness value
- Added a source and header type effectiveness handler which inherits from _IEffectHandler_ that will calculate the type effectiveness part of the damage calculation from Bulbapedia.

### Changed

- Standardized builtin ID header naming convention with camelCase ("builtin" → "builtIn") for consistency:
  - _builtinAbilityID.h_ → _builtInAbilityID.h_ with updated imports throughout
  - _builtinMultiplierID.h_ → _builtInMultiplierID.h_ with updated imports throughout
  - _builtinItemID.h_ → _builtInItemID.h_ with updated imports throughout
  - _builtinMoveID.h_ → _builtInMoveID.h_ with updated imports throughout
  - _builtinStatusID.h_ → _builtInStatusID.h_ with updated imports throughout
  - _builtinTerrainID.h_ → _builtInTerrainID.h_ with updated imports throughout
  - _builtinWeatherID.h_ → _builtInWeatherID.h_ with updated imports throughout
  - Rationale: Enforces consistent naming style across all builtin identifier headers
- Refactored type identification system for improved organization:
  - Renamed _types.h_ to _builtInTypeID.h_ to align with builtin ID naming convention
  - Updated all imports to reference new header name
  - Renamed _typeEffectiveness.h_ to _builtInTypeEffectivenessID.h_ for consistency
  - Updated all imports accordingly
- Enhanced type-related headers with improved structure:
  - Updated header guard in _typeID.h_ for consistency
  - Renamed namespace/detail types: _Detail::TypeIDTag_ → _Detail::TypeID_
  - Relocated _toTypeID()_ function from _typeID.h_ to _builtInTypeID.h_ for better organization
  - Consolidates type ID conversion logic with type constants
- Refactored type effectiveness usage throughout type system:
  - Updated following files to use _TypeEffectivenessID_ instead of _TypeEffectiveness_:
    - _typeRegistryConfiguration.h/cpp/test.cpp_
    - _typeRegistry.h/test.cpp_
    - _Types/constants.h_

### Removed

- Removed ccache compiler cache tool from build infrastructure:
  - Removed from CI/CD workflows: _codeql-analysis.yml_, _testing.yml_
  - Removed from build system: _Makefile_, _makefileDependencies.sh_
  - Rationale: Simplifies build pipeline and reduces caching complexity
- Removed unused import directives from _typeID.h_
  - Cleanup following type system refactoring

## [0.9.1] - 2026-08-03

### Added

- Comprehensive documentation additions to core registry infrastructure:
  - Index-related documentation: _IDIndexEntry_ struct and member variables, _findEntryIndexByName()_, _findEntryIndexByID()_, _insertIDIndex()_, _removeIDIndex()_, _idIndexEntryLess()_, _rebuildIDIndex()_
  - Registry documentation: _FixedMetadataRegistry_ member variables explaining state tracking and ID management
  - Provider documentation: _RegistryProvider_ class and member variables explaining registry access patterns
  - Type-related documentation: _Types_ enum and identifiers

### Changed

- Modernized documentation format across core headers by converting from old Doxygen syntax (_\\_ comments) to modern format (_@_ comments):
  - Applied consistently to: _attributeMacros.h_, _configCat.h_, _types.h_, _contiguousSequence.h_, _overflowProtection.h_
  - Improves documentation consistency and readability throughout codebase

## [0.9.0] - 2026-08-03 (Type Registry with Fixed Metadata Registry Update)

### Added

- Extended _fixedMetadataRegistryConfiguration.h_ with read-only registry access patterns:
  - Mutable registry access for runtime modifications
  - Const read-only access for safe multi-threaded queries
- Added template variable to _FixedMetadataRegistry_ for flexible member name handling:
  - _NameMember_ template parameter enabling type-specific naming conventions

### Changed

- Applied performance optimization attributes to frequently-called registry functions:
  - _ATTR_PURE_ attribute to _statStageCacheIndex()_, _findIndexByID()_, _StableID findEntryIndexByName()_, _getActiveMultipliers()_ functions
  - _ATTR_NOINLINE_ attribute to prevent unwanted inlining: _eraseEntry()_, _string_view findEntryIndexByName()_, _TypeRegistry()_ constructor, _span addBuiltin()_
- Refactored _TypeEntry_ to use member variable prefixes (_m_ prefix) for consistency:
  - Updated all call sites throughout codebase to use new naming convention
- Optimized _IDIndexEntry_ lookup functions by caching indexed entries:
  - _insertIDIndex()_, _removeIDIndex()_, _rebuildIDIndex()_ now call _getIndexedEntries()_ once instead of accessing _mIDIndex_ multiple times
- Comprehensive refactoring of type registry integration with fixed metadata base:
  - _typeRegistryConfiguration.h_ methods now delegate to _fixedMetadataRegistryConfiguration.h_ base implementations (delegating patterns for all getter/setter methods)
  - _typeRegistry.h_ methods now delegate to _fixedMetadataRegistry.h_ base for memory/lookup consistency
- Enhanced type registry architecture:
  - Refactored _clearRows()_ to use _getRegistry()_ accessor instead of direct member access
  - Updated _typeRegistryConfiguration.cpp_ to use _getRegistry()_ for consistent registry access patterns
- Applied code formatting and consistency passes:
  - Ran _clang-format_ on 10+ registry and handler files for style consistency
  - Fixed _clang-tidy_ warnings in 23 source and test files
  - Extracted namespace prefixes into `using` statements for 7 registry configuration headers
- Updated _getName()_ method to return templated _NameMember_ instead of hardcoded _mName_ for flexibility

### Removed

- Removed member registry variable from _typeRegistryConfiguration.h_ (now uses inherited base implementation):
  - Eliminates duplication and improves memory efficiency
- Removed redundant functions from _typeRegistry.h_ that are now inherited from base:
  - _findEntryIndexByName()_, _findEntryIndexByID()_
  - Member variables: _mEntries_, _mAmountRegistered_, _mNextTypeID_

## [0.8.7] - 2026-08-03

### Added

- Enhanced build system infrastructure:
  - Added warnings variable to _Makefile_ for explicit warning control
  - Excluded _main.cpp_ from benchmark compilation targets
  - Implemented performance benchmarks for core systems:
    - Multiplier functionality performance in _EffectContext_
    - Combined _EffectTypeID_ function performance
    - Move ID lookup performance at registry full capacity
- Optimized stat stage caching with helper function:
  - Added _statStageCacheIndex()_ for centralized cache index calculation
  - Used by stat multiplier cache functions (_STAT_STAGE_MULTIPLIERS_, _ACCURACY_STAGE_MULTIPLIERS_, _EVASION_STAGE_MULTIPLIERS_)
- Enhanced _EffectContext_ multiplier system:
  - Added read-only span accessor for all active multipliers
  - Added array mapping builtin multiplier values to sparse vector positions (enables O(1) lookup for builtin multipliers)
  - Added battle context helpers: _getUserBattleSlot()_, _getTargetBattleSlot()_ accessors
- Optimized move effect functions in _moveMeta.h_:
  - Applied _ATTR_CONST_ attribute to common _EffectTypeID_ query functions
  - Changed return types from vectors to non-owning read-only spans for efficiency
  - Added _noexcept_ specifications
- Implemented registry erasure functionality:
  - Added _eraseEntry()_ to _fixedMetadataRegistry.h_ base class
  - Propagated to all registry types: _abilityRegistry.h_, _itemRegistry.h_, _moveRegistry.h_, _multiplierRegistry.h_, _statusRegistry.h_, _terrainRegistry.h_, _weatherRegistry.h_
- Introduced stable ID index system for O(log n) lookups:
  - _IDIndexEntry_ struct containing stable ID and entry position
  - Sorted stable ID index array with entry count tracking
  - _insertIDIndex()_ - Inserts ID-index mapping while preserving sorted order
  - _removeIDIndex()_ - Removes ID-index mapping with compaction
  - _idIndexEntryLess()_ - Comparator ordering by stable ID then entry index
  - _rebuildIDIndex()_ - Rebuilds and resorts index from registered entries
- Added code coverage exclusions:
  - _lcov_ pragmas in _typeRegistryConfiguration.cpp_ for unreachable branches
- Added defensive validation:
  - Attack/defense stat infinity and negativity checks in _baseDamageHandler.cpp_

### Changed

- Unified benchmark warning management:
  - Updated benchmark warnings to use explicit benchmark-specific flags instead of mixed compiler warnings
- Refactored stat multiplier cache usage:
  - Updated _STAT_STAGE_MULTIPLIERS_, _ACCURACY_STAGE_MULTIPLIERS_, _EVASION_STAGE_MULTIPLIERS_ to call _statStageCacheIndex()_ instead of repeating logic
- Fixed critical hit calculation:
  - Changed _MAX_CRITICAL_HIT_VALUE_ from 100 to 99 to account for 0-based counting
  - Ensures accurate critical hit percentage calculations
- Simplified registry entry removal:
  - Refactored _removeEntry()_ to delegate to new _eraseEntry()_ function
- Updated _fixedMetadataRegistry.h_ documentation for clarity
- Enhanced ID index integration:
  - _incrementAmountRegistered()_ now inserts ID index entry
  - _decrementAmountRegistered()_ now removes ID index entry
  - _addBuiltin()_ now calls _incrementAmountRegistered()_ instead of manual updates
  - _findEntryIndexByID()_ now performs binary-search on sorted ID index (O(log n) instead of O(n))
  - _setEntry()_ and _setAmountRegistered()_ trigger ID index rebuilds
- Optimized move registry initialization:
  - Refactored _moveRegistry.h_ effect initialization to use iterator begin/end instead of explicit loops
- Improved function early returns:
  - Updated both functions in _contiguousSequence.h_ with early exit patterns
  - Updated _resetMatchups()_ unexpected return handling
- Enhanced multiplier processing performance:
  - _setMultiplier()_ now achieves O(1) for builtin multipliers using mapped positions (falls back to O(n) for others)
  - _resetMultipliers()_ fills builtin multiplier positions with 0
  - Updated float typing to double in _baseDamageHandler.cpp_ for precision
- Refactored handler slot access:
  - Updated handlers to use battle slot helper functions from _IEffectHandler_:
    - _accuracyCheckHandler.cpp_, _baseDamageHandler.cpp_, _stabHandler.cpp_
  - Improves code consistency and reusability
- Applied code formatting:
  - Ran _clang-format_ on _accuracyCheckHandler.cpp_, _baseDamageHandler.cpp_
  - Updated includes in _moveMeta.cpp_
- Improved test coverage:
  - Updated 7 test files to fully cover all lines and branches:
    - Registry configurations and type system tests
    - Utility and logger tests
  - Added comprehensive test suites for 15 new components:
    - Registry configurations (multiplier, status, terrain, weather)
    - Effect system (context, handlers, interface)
    - Utilities (input, random, move metadata)

### Fixed

- Fixed registry transaction rollback behavior:
  - _addMetadataBatch()_ now reverts registry to pre-call state if any addition fails
  - _setMatchupRow()_ now reverts registry to pre-call state if any addition fails
  - _setDefensiveColumn()_ now reverts registry to pre-call state if any addition fails
  - Ensures atomic registry operations with all-or-nothing semantics
- Fixed name uniqueness validation:
  - Both _mutateMetadata()_ calls now prevent updates where new name already exists in registry
- Fixed boundary condition bugs:
  - _findEntryIndexByID()_ and _findEntryIndexByName()_ asserts changed from `<` to `<=` for correct boundary checking
- Fixed template type handling:
  - Removed redundant _T::value_type_ usage in _input.h_
- Fixed damage calculation formula:
  - Corrected _applyMultiplier()_ damage calculation logic
  - Implemented damage clamping in _baseDamageHandler.cpp_ to range [1.0, USHORT_MAX]
- Fixed critical hit percentage check:
  - Changed comparison from `<=` to `<` for accurate percentage-based calculation
  - Prevents off-by-one errors in critical hit probability

### Removed

- Removed redundant includes from _Configuration/constants.h_

## [0.8.6] - 2026-07-31

### Removed

- Removed _EffectResult_ struct and associated member variable from _EffectContext_:
  - Simplifies effect execution model
- Removed `if` protected scope in _DamageContext_
  - Clarifies conditional logic structure

## [0.8.5] - 2026-07-31

### Added

- Implemented stat damage modifiers system in _DamageFormulaModifiers_ struct:
  - Separate multipliers for each of the 6 base stats (HP, ATK, DEF, SpA, SpD, SPE)
  - Integrated into each _BattleSlot_ for per-Pokemon damage adjustments
- Applied damage formula modifiers in _baseDamageHandler_:
  - Uses _DamageFormulaModifiers_ to adjust attack, special attack, defense, and special defense calculations
  - Enables stat-stage interactions with base damage formula

### Changed

- Enhanced _EffectContext_ initialization:
  - Default-initialized sparse multiplier list for consistent starting state

## [0.8.4] - 2026-07-31

### Changed

- Optimized _Pokemon_ class with compile-time evaluation:
  - Converted setters to `constexpr` and moved implementations into _pokemon.h_
  - Enables compile-time constant Pokemon creation
- Updated test construction patterns:
  - Modified _pokemon.test.cpp_ to include Pokemon level when constructing test instances

### Removed

- Removed _pokemon.cpp_ implementation file:
  - All functionality moved to header for constexpr support
  - Reduces build artifacts and simplifies compilation

## [0.8.3] - 2026-07-31

### Added

- Implemented fixed-point multiplier system for precise damage calculation:
  - Configuration constants:
    - _FIXED_POINT_MULTIPLIER_NUMERATOR_ - Numerator for fixed-point calculation
    - _FIXED_POINT_MULTIPLIER_DENOMINATOR_ - Denominator for fixed-point calculation
    - _FIXED_POINT_ROUNDING_THRESHOLD_ - When to round down
    - _FIXED_POINT_TOLERANCE_THRESHOLD_ - Double precision rounding tolerance
  - Added _applyMultiplier()_ function to _EffectContext_:
    - Processes all damage multipliers through fixed-point arithmetic
    - Multiplies result with base damage for final calculation
- Integrated weather damage mechanics:
  - Added _Weather_ multiplier ID to _builtInMultiplierID_
  - Added user-readable name for weather multiplier
  - Integrated builtin weather multiplier into registry
- Enhanced _Pokemon_ level tracking:
  - Added level parameter to _Pokemon_ constructors
  - Calls _setLevel()_ in constructor body for initialization

### Removed

- Removed combined multiplier variable from _EffectContext_:
  - No longer needed with fixed-point multiplier system
  - Eliminates redundant state tracking
- Removed combined multiplier usage from _setMultiplier()_ and _resetMultipliers()_
  - Refactored to work with individual multiplier values

## [0.8.2] - 2026-07-30

### Added

- Implemented comprehensive damage calculation foundation with stat stage and level caching:
  - Configuration constants for stat calculations:
    - Evasion stage multiplier numerator and denominator for dodge probability scaling
    - Level damage factor numerator, denominator, and offset
    - Maximum stages for stat stage multiplier caching
    - Cache size constants based on stat stage counts
  - Stat stage multiplier cache files for O(1) lookup:
    - Base stat stage cache (HP, ATK, DEF, SpA, SpD, SPE)
    - Accuracy stage multiplier cache
    - Evasion stage multiplier cache
- Implemented central registry provider:
  - _RegistryProvider_ struct holding non-owning pointers to all registries
  - Enables consistent registry access across effect handlers
- Extended _EffectContext_ multiplier system:
  - _setMultiplier()_ method to update active damage multiplier
  - _resetMultipliers()_ method to clear all active multipliers
  - Combined multiplier float for final damage adjustment
- Enhanced effect handler architecture:
  - Updated 6 handlers to accept _RegistryProvider_ parameter (source and header):
    - _effectHandlerInterface_, _accuracyCheckHandler_, _baseDamageHandler_, _criticalHitHandler_, _stabHandler_, _targetsHandler_
  - Enables handlers to query registry data at runtime
- Added _OnTarget_ move trigger ID for target-specific effect triggers
- Enhanced _Pokemon_ class with level caching:
  - Added level damage factor pseudo-cache for efficiency
  - Implemented getter method for cached value

### Changed

- Refactored _BattleSlot_ party member type:
  - Changed from `const Pokemon *` to `Pokemon *` for mutability
- Replaced multiplier storage mechanism in _EffectContext_:
  - Changed from unordered_map to sparse multiplier vector for predictable performance
- Updated move trigger assignments:
  - Changed _Pound_ and _Karate Chop_ from _OnUse_ to _OnTarget_ trigger
- Optimized handler performance with caching:
  - _accuracyCheckHandler_ now uses cached evasion and accuracy stage multipliers instead of recalculating
  - _baseDamageHandler_ now uses cached stat stage multipliers and pseudo-cached level factor
  - Eliminates redundant calculations on every handler invocation
- Unified multiplier updates across handlers:
  - _criticalHitHandler_, _stabHandler_, _targetsHandler_ now call _setMultiplier()_ instead of direct map access
- Refactored _targetsHandler_:
  - Now uses _RegistryProvider_ instead of storing _MoveRegistryConfiguration_ member
- Updated _MoveRegistry_ test expectations:
  - _Pound_ move now checks for _OnTarget_ trigger

### Removed

- Removed current HP tracking from _BattleSlot_ (derived from _Pokemon_ object)
- Removed _EffectContext_ constructor that base-initialized multiplier map
- Removed _MoveRegistryConfiguration_ member variable from _TargetsHandler_ (now uses provider)

## [0.8.1] - 2026-07-30

### Added

- Implemented multiplier registry system for flexible damage calculation modifiers:
  - Configuration constants:
    - _MAX_MULTIPLIERS_ - Registry capacity limit
    - _BASE_MULTIPLIER_VALUE_ - Default multiplier (1.0)
    - _STAB_HIT_MULTIPLIER_ - Same-Type Attack Bonus multiplier (1.5)
    - _TARGETS_HIT_MULTIPLIER_ - Multi-target penalty multiplier (0.75)
  - Error codes for duplicate multiplier and multiplier not found with _errorKindToString()_ support
  - Multiplier registry class with constructor initializing all builtin multipliers
  - Registry provides:
    - Multiplier metadata lookup (name, stable ID)
    - Registry queries (multiplier exists by name or ID, get all entries, get next ID)
    - ID to array index mapping for efficient lookup
- Created multiplier registry configuration classes:
  - Add/remove/rename multipliers by name or stable ID
  - Update multiplier metadata
  - Comprehensive error handling
- Added _EffectContext_ constructor:
  - Initializes multiplier list with _BASE_MULTIPLIER_VALUE_
- Extended effect type system:
  - Added _Targets_ and _STAB_ to _effectType.h_ and _moveMeta.cpp_
  - New effect types for multi-target and same-type attack mechanics
- Implemented effect handlers:
  - STAB (Same-Type Attack Bonus) handler calculating type bonus per Bulbapedia
  - Targets handler calculating multi-target damage reduction
  - Both inherit from _IEffectHandler_ for consistent effect integration
- Added type system infrastructure:
  - Hashing override for _IDInterface_ for container support
  - Builtin Multiplier ID file with conversion function
  - _Multiplier/constants.h_ containing user-readable names
  - Multiplier metadata file
- Enhanced _Pokemon_ class:
  - Added stable status ID with getter and setter
  - Added _isFainted()_ member function
- Added missing source file implementations:
  - _terrainRegistryConfiguration.cpp_, _weatherRegistryConfiguration.cpp_
- Integrated multiplier registry into _main.cpp_

### Changed

- Removed const from _BattleSlot_ party member for mutability
- Refactored _EffectContext_ from per-multiplier tracking to stable multiplier ID pattern
- Reordered _EffectTypeID_ enum values for optimization
- Updated _Status/constants.h_ documentation
- Refactored _criticalHitHandler.cpp_ to use builtin multiplier ID dictionary
- Updated _moveRegistry.test.cpp_:
  - Changed expected effect count for _Pound_ from 9 to 11 effects

### Removed

- Removed item ID, status ID, and fainted status from _BattleSlot_
  - These are now accessed through _Pokemon_ object instead

## [0.8.0] - 2026-07-30 (Weather and Terrain Registry Update)

### Added

- Implemented weather and terrain registry systems for environmental battle mechanics:
  - Configuration constants:
    - _MAX_WEATHERS_ - Weather registry capacity
    - _MAX_TERRAINS_ - Terrain registry capacity
  - Error codes for duplicate weather/terrain and not found conditions with _errorKindToString()_ support
  - Weather registry class with constructor initializing all Pokemon-standard weathers
  - Terrain registry class with constructor initializing all Pokemon-standard terrains
  - Both registries provide:
    - Metadata lookup (name, stable ID)
    - Registry queries (exists by name/ID, get all entries, get next ID)
    - ID to array index mapping
- Created weather and terrain registry configuration classes:
  - Add/remove/rename weather/terrain by name or stable ID
  - Update weather/terrain metadata
  - Comprehensive error handling
- Added type system infrastructure for weathers and terrains:
  - Builtin Weather ID file with conversion function to stable Weather ID
  - Builtin Terrain ID file with conversion function to stable Terrain ID
  - _Weather/constants.h_ with user-readable weather names
  - _Terrain/constants.h_ with user-readable terrain names
  - Metadata files for weather and terrain persistence
- Integrated weather and terrain registries into _main.cpp_

### Changed

- Refactored _BattleState_ to track stable weather and terrain IDs instead of full objects:
  - Reduces memory footprint and simplifies lookup
  - Enables dynamic weather/terrain management via registries

### Removed

- Deleted _weather.h_ (functionality moved to weather registry)

## [0.7.7] - 2026-07-29

### Changed

- Enhanced _statusID.h_ documentation for clarity and completeness

## [0.7.6] - 2026-07-29

### Added

- Applied _ATTR_NOINLINE_ attribute to _setEntry()_ in _fixedMetadataRegistry.h_
  - Prevents compiler from inlining this frequently-called method for better code locality

## [0.7.5] - 2026-07-29

### Added

- Implemented accuracy calculation system with configuration constants:
  - _MIN_ACCURACY_HIT_VALUE_ - Minimum roll value for accuracy check (0)
  - _MAX_ACCURACY_HIT_VALUE_ - Maximum roll value (255)
  - Accuracy stage multiplier numerator and denominator for stat-based accuracy scaling
- Implemented _AccuracyCheckHandler_ inheriting from _IEffectHandler_:
  - Determines move hit/miss based on user and target accuracy stats
  - Integrates with battle system for move resolution
- Added missing import directives to _criticalHitHandler.cpp_

### Changed

- Updated _EffectContext_ move accuracy member variable naming to follow conventions

### Fixed

- Fixed target defense stage multiplier lookup:
  - Changed from using user's attack stage to target's defense stage (was incorrectly using _userAttackStage_ instead of _targetDefenseStage_)
  - Corrects base damage formula calculations

## [0.7.4] - 2026-07-29

### Added

- Integrated compiler cache tool into build infrastructure:
  - Added _ccache_ to install steps in _codeql-analysis.yml_ and _testing.yml_
  - Speeds up rebuild times by caching compilation artifacts

## [0.7.3] - 2026-07-29

### Fixed

- Corrected move registry test expectations:
  - Fixed _moveRegistry.test.cpp_ to expect _Pound_ having 10 effects instead of 9

## [0.7.2] - 2026-07-29

### Added

- Implemented battle slot positioning system:
  - Added battle position variable to _BattleSlot_ struct for team coordination
- Implemented critical hit calculation system:
  - Configuration constants:
    - _MIN_CRITICAL_HIT_VALUE_ - Minimum critical hit roll (0)
    - _MAX_CRITICAL_HIT_VALUE_ - Maximum critical hit roll (100)
    - _CRITICAL_HIT_PERCENTAGE_ - Base critical hit chance
    - _CRITICAL_HIT_MULTIPLIER_ - Damage multiplier for critical hits (1.5)
    - Stat stage multiplier numerator and denominator
  - Added range override to _EffectContext_ for target specification
- Implemented effect handler base class architecture:
  - _IEffectHandler_ base class with virtual _apply()_ method
  - All future handlers inherit from this interface for consistent effect integration
- Implemented base damage handler:
  - _BaseDamageHandler_ inheriting from _IEffectHandler_
  - Calculates damage according to Bulbapedia formula
- Implemented critical hit handler:
  - _CriticalHitHandler_ inheriting from _IEffectHandler_
  - Calculates critical hit probability and damage multiplier
- Added effect handler utilities:
  - _getTeamBySlot()_ - Gets mutable team reference from _Side_
  - _getTeamBySlot()_ const overload - Gets const team reference
- Extended _Pokemon_ class:
  - Added level member variable with getter and setter
  - Enables per-Pokemon level tracking for damage calculation

### Changed

- Updated _StatStages_ variable type from `unsigned short` to `signed char`:
  - Allows negative stat stage values
  - Reduces memory footprint
- Optimized struct layouts:
  - Reordered _EffectResult_ members for smaller total byte size
  - Updated all call sites to match new member order
- Reorganized effect type IDs in _effectType.h_ and _moveMeta.cpp_ for optimization

### Removed

- Removed _StatStage_ effect type ID from _effectType.h_ and _moveMeta.cpp_
  - Functionality integrated directly into stat system

## [0.7.1] - 2026-07-29

### Added

- Integrated compiler cache into build system:
  - Added _ccache_ to _makefileDependencies.sh_ install step
  - Added _ccache_ to _COMPILER_ Makefile variable
  - Speeds up incremental builds significantly

## [0.7.0] - 2026-07-29 (Status Registry Update)

### Added

- Implemented status ID system infrastructure:
  - Builtin Status ID file with conversion function to stable Status ID
  - _Status/constants.h_ with user-readable status names
  - Status metadata file for persistence
- Added comprehensive test suite for _fixedMetadataRegistry.h_

### Changed

- Refactored _StatusID_ from enum to typedef of _IDInterface_:
  - Enables runtime status management and extensibility
  - Allows custom status definitions beyond hardcoded enum values

## [0.6.4] - 2026-07-29

### Added

- Implemented status registry system for extensible status ailment management:
  - Configuration constant _MAX_STATUSES_ controlling registry capacity
  - Error codes for duplicate status and status not found with _errorKindToString()_ support
  - Status registry class with constructor initializing all Pokemon-standard statuses
  - Registry provides:
    - Status metadata lookup (name, stable ID)
    - Registry queries (status exists by name or ID, get all entries, get next ID)
    - ID to array index mapping for efficient lookup
- Created status registry configuration classes:
  - Add/remove/rename statuses by name or stable ID
  - Update status metadata
  - Comprehensive error handling
- Integrated status registry configuration into _main.cpp_

### Changed

- Optimized memory layout:
  - Reordered _BattleSlot_ member variables for smaller total struct byte size
  - Reordered _EffectResult_ member variables for smaller total struct byte size
  - Updated all call sites to reflect new member order

## [0.6.3] - 2026-07-28

### Added

- Added mutation testing to _moveRegistry.test.cpp_
  - Improves test robustness against code mutations

## [0.6.2] - 2026-07-28

### Added

- Added code coverage exclusions:
  - _lcov_ pragmas in _abilityRegistry.h_ for unreachable branches
  - _lcov_ pragmas in _itemRegistry.h_ for unreachable branches
- Expanded registry test coverage:
  - New tests for _abilityRegistry_: get next ability ID, get amount registered, increment next ability ID
  - New tests for _itemRegistry_: get next item ID, get amount registered, increment next item ID
  - New tests for _moveRegistry_ with similar registry operations

### Changed

- Refactored test constants to use centralized constant values:
  - Updated _abilityRegistry.test.cpp_ to use _Ability/constants.h_ instead of hardcoded values
  - Updated _itemRegistry.test.cpp_ to use _Item/constants.h_ instead of hardcoded values
  - Updated _moveRegistry.test.cpp_ to use _Move/constants.h_ instead of hardcoded values
  - Updated test case names in _moveRegistry.test.cpp_ for clarity

## [0.6.1] - 2026-07-28

### Fixed

- Fixed move registry test expectations:
  - Updated _moveRegistry.test.cpp_ to include _PsychicTerrainPriorityBlock_ trigger for _Pound_ move

## [0.6.0] - 2026-07-28 (Move Registry Update)

### Added

- Extended ability and item registry with metadata update capabilities:
  - Added ability to replace target ID for abilities by stable ID or user-readable name
  - Added ability to update ability metadata by stable ID or user-readable name
  - Added ability to replace target ID for items by stable ID or user-readable name
  - Added ability to update item metadata by stable ID or user-readable name
- Implemented move registry system for extensible move management:
  - Configuration constant _MAX_MOVES_ controlling registry capacity
  - Error codes for duplicate move and move not found with _errorKindToString()_ support
  - Move registry class inheriting from _fixedMetadataRegistry.h_
  - Registry provides:
    - Move metadata lookup (name, stable ID)
    - Registry queries (move exists by name or ID, get all entries, get next ID)
    - ID to array index mapping for efficient lookup
- Created move registry configuration classes:
  - Add/remove/rename moves by name or stable ID
  - Set move triggers and effects by name or stable ID
  - Update move metadata by name or stable ID
  - Comprehensive error handling
- Extended _MoveMeta_ with range specification:
  - Added _MoveRangeID_ enum to _moveTargetsAndTriggers.h_ for targeting specification
  - Specifies what the move can hit (single, range, etc.)
- Enhanced ability and item metadata:
  - Added self target ID to _Drizzle_ ability
  - Added self target ID to _CheriBerry_ and _ChestoBerry_ items
- Completed move metadata initialization:
  - Added type ID, power, target ID, range ID, accuracy, priority, and special flag to all builtin moves
- Added code coverage exclusions:
  - _lcov_ pragmas in _fixedMetadataRegistry.h_ for unreachable branches
- Integrated move registry into _main.cpp_
- Added comprehensive test suite for _moveRegistry.h_

### Changed

- Updated documentation across registry configuration files:
  - Enhanced _abilityRegistryConfiguration.h_, _itemRegistryConfiguration.h_, _moveRegistry.h_, _itemRegistryConfiguration.cpp_
- Standardized metadata class naming:
  - Renamed _AbilityDefinition_ to _AbilityMeta_ throughout codebase
  - Renamed _ItemDefinition_ to _ItemMeta_ throughout codebase
- Applied code formatting:
  - Ran _clang-format_ on _effectContext.h_ and _moveMeta.cpp_
- Fixed linting warnings:
  - Resolved _clang-tidy_ warnings in _effectType.h_, _itemTargetsAndTriggers.h_, _moveRegistry.h_
- Updated ability trigger assignments:
  - Changed _Stench_ ability trigger from _OnUse_ to _OnDamageCalc_ in _abilityRegistry.h_ and tests

### Removed

- Removed _AbilityDefinition_ struct (renamed to _AbilityMeta_)
- Removed _ItemDefinition_ struct (renamed to _ItemMeta_)

## [0.5.3] - 2026-07-27

### Added

- Extended ability and item metadata with targeting:
  - Added target ID to _AbilityMeta_ for ability effect targeting
  - Added target ID to _ItemMeta_ for item effect targeting
- Implemented move registry capacity constant:
  - Configuration constant _MAX_MOVES_ controlling registry capacity
- Extended effect type system:
  - Added psychic terrain effect type for terrain-based ability interactions
- Enhanced item trigger and target system:
  - Added new trigger and target combinations in _itemTargetsAndTriggers.h_
- Implemented move builtin support:
  - Added _None_ variant to builtin Move IDs for null move representation
  - Implemented comprehensive move registry inheriting from _fixedMetadataRegistry.h_
  - Registry provides:
    - Move metadata lookup (name, stable ID)
    - Registry queries (move exists by name or ID, get all entries, get next ID)
    - ID to array index mapping for efficient lookup
- Extended _MoveMeta_ with complete metadata:
  - Type ID for move type classification
  - Power for base damage calculation
  - Target ID for move targeting specification
  - Accuracy for hit probability
  - Priority for move ordering
  - Special flag for status vs physical classification
- Implemented move effect helper functions:
  - Added helper functions returning commonly used _EffectTypeID_ sets for move classification

### Changed

- Updated dynamic linker configuration:
  - Changed _libstdc++.so.6_ symbolic link to reference version from _makefileDependencies.sh_ install step
  - Ensures consistent C++ runtime version across builds

### Removed

- Removed hardcoded _libstdc++.so.x.x.xx_ version copying from _makefileDependencies.sh_
  - Simplified by using symbolic link approach

## [0.5.2] - 2026-07-27

### Added

- Implemented move ID system infrastructure:
  - Builtin Move ID file with conversion function to stable Move ID
  - _Move/constants.h_ with user-readable move names
  - Move metadata file containing name, stable ID, and trigger/effect lists
- Implemented move trigger and target system:
  - Move triggers, targets, and combined wrapper structures for move effect specification

### Changed

- Refactored _MoveID_ from enum to typedef of _IDInterface_:
  - Enables runtime move management and extensibility
  - Allows custom move definitions beyond hardcoded enum values
- Applied code formatting:
  - Ran _clang-format_ on _pokemon.h_ for style consistency

### Fixed

- Fixed namespace resolution in _pokemon.h_ move field initialization

## [0.5.1] - 2026-07-27

### Added

- Added public access specifiers to configuration policy structures:
  - _AbilityRegistryConfigurationPolicy_, _AbilityMeta_
  - _ItemRegistryConfigurationPolicy_, _ItemMeta_
  - _MatchupPair_, _TypeDefinition_
  - _TypeEntry_

### Changed

- Enhanced function attributes for better optimization and safety:
  - Updated _errorKindToString()_ definition to use _ATTR_NODISCARD_ attribute
  - Updated _toTypeID()_ definition to use _ATTR_NODISCARD_ attribute
  - Ensures compiler warns when return values are discarded
- Optimized struct memory layouts throughout codebase:
  - Reordered 9 struct members for smaller total byte sizes:
    - _AbilityMeta_, _BattleSlot_, _AbilityEffectTrigger_, _RegistryError_, _EffectContext_, _ItemMeta_, _ItemEffectTrigger_, _Pokemon_
  - Updated all call sites to match new member order
- Enhanced code organization:
  - Extracted _PocketCore::Core::ub_ to using declarations in _AbilityTargetsAndTriggers.h_ and _builtinAbilityID.h_
  - Improves readability for frequently-used namespaces
- Improved documentation:
  - Updated documentation for _AbilityMeta_, _AbilityRegistryConfiguration_, _AbilityRegistry_, _FixedMetadataRegistry_
- Applied code formatting:
  - Ran _clang-format_ on _abilityRegistryConfiguration.h_, _fixedMetadataRegistryConfiguration.h_, _Configuration/constants.h_
- Comprehensive const-correctness improvements (73 function updates):
  - Made return types const in ability/item/type registry methods
  - Updated method parameters to const-reference/const for safety
  - Applied to all registry classes: ability, item, type, fixed metadata registries
- Enhanced parameter passing:
  - Updated _addBuiltin()_ to accept parameter by rvalue reference (&&) for move semantics
- Updated metadata getter return types:
  - _getAbilityMetadata()_ returns `const AbilityMeta *`
  - _getMetadata()_ returns `const Metadata *`
  - _getItemMetadata()_ returns `const ItemMeta *`
  - Prevents accidental modification of registry metadata

## [0.5.0] - 2026-07-27 (Item and FixedMetadata Registry Update)

### Added

- Implemented base registry infrastructure that all specialized registries inherit from:
  - _fixedMetadataRegistryConfiguration.h_ - Configuration base providing:
    - Metadata, stable ID, and user-readable name access
    - Entry existence queries (by stable ID or name)
    - Add/batch add metadata with transaction support
    - Mutate registered metadata (copy-modify-write pattern)
    - Rename and remove metadata operations
  - _fixedMetadataRegistry.h_ - Runtime registry base providing:
    - Entry access by stable ID or array index
    - Stable ID, name, and metadata queries
    - Next stable ID and amount registered tracking
    - Array index lookup by stable ID (binary search ready)
    - Entry existence queries
    - Entry mutation and amount management
    - Protected constructor for inheritance
    - Protected builtin initialization method
- Implemented item registry system:
  - Configuration constant _MAX_ITEMS_ controlling registry capacity
  - Error codes for duplicate item and item not found with _errorKindToString()_ support
  - Item registry configuration class with user-friendly operations:
    - Get/set item metadata and properties
    - Add/remove/rename items
    - Set item triggers and effects
    - Comprehensive error handling
  - Item registry class for runtime management
  - Item registry configuration policy structure
- Added item error handling:
  - New error codes in _RegistryError_ for item operations
  - Updated _errorKindToString()_ with item error branches
- Integrated item and ability registries into _main.cpp_
- Added comprehensive test suites:
  - _itemRegistryConfiguration.h_ test suite
  - _itemRegistry.h_ test suite

### Changed

- Refactored ability and item registries to inherit from fixed metadata base:
  - _AbilityRegistryConfiguration_ delegates to _FixedMetadataRegistryConfiguration_:
    - All getter methods call parent implementations
    - Add/remove/rename operations delegate to parent
  - _AbilityRegistry_ delegates to _FixedMetadataRegistry_:
    - All metadata access calls parent implementations
    - ID management delegates to parent
  - _ItemRegistry_ delegates to _FixedMetadataRegistry_:
    - Mirrors ability registry structure for consistency
- Updated ability and item configuration implementations to use parent methods:
  - _addAbility()_, _addAbilities()_ delegate to parent batch add
  - _setAbilityTriggers()_ delegates to parent mutate
  - _renameAbility()_, _removeAbility()_ delegate to parent implementations
- Updated test initialization:
  - Modified _pokemon.test.cpp_ to use default initialization for item IDs

### Fixed

- Fixed header guard inconsistencies:
  - _Ability/constants.h_, _Item/constants.h_
- Updated return types for consistency:
  - Changed from `unsigned char` to `unsigned short` in multiple test files:
    - _typeRegistryConfiguration.test.cpp_, _typeRegistry.test.cpp_, _idInterface.test.cpp_

### Removed

- Removed redundant functions from _AbilityRegistryConfiguration_:
  - Both overloads of _resolveIndex()_
  - _removeEntry()_
  - Member variable (now inherited)
- Removed redundant functions from _AbilityRegistry_ (now inherited from base):
  - _getEntry()_, _getAmountRegistered()_, _setEntry()_, _setAmountRegistered()_
  - _incrementAmountRegistered()_, _decrementAmountRegistered()_
  - _findEntryIndexByName()_, _findEntryIndexByID()_, _addBuiltin()_
  - Member variables (now inherited)
- Removed redundant functions from _ItemRegistry_ (now inherited):
  - Same methods as removed from _AbilityRegistry_
  - Member variables (now inherited)

## [0.4.1] - 2026-07-27

### Added

- Implemented generic stable ID infrastructure:
  - Created _IDInterface_ base class that all stable ID tags inherit from
  - Enables typed stable IDs for abilities, items, moves, and types
  - Provides common interface for ID management across registry system
- Implemented item ID system:
  - Builtin Item ID file with conversion function to stable Item ID
  - _Item/constants.h_ with user-readable item names
  - Item metadata file containing name, stable ID, and trigger/effect lists
- Implemented item trigger and target system:
  - Item triggers, targets, and combined wrapper structures
- Implemented item registry:
  - Constructor initializing all builtin items
  - Metadata access and query methods
  - Array index lookup by stable item ID
  - Entry existence checks by name or ID
  - Entry mutation and amount tracking
- Implemented ability ID tagging:
  - Unique empty struct _AbilityIDTag_ for type safety
- Extended configuration constants:
  - _MAX_ABILITIES_PER_POKEMON_ for individual ability capacity
  - _MAX_ITEMS_ for item registry capacity
  - _MAX_ITEMS_PER_POKEMON_ for individual item capacity
- Extended effect types:
  - Added _StatusRemove_ effect type for status removal effects
- Added test infrastructure:
  - Test suite for _idInterface.h_

### Changed

- Enhanced ID type safety:
  - Updated _toAbilityID()_ to use _ATTR_NODISCARD_ attribute
  - Refactored _AbilityID_ from class to typedef of _IDInterface_
  - Refactored _ItemID_ from enum to typedef of _IDInterface_
  - Refactored _TypeID_ from class to typedef of _IDInterface_
  - All enable runtime extensibility and consistent type safety
- Updated numeric types for registry indices:
  - Changed return types from `unsigned char` to `unsigned short` in:
    - _abilityRegistryConfiguration.h_: 9 functions + 2 member variables
    - _typeRegistryConfiguration.h_: 6 functions
    - _typeRegistry.h_: 12 functions + 2 member variables
  - Allows support for up to 65,535 registry entries
- Updated ability registry capacity:
  - Increased maximum abilities from 64 to 1000 in configuration
- Applied code formatting:
  - Updated _abilityRegistryConfiguration.h_ for style consistency
- Fixed namespace resolution:
  - Updated registry member variable access in _abilityRegistryConfiguration.h_
  - Updated _abilityRegistryConfiguration.cpp_
  - Updated _abilityRegistry.h_
  - Updated _abilityRegistry.test.cpp_
- Reordered configuration constants:
  - Reorganized _Configuration/constants.h_ values

### Fixed

- Fixed namespace resolution issues across ability registry implementation

### Removed

- Removed default initializer of _mItemID_ in _pokemon.h_
  - Items must now be explicitly set

## [0.4.0] - 2026-07-27 (Ability Registry Update)

### Added

- Implemented foundational ability registry system:
  - Configuration constant for NO_ABILITY_ID
  - Builtin Ability ID file with conversion function to stable Ability ID
- Implemented ability registry:
  - Constructor initializing all builtin abilities
  - Metadata access (name, ID lookup)
  - Registry query methods (entry count, next ID)
  - Array index lookup by stable ability ID
  - Entry existence checks by name or ID
  - Entry mutation and amount tracking
  - Getter/setter methods for next ability ID and amount registered
- Implemented ability registry configuration:
  - User-friendly configuration interface
  - Add/remove/rename ability operations
  - Set ability triggers and effects
  - Comprehensive error handling
  - Ability configuration policy structure with:
    - Configuration user name
    - Configuration type
    - Error codes for duplicate/not-found conditions
- Extended configuration system:
  - _MAX_ABILITIES_IN_REGISTRY_ for ability registry capacity
  - _MAX_TYPES_PER_POKEMON_ for Pokemon type array capacity
  - _MAX_MOVES_PER_POKEMON_ for Pokemon move array capacity
- Implemented Pokemon type system:
  - Added type ID array to Pokemon class
  - Getter/setter methods for type array and individual types
- Implemented stable type ID system:
  - Created _typeID.h_ with stable TypeID class
  - Added NO_TYPE_ID constant
  - Added conversion function from type enum to stable TypeID
- Extended effect system:
  - Added ability and item error codes to _RegistryError_
  - Updated _errorKindToString()_ with error branches
- Added comprehensive test suites:
  - _abilityRegistryConfiguration.h_ test suite
  - _abilityRegistry.h_ test suite
  - _pokemon.h_ test suite
  - Extended tests in _typeRegistryConfiguration.test.cpp_

### Changed

- Refactored ID system for type safety:
  - Changed _AbilityID_ from enum class to stable typed ID class
  - Enables custom ability definitions beyond builtin set
- Updated type registry checks:
  - Added _NO_TYPE_ID_ validation in _addType()_ method
- Enhanced numeric types:
  - Updated return types from `unsigned char` to `unsigned short` in:
    - _typeRegistryConfiguration.h_ and implementations
    - _typeRegistry.h_: returns stable TypeID instead of raw bytes
  - Updated member variable types in _typeRegistry.h_
- Updated effect system:
  - Changed _mMoveTypeID_ in _effectContext.h_ from `unsigned char` to _TypeID_
  - Uses stable typed ID for type safety
- Updated Pokemon interface:
  - Constructors now accept array of type IDs
  - Updated all move count references to use _MAX_MOVES_PER_POKEMON_ constant
  - Improved consistency across _pokemon.h_ and _pokemon.cpp_
- Applied code analysis fixes:
  - Fixed clang-tidy warnings in 14 files:
    - Headers: _abilityTargetsAndTriggers.h_, _effectContext.h_, _effectType.h_, _itemID.h_, _moveID.h_, _statusID.h_, _typeID.h_, _pokemon.h_, _timer.h_, _weather.h_, _Types/constants.h_
    - Tests: _typeRegistryConfiguration.test.cpp_, _timer.test.cpp_
    - Implementation: _main.cpp_
- Updated documentation:
  - Enhanced docs for _abilityMeta.h_, _Configuration/constants.h_, _typeRegistry.h_
- Applied code formatting:
  - Ran _clang-format_ on _typeRegistryConfiguration.cpp_
- Updated test expectations:
  - Modified _typeRegistryConfiguration.test.cpp_ for stable TypeID
  - Modified _typeRegistry.test.cpp_ for stable TypeID

### Removed

- Removed _getAbilityMetadata()_ function from registry (consolidated into generic interface)

## [0.3.2] - 2026-07-27

### Added

- Extended build artifact management:
  - Added _.log_ files to _.gitignore_ for build log exclusion
- Implemented type registry utilities:
  - Added _clearRows()_ method to _typeRegistryConfiguration.h_
  - Clears offensive row and defensive column for type at specified index
  - Used by reset/matchup update operations
- Enhanced code coverage tracking:
  - Added _lcov_ exclusion annotations in _typeRegistryConfiguration.cpp_
  - Marks branches that are provably unreachable (impossible code paths)

### Changed

- Applied code formatting:
  - Ran _clang-format_ on _typeRegistryConfiguration.cpp_
- Refactored matchup reset operations:
  - Updated both _resetMatchup()_ function overloads to use new _clearRows()_ method
  - Improves code reusability and maintainability

### Removed

- Removed redundant array index check in _renameType()_
  - Previously checked array index existence after type ID lookup
  - Type ID existence check guarantees array index exists (invariant)
  - Eliminates unnecessary duplicate validation

## [0.3.1] - 2026-07-26

### Removed

- Removed redundant hooks folder from repository
  - Simplifies project structure

## [0.3.0] - 2026-07-26 (Pokemon & Ability, Item, and Status IDs)

### Added

- Enhanced build system infrastructure:
  - Added _COMPILER_VERSION_ to makefile targets: _run_ and _run_tidy_
  - Enables version-specific compilation and analysis
- Implemented stable ID system for Pokemon attributes:
  - Stable ID for moves enabling runtime move management
  - Stable ID for items enabling custom item definitions
  - Stable ID for abilities enabling custom ability definitions
  - Stable ID for status conditions enabling custom status effects
- Implemented ability system infrastructure:
  - Ability trigger, target, and combined wrapper structures
  - Ability constants file with user-readable ability names
  - Ability metadata file containing name, stable ID, and trigger/effect lists
- Implemented Pokemon class:
  - Base stat tracking:
    - Attack, Defense, Health
    - Special Attack, Special Defense, Speed
  - Stable ID fields:
    - Item ID
    - Ability ID
    - Move ID array (4-move set)
  - Getter/setter methods for all fields
  - PP (Power Point) usage tracking method
- Implemented battle slot infrastructure:
  - _BattleSlot_ structure containing:
    - Active Pokemon reference
    - Item and status ID tracking
    - Choice lock tracking (move ID)
    - Condition flags: protected, flinched, fainted, item consumed
    - Counters: toxic, sleep, protection
    - Current HP and speed boost tracking
    - Stat stage modifiers for all 6 stats
- Implemented battle state tracking:
  - _BattleState_ structure containing:
    - Both sides' Pokemon and corresponding BattleSlot entries
    - Entry hazard state per side (spikes, toxic spikes, stealth rock)
    - Battle initialization flag
    - Weather tracking: rain, sun, sandstorm support
    - Terrain tracking: electric, grassy, misty, psychic support
- Implemented effect context infrastructure:
  - Battler side enumeration for targeting
  - Effect source enumeration for origin tracking
  - Damage context structure containing:
    - Recoil ratio for move recoil calculation
    - Total damage accumulator
    - Critical hit, miss, and protection flags
    - Damage application and continuation flags
  - Effect result structure with:
    - Sleep turn counter
    - Status application
    - Stat stage changes (attack/defense)
  - Effect context aggregation containing:
    - Damage and effect result structures
    - Effect source tracking
    - Ability/item/critical hit multipliers
    - User/target index tracking
    - Move base power, type ID, accuracy
    - Move and ability ID references
    - User/target side tracking
    - Special flag for move classification
- Implemented effect type system:
  - Effect type ID enumeration for effect classification
  - Foundational for effect handler dispatch system
- Implemented weather system:
  - Weather enumeration for battle condition tracking
- Automated configuration file management:
  - Updated clang-tidy macro regex to include _ATTR_ prefix
  - Enables custom attribute macro checking
- Enhanced build system robustness:
  - Changed _TEST_INTEGRATIONS_SOURCES_, _TEST_MOCKS_SOURCES_, _BENCHMARKS_SOURCES_ to use wildcard patterns
  - Improves safety for file discovery
- Namespace unification across codebase (20+ files updated):
  - Changed namespace from _Pokemon_ to _PocketCore_ throughout:
    - Configuration files: _configFlags.h_, _Configuration/constants.h_, _Registry/constants.h_
    - Registry system: _typeRegistryConfiguration.h_, _typeRegistry.h_, _typeRegistryConfiguration.test.cpp_, _typeRegistry.test.cpp_
    - Type system: _cconcepts.h_, _Types/constants.h_, _typeEffectiveness.h_, _types.h_
    - Utility files: _configCat.h_, _configCat.cpp_
    - Infrastructure: _timer.h_, _timer.test.cpp_, _contiguousSequence.h_, _contiguousSequence.test.cpp_
    - Logging: _Logging/constants.h_, _logger.h_, _logger.cpp_, _logger.test.cpp_
    - Helper utilities: _floatUtility.h_, _floatUtility.test.cpp_, _overflowProtection.h_, _overflowProtection.test.cpp_, _input.h_, _random.h_
    - Main entry: _main.h_
  - Reflects project scope change from single-game Pokemon engine to generic PocketCore framework

### Changed

- Optimized struct memory layout:
  - Moved _mTypeChart_ member variable below _mEntries_ in type registry
  - Reduces struct byte size and improves cache efficiency

### Removed

## [0.2.19] - 2026-05-01

### Added

- Enhanced code coverage tracking:
  - Added _lcov_ exclusion annotations to _typeRegistryConfiguration.cpp_
  - Marks unreachable/impossible code branches (provably unreachable)
- Enhanced error handling:
  - Added logging in _resetMatchups()_ for type not found condition
  - Improves debuggability of registry operations
- Added comprehensive test suite:
  - Test file for _typeRegistryConfiguration_ with full coverage

### Changed

- Improved error handling defaults:
  - Changed default error in _errorKindToString()_ to _UnknownError_
  - Ensures graceful fallback for unexpected error types
- Standardized identifier naming convention:
  - Changed all instances of _\*Id_ to _\*ID_ across 5 files:
    - _typeRegistryConfiguration.h_
    - _Registry/constants.h_
    - _typeRegistry.h_
    - _typeRegistryConfiguration.cpp_
    - _typeRegistry.test.cpp_
  - Improves code consistency and readability

### Removed

- Removed BatchMismatch error from _RegistryError_:
  - No longer needed for registry operations
- Optimized type registry operations by removing redundant checks:
  - Removed duplicate check in _addType()_ (validation already performed)
  - Removed redundant bounds check in _rollbackEntries()_
    - Amount validation guaranteed before this method
  - Removed redundant index lookup check in _resolveIndex()_
    - Previous type ID existence check guarantees index validity
  - Reduces code complexity and improves performance

## [0.2.18] - 2026-05-01

### Added

- Enhanced inline optimization control:
  - Added _-Winline_ compiler warning to _GCC_WARNINGS_ in Makefile
  - Warns when inline requests cannot be satisfied
  - Added _ATTR_NOINLINE_ attribute to _attributeMacros.h_
  - Allows explicit prevention of function inlining
- Enhanced documentation:
  - Added reasoning/documentation for _timer.h_ constructors/destructors deletions
  - Added reasoning/documentation for _logger.h_ constructors/destructors deletions
  - Improves code understandability

### Changed

- Applied no-inline optimization:
  - Applied _ATTR_NOINLINE_ to _getTypeID()_ function
  - Prevents inlining for performance profiling/debugging

### Fixed

- Fixed typo in _typedefs.h_

### Removed

- Removed _sfloat_ type definition from _typedefs.h_
  - No longer used in codebase

## [0.2.17] - 2026-04-30

### Added

- Implemented advanced type checking:
  - Added C++ concept to check if type is an enum
  - Enables compile-time enumeration detection
- Added commented C++ reflection code in _typedefs.h_:
  - Converts enum values to string names
  - Awaiting clangd linter support for C++26 reflection
  - Comment indicates future enhancement when language support available

### Changed

- Updated C++ language standard:
  - Changed _COMPILER_VERSION_ from previous version to _c++26_ in Makefile
  - Enables use of latest C++ features and optimizations

### Removed

- Removed _-Winline_ from _GCC_WARNINGS_:
  - Determined to be too noisy for codebase

## [0.2.16] - 2026-04-30

### Changed

- Updated C++ standard library version:
  - Updated libstdc++ from version 6.0.34 to 6.0.35 in _makefileDependencies.sh_
  - Ensures runtime compatibility with latest GCC improvements

## [0.2.15] - 2026-04-30

### Changed

- Updated compiler version:
  - Updated GCC from 15.2.0 to 16.1.0 in _makefileDependencies.sh_
  - Provides access to latest compiler optimizations and fixes

## [0.2.14] - 2026-04-29

### Added

- Added build analysis tool:
  - Added _bear_ to build dependency packages in _makefileDependencies.sh_
  - Bear captures compilation commands for analysis tooling

## [0.2.13] - 2026-04-29

### Removed

- Removed environment variable configuration from _codeql-analysis.yml_
  - Determined to be unnecessary for analysis

## [0.2.12] - 2026-04-29

### Added

- Enhanced CodeQL configuration:
  - Added environment variable setup in _codeql-analysis.yml_
  - Added workflow step to ensure CodeQL TRAP directory exists
  - Creates directory if not present, enabling proper analysis output

## [0.2.11] - 2026-04-29

### Added

- Enhanced CodeQL workflow configuration:
  - Added _build-mode: manual_ to _codeql-analysis.yml_
  - Provides explicit control over build process during analysis

## [0.2.10] - 2026-04-29

### Changed

- Updated GitHub Actions:
  - Updated checkout action from v4 to v6 in _codeql-analysis.yml_
  - Provides latest features and security improvements

## [0.2.9] - 2026-04-29

### Changed

- Optimized workflow caching:
  - Reordered workflow steps to run CodeQL analysis before cache save
  - Ensures analysis results are cached, improving subsequent run performance

## [0.2.8] - 2026-04-29

### Changed

- Updated CodeQL workflow configuration:
  - Updated _codeql-analysis.yml_ to use official CodeQL workflow template
  - Provides better maintainability and feature updates
- Updated testing workflow:
  - Updated _testing.yml_ runner to use _ubuntu-latest_
  - Ensures compatibility with latest CI/CD environment

## [0.2.7] - 2026-04-29

### Changed

- Updated GitHub Actions:
  - Updated CodeQL init action from v3 to v4
  - Provides improved CodeQL database initialization

## [0.2.6] - 2026-04-29

### Changed

- Updated GitHub Actions:
  - Updated CodeQL analyze action from v2 to v4
  - Provides improved security analysis capabilities

## [0.2.5] - 2026-04-29

### Added

- Enhanced spdlog dependency caching:
  - Added _local/cmake/pkgconfig_ folder creation in build workflows
  - Ensures CMake package configuration files available for spdlog
  - Implemented in both _testing.yml_ and _codeql-analysis.yml_

### Changed

- Improved dependency management:
  - Updated spdlog cache installation to copy _/lib/cmake/spdlog_ files to local directory
  - Ensures spdlog CMake modules properly located for package discovery

## [0.2.4] - 2026-04-29

### Added

- Added Catch2 testing framework:
  - Integrated _catch2_ into testing framework installation step
  - Added Catch2 to build dependencies
  - Enables parallel testing framework support (GoogleTest + Catch2)

### Changed

- Improved CI/CD clarity:
  - Renamed testing installation step from "Install Google Mock" to "Install Testing Frameworks"
  - Updated in both _testing.yml_ and _codeql-analysis.yml_
  - Reflects support for multiple test frameworks

## [0.2.3] - 2026-04-29

### Changed

- Enhanced build permissions:
  - Added sudo permissions when creating _local/lib/pkgconfig_ folder
  - Ensures folder creation succeeds in restricted environments

## [0.2.2] - 2026-04-29

### Added

- Enhanced dependency caching:
  - Added _local/lib/pkgconfig_ folder creation in build workflows
  - Ensures pkg-config can locate spdlog package configuration
  - Implemented in both _testing.yml_ and _codeql-analysis.yml_

## [0.2.1] - 2026-04-29

### Changed

- Improved spdlog caching strategy:
  - Updated spdlog cache installation path in both _testing.yml_ and _codeql-analysis.yml_
  - Modified to copy entire folders from GCC cache install to local directory
  - Improves cache efficiency and dependency resolution

## [0.2.0] - 2026-04-29 (Type Registry Conversion)

### Changed

- Refactored registry module naming:
  - Renamed _configuration.h_ to _typeRegistryConfiguration.h_
  - Renamed _configuration.cpp_ to _typeRegistryConfiguration.cpp_
  - Clarifies file purpose for type registry configuration
  - Makes codebase extensible for future registries (ability, item, status)
- Updated registry documentation:
  - Updated documentation for configuration constants file
  - Updated documentation for _typeRegistryConfiguration.h_
- Reorganized function definitions:
  - Reordered functions in _typeRegistryConfiguration.h_ and _typeRegistryConfiguration.cpp_
  - Improves logical grouping and code navigation
- Updated module imports:
  - Updated imports in _main.cpp_ to reflect new filenames
- Enhanced code analysis:
  - Added clang-tidy exclusions in _typeRegistry.test.cpp_
  - Suppresses false positives in test code

## [0.1.14] - 2026-04-29

### Added

- Start caching Spdlog in the github workflows

### Changed

- Reorder Spdlog copy steps in _makefileDependencies.sh_

## [0.1.13] - 2026-04-28

### Changed

- Reordered the _configCat_ installation in _makefileDependencies.sh_ to be higher up

## [0.1.12] - 2026-04-28

### Changed

- Updated the checkout action from v4 to v6 and the cache action from v3 to v5 in:
  - _codeql-analysis.yml_
  - _testing.yml_
  - _semgrep.yml_
- In _makefileDependencies.sh_, changed _sudo apt..._ to _sudo apt-get update_

## [0.1.11] - 2026-04-28

### Added

- Caching for config cat, curl, and hash library for _codeql-analysis.yml_

## [0.1.10] - 2026-04-28

### Added

- Add a check in _makefileDependencies.sh_ to find determine if configCat is already installed, if not install it.

## [0.1.9] - 2026-04-28

### Added

- A step in the github workflows to find the latest version of GCC

### Changed

- When saving the GCC cache, we now use the latest gcc version found in the previous step.

### Removed

- Removed conditional gcc installation in _makefileDependencies.sh_, now just always installs one version.

## [0.1.8] - 2026-04-28

### Added

- GCC cache in github workflow actions:
  - _codeql-analysis.yml_
  - _testing.yml_

## [0.1.7] - 2026-04-24

### Fixed

- Ran shellcheck and shellfmt on _makefileDependencies.sh_

## [0.1.6] - 2026-04-24

### Changed

- Updated the _typeRegistry.test.cpp_ to _catch2_ format

## [0.1.5] - 2026-04-24

### Added

- Added _-D${TEST_STANDARD}_ to the _clang-tidy_ command within the _tidy_ Makefile target
- Added _-D${TEST_STANDARD}_ to the _run-clang-tidy_ command within the _run_tidy_ Makefile target

### Changed

- Updated the _TEST_STANDARD_ Makefile variable to _catch2_ by default
- Converted the google tests for the following test files into catch2 tests:
  - _cconcepts.test.cpp_
  - _timer.test.cpp_
  - _contiguousSequence.test.cpp_
  - _logger.test.cpp_
  - _floatUtility.test.cpp_
  - _overflowProtection.test.cpp_

## [0.1.4] - 2026-04-22

### Added

- Added _-D${TEST_STANDARD}_ to the _COMPILER_FLAGS_TEST_ Makefile variable

### Changed

- Moved the _TEST_STANDARD_ Makefile variable closer to the top of the file
- Changed the _testMain.cpp_ file to conditionally include either the catch2 or googletest libraries depending on what the _TEST_STANDARD_ is set to
  - This will also then execute either the catch2 or googletest test runner depending on what the _TEST_STANDARD_ is set to

## [0.1.3] - 2026-04-22

### Added

- Added a _TEST_STANDARD_ variable in the Makefile that defaults to _googletest_
- Added a _GOOGLE_TEST_LIBRARIES_ Makefile variable that contains the libraries required for Google Test to work if the _TEST_STANDARD_ is set to _googletest_ otherwise it will be empty
- Added a _Catch2_LIBRARIES_ Makefile variable that contains the libraries required for Catch2 to work if the _TEST_STANDARD_ is set to _catch2_ otherwise it will be empty
- Added a _GOOGLE_TEST_EXECUTION_FLAGS_ Makefile variable that contains the execution flags for Google Test if the _TEST_STANDARD_ is set to _googletest_ otherwise it will be empty
- Added a _CATCH2_EXECUTION_FLAGS_ Makefile variable that contains the execution flags for Catch2 if the _TEST_STANDARD_ is set to _catch2_ otherwise it will be empty

### Changed

- Updated the _TEST_LIBRARIES_ Makefile variable to both _GOOGLE_TEST_LIBRARIES_ and _Catch2_LIBRARIES_
- Updated the _TEST_EXECUTION_FLAGS_ Makefile variable to both _GOOGLE_TEST_EXECUTION_FLAGS_ and _CATCH2_EXECUTION_FLAGS_

## [0.1.2] - 2026-04-22

### Added

- Added in _catch2_ to the list of makefile dependencies
- Added in the _Catch2_ library in the _TEST_LIBRARIES_ Makefile variable
- Added _lcov_ exclusions that match assert and _LCOV_EXCL_BR_
- Added a _clean_coverage_ phony Makefile target that will delete all _lcov_ coverage files
- Added the _clean_coverage_ target to the _README.md_ and the _makefileDescriptions.rst_
- Added the _run_tidy_ target to the _README.md_ and the _makefileDescriptions.rst_

### Changed

- Added the _lcov_ exclusions to the _LCOV_FLAGS_ Makefile variable
- Added the _lcov_ exclusions to the _lcov_ command within the _lcov_ Makefile target
- Added the _lcov_ exclusions to the _genhtml_ command within the _genhtml_ Makefile target
- The _coverage_ Makefile target now also depends on the _clean_coverage_ target
- Updated the description of the _coverage_ target in the _README.md_ and _makefileDescriptions.rst_
- Updated the description of the _tidy_ target in the _README.md_ and _makefileDescriptions.rst_
- Added in _lcov_ exclusions to the _typeRegistry_ as some of the branches are just unreachable
- Changed types from auto to their exact types in the _typeRegistry.test.cpp_ file
- Changed types from auto to their exact types in the _logger.test.cpp_ file
- Changed types to their _typedefs.h_ types in the _logger.test.cpp_ file
- Changed _readLogFile_ in _logger.test.cpp_ to take in a file name variable and use that instead of the private log file member variable.

### Removed

- Removed redundant folder path finding on _GENHTML_OUTPUT_FOLDER_ Makefile variable
- Removed redundant folder path finding on _PROFILE_FOLDER_ Makefile variable

## [0.1.1] - 2026-04-20

### Fixed

- Fixed the project name in the README.md to _pocketcore_
- Fixed the sphinx project documentation _conf.py_ to have the appropriate title and repository URL

## [0.1.0] - 2026-04-20 (Type Configuration Update)

### Added

- Added in boilerplate code from [CPPBase](https://github.com/Phaysik/CPPBase/commit/34d5cc39654bd807c3f62a8af207e07d3f971f1d)
- Create a constants file that defines registry error assert message strings.
- Create a type effectiveness file that holds the 5 possible states of type effectiveness.
  - NOT_DEFINED - A sentinel value for uninitialized matchups
  - NE - No Effect (0x multiplier)
  - NVE - Not Very Effective (0.5x multiplier)
  - E - Effective (1x multiplier)
  - SE - Super Effective (2x multiplier)
- Create a type enum file that holds the enum of all standard pokemon types.
- Create a type registry file.
  - Holds an enum that is a mapping pair of a stable type ID to a human readable name.
  - Holds the type registry which contains:
    - All the builtin standard types and their type matchups that come from Pokemon (including the Stellar type)
    - Can query to get the type entry by index
    - Can query to get a single type matchup from the type chart by offensive row and defensive column
    - Can query to get an entire type matchup row from the type chart
    - Can get the stable type ID from the registry by name
    - Can get the name of a type from the registry by stable type ID
    - Get the amount currently registered types in the registry
    - Get the next type ID that will be assigned to a new type
    - Get a read-only span of all registered types in the registry
    - Can set/override an entry in the registry with a new type entry
    - Can set the type matchup in the type chart by passing in the offensive row, defensive column, and the type effectiveness
    - Set an entire row of type matchups in the type chart by passing in a row index and an array of type effectiveness values
    - Can set the amount of types registered in the registry
    - Can set the next stable type id that will be assigned to a new type
    - Can find the internal array index of a type by its stable type ID
    - Can query to see if the registry contains a type:
      - By its stable type ID
      - By its user name
- Create a constants file that holds the metadata for the builtin types and their type matchups.
- Create a constants file that defines configuration constants for the application and registry error metadata.
  - Holds a RegistryError enum to specify any error that could happen while adding types to the registry.
  - A single struct to convert the error enum to a human readable string.
  - An enum that can be used to specify what to fill unspecified type matchups as:
    - Neutral (1x)
    - NotDefined (NOT_DEFINED)
- Create a type configuration file that allows users to:
  - Define a matchup pair that has a user readable name along with the effectiveness of the type. The effectiveness here is meant to be used to determine the strength of the type against other types.
  - Define a type definition that contains a user readable name along with the offensive and defensive strengths of the type.
  - Add a single type or an array of types
  - Remove a type:
    - By user defined name
    - By stable type ID
  - Rename a type
  - Set the type matchup of any type:
    - Can do entire offensive rows
    - Can do entire defensive columns
  - Can reset the type's matchup to NOT_DEFINED
  - Can get the type matchup effectiveness for any type against any other type
    - Can get the entire type matchup effectiveness offensive row
    - Can get the entire type matchup effectiveness defensive column
  - Can check if the registry has a type
    - By user defined name
    - By stable type ID
  - Can get the stable type ID of a type by its user defined name
  - Can get the user defined name of a type by its stable type ID
  - Get the entire type registry as a non-owning view
  - Get the total amount of types registered

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
