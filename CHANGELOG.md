# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/2.0.0/), and this project adheres to _vX.Y.Z_ versioning where _X_ represents an _edition_, _Y_ represents an _update_, and _Z_ represents an _addendum_.

## [0.11.0] - 2026-08-21 (BattleEngine Update)

### Added

- Added more methods to _BattleEngine_:
  - Execute a weight hit count policy
  - Execute a fixed hit count policy
  - Execute the damage application

### Changed

## [0.10.11] - 2026-08-21

### Changed

- Use maximum ammount of cores on make run command in:
  - _codeql-analysis.yml_
  - _testing.yml_

## [0.10.10] - 2026-08-21

### Changed

- _hasReserve()_ take in a _std::span<Pokemon *const>_ instead of _std::span<Pokemon *>_

## [0.10.9] - 2026-08-21

- Add documentation to:
  - _getOppositeSide()_
  - _getSideOrder()_
  - _anyPartyPokemonNull()_
  - Both overloads of _activeSlots()_
  - _contextSlot()_
  - Both overloads of _party()_
  - Both overloads of _isHealthy()_
  - _isActive()_
  - _isAdjacent()_
  - _targetExists()_
  - _sideHasHealthyPokemon()_
  - _getEffectiveSpeed()_
  - _makeMoveContext()_
  - _applyRecoil()_
  - _resolveHitCount()_
  - _hasDuplicatePokemonPointers()_
  - _healthyPokemonInParty()_
  - _assignActiveSlots()_
  - _canTarget()_
  - _appendSide()_
  - _getResult()_
  - _hasReserve()_
  - _getMoveTargets()_
  - _validateSwitchAction()_
  - _validateMoveAction()_
  - _getValidationResult()_
  - _getBattleTarget()_
  - _handleMovePrioritization()_

### Changed

- Ran _clang-format_ on:
  - _battleEngine.h_
  - _battleTargetsAndTriggers.h_
  - _effectSourceAndSuppression.h_
  - _moveMeta.h_
  - _moveRegistryConfiguration.test.cpp_
- Update _hasReserve()_ to take in a _span_ instead of a _vector_

## [0.10.8] - 2026-08-18

### Added

- Add a _BattleEventRole_ struct that identifies a participants role in a battle
- Add a _BattleEventID_ struct that identifies an event that can trigger a battle effect
- Add documentation to _BattleTargetID_ and _BattleRangeID_

### Changed

- Update all occurences of _BattleTriggerID_ with a _BattleEventID_ and _BattleEventRole_
- Extract shared namespace usage into a using statement in:
  - _abilityRegistry.h_
  - _effectRegistry.h_
  - _itemRegistry.h_
  - _moveRegistry.h_
  - _multiplierRegistry.h_
  - _statusRegistry.h_
  - _terrainRegistry.h_
  - _typeRegistry.h_

### Fixed

- Add in missing headers for _battleHelpers.cpp_

### Removed

- Removed _BattleTriggerID_

## [0.10.7] - 2026-08-18

### Added

- Added more methods to _BattleEngine_:
  - Execute a turn
  - Execute a move
  - Execute an end turn trigger
- Add a few more battle function helpers:
  - _getValidationResult()_
  - _getBattleTarget()_
  - _handleMovePrioritization()_
- Add _ATTR\_NOINLINE_ to _getMetadata()_
- Added a small example of how to execute a turn in _main.cpp_

### Removed

- Removed _configFlags.h_

## [0.10.6] - 2026-08-17

### Added

- Add _ATTR\_CONST_ to _getState()_

### Fixed

- Fix _clang-tidy_ warnings on _getState()_ in _battleEngine.cpp_ by wrapping it in a _GCC_ diagnostic push

## [0.10.5] - 2026-08-17

### Added

- Added more methods to _BattleEngine_:
  - Get the current state of the battle
  - Switch a _Pokemon_

## [0.10.4] - 2026-08-17

### Added

- Add a function to execute a move trigger
- Add a few more battle function helpers:
  - _getMoveTargets()_
  - _validateMoveAction()_
  - _validateSwitchAction()_

### Changed

- Move _resolveTargets_ from _battleEngine.h_ to _battleHelpers.h_

## [0.10.3] - 2026-08-14

### Added

- Add in a battle engine header
  - It can start a battle
    - This will activate suppressions for:
      - Abilities
      - Items
    - Check if an ability or item is suppressed
    - Get a list of targets
    - Execute triggers
      - Abilities
      - Items
    - Process faints
    - Refresh the battle phase
    - Trigger a faint
- Add in more battle helper functions:
  - _anyPartyPokemonNull()_
  - _hasDuplicatePokemonPointers()_
  - _healthyPokemonInParty()_
  - _assignActiveSlots()_
  - _canTarget()_
  - _appendSide()_
  - _getResult()_
  - _hasReserve()_
- Add default ctors and a dtor for _BattleState_
- Each of the registry configurations have a _getRuntimeRegistry()_ that will call _getRegistry()_
- Add a basic _battleState.cpp_ that holds a default dtor to resolve build warnings
- Add a _RegistryProvider_ to _main.cpp_ and initialize it will the other registries and create a _BattleEngine_ from it and the _EffectRegistry_

### Changed

- Extract shared namespace usage into a using statement in _multiplierRegistryConfiguration.h_

### Fix

- Fix _clang-tidy_ warnings on _activeSlots()_ in _battleHelpers.cpp_ by wrapping it in a _GCC_ diagnostic push

## [0.10.2] - 2026-08-13

### Added

- Add a _SwitchAction_ struct that describes an active _Pokemon_ selecting a switch during a turn
- Added a _battleValidation.h_ file that:
  - Holds a _BattleEngineError_ enum of errors that can occur during a battle
  - Holds a _BattleResult_ enum to show whether a battle is ongoing or who has won
  - Holds a _BattlePhase_ enum to determine what state the battle is in

### Changed

- Moved _BattleTarget_ and _MoveAction_ from _battleHelpers.h_ to _battleAction.h_

## [0.10.1] - 2026-08-13

### Changed

- Ran _clang-format_ on _fixedMetadataRegistry.benchmark.cpp_

### Removed

- Removed the effect registry from the _RegistryProvider_
- Removed the nullptr effect registry in all the tests that mock a _RegistryProivder_

## [0.10.0] - 2026-08-12 (Effect Registry Update)

### Added

- Add a configuration constant that holds the maximum amount of effects that the registry can hold
- Add a duplicate effect and effect not found error code with their branches implemented in _errorKindToString()_
- Added a new file to hold the built-in Effect IDs and a function to convert built-in Effect IDs to the stable Effect ID
- Added a _Effect/constants.h_ file that holds the user readable names for each Effect ID.
- Add a file that holds the stable Effect ID
- Added a metadata file for effects that holds the relevant:
  - User defined name, stable ID, and effect function to apply
- Added a file that holds the effect registry.
  - The actual registry class holds a constructor that initializes all the builtin effects:
  - Get the effect metadata
  - Get the effect ID
  - Get the effect name
  - Get a non-owning span of all the effects
  - Get the next effect ID
  - Find the internal array index by stable effect ID
  - If the registry has an effect:
    - By user readable name
    - By stable effect ID
  - Set the next effect ID
  - Increment the next effect ID
- Added in both the source and header files to configure the effect registry.
  - It holds a struct that contains the configuration policy.
  - The actual registry configuration class allows the user to:
    - Get the effect metadata
    - Get the Effect ID
    - Get the effect name
    - Get a non-owning span of all the effects
    - Get the amount of effects registered
    - If the registry has a effect:
      - By user readable name
      - By stable Effect ID
    - Add a effect
    - Add in a list of effects
    - Rename a effect
    - Update a effect:
      - By user readable name
      - By stable Effect ID
    - Remove a effect:
      - By user readable name
      - By stable Effect ID
- For each of the handlers, add an inline method that can be tied to _EffectMeta_
- Add handlers for setting:
  - _rain_
  - _sandstorm_
  - _sun_
- Add empty implementations for these effects:
  - _flinchHandler_
  - _psychicTerrainPriorityBlockHandler_
  - _recoilHandler_
  - _statusApplyHandler_
  - _statusRemoveHandler_
  - _statusTickHandler_
  - _statusTurnSkipHandler_
- Add the effect registry to the _RegistryProvider_
- Added in the effect registry configuration objects to _main.cpp_
- Add a nullptr effect registry in all the tests that mock a _RegistryProivder_

### Changed

- Change all occurences of _EffectTypeID_ to _BuiltinEffectID_

### Removed

- Removed _effectType.h_
- Removed _statusChangeEffects()_

## [0.9.19] - 2026-08-07

### Changed

- Change _ATTR\_PURE_ to _ATTR\_CONST_ on:
  - Non const overload of _activeSlots()_
  - Non const overload of _party()_

## [0.9.18] - 2026-08-07

### Changed

- Change _ATTR\_PURE_ to _ATTR\_CONST_ on:
  - Const overload of _activeSlots()_
  - Const overload of _party()_

## [0.9.17] - 2026-08-07

### Added

- Added _ATTR\_PURE_ to:
  - Both overloads of _activeSlots()_
  - _contextSlot_
  - Both overloads of _party()_
  - Both overloads of _isHealthy()_
  - _isActive()_
  - _isAdjacent()_
  - _targetExists()_
  - _sideHasHealthyPokemon()_
  - _getEffectiveSpeed()_
  - _makeMoveContext()_

## [0.9.16] - 2026-08-07

### Added

- Add a _BattleTarget_ struct that identifies an active battle slot that is selected as a target
- Add a _MoveAction_ struct that describes an active _Pokemon_ selecting a move during a turn
- Add in more helpers for the eventual battle between _Pokemon_
  - _contextSlot()_
  - _getEffectiveSpeed()_
  - _makeMoveContext()_
  - _applyRecoil()_
  - _resolveHitCount()_
- Add if the _BattleSlot_ has had the faint processed
- Add an accuracy check before hit trigger on the built in moves
- Create an empty battle engine source file

### Changed

- Apply _clang-format_ on:
  - _abilityRegistryConfiguration.h_
  - _abilityRegistryConfiguration.cpp_
  - _itemRegistryConfiguration.h_
  - _moveRegistryConfiguration.h_
  - _pokemon.h_
- Change all occurences of _float_ to _double_ in _cache.h_
- Change the base stage, accuracy, and evasion multipliers from _float_ to _double_
- Chane the early return in _applyMultiplier()_ to normalize to the max of the multiplier and 1
- Change the intermediate accuracy variable type from _float_ to _double_ in _accuracyCheckHandler.cpp_
- Change the attack mult and defense mult type from _float_ to _double_ in _baseDamageHandler.cpp_
- Update the _moveMeta.test.cpp_ and _moveRegistry.test.cpp_ tests with the changes to common _EffectTypeID_ helpers

### Fixed

- Fixed the damage clamp calculation in _effectContext.test.cpp_

### Removed

- Removed the speed boost member variable in _BattleSlot_
- Remove the _F_ suffix in _accuracyCheckHandler.cpp_
- Remove _AccuracyCheck_ from each of the common _EffectTypeID_ arrays
- Remove the _protectEffects()_ and _fieldEffectEffects()_ implementation and their tests

## [0.9.15] - 2026-08-05

### Added

- Add early return in _applyMultiplier()_ if any of the active multipliers are <= 0
- Add tests for the _Pokemon_ health bounds

### Changed

- Changed the move priority variable from unsigned byte to signed byte

## [0.9.14] - 2026-08-05

### Added

- Add in helpers for the eventual battle between _Pokemon_:
  - _getOppositeSide()_
  - _getSideOrder()_
  - Const and non-const variants of _activeSlots()_
  - Const and non-const variants of _party()_
  - _isHealthy()_ with a _BattleSlot_
  - _isHealthy()_ with a _Pokemon_
  - _isActive()_
  - _isAdjacent()_
  - _targetExists()_
  - _sideHasHealthyPokemon()_
- Add in a shared:
  - _BattleTriggerID_
  - _BattleTargetID_
  - _BattleRangeID_
- Add max health member variable with their getter and setter in _Pokemon_ and add it to the constructors

### Changed

- Change all occurences of the following to _BattleTriggerID_:
  - _AbilityTriggerID_
  - _ItemTriggerID_
  - _MoveTriggerID_
- Change all occurences of the following to _BattleTargetID_:
  - _AbilityTargetID_
  - _ItemTargetID_
  - _MoveTargetID_
- Change all occurence of _MoveRangeID_ to _BattleRangeID_
- Change the move base power variable type from unsigned byte to unsigned short

### Removed

- Removed _AbilityEffectTrigger_
- Removed _ItemEffectTrigger_
- Removed _MoveEffectTrigger_
- Deleted _abilityTargetsAndTriggers.h_
- Deleted _itemTargetsAndTriggers.h_
- Deleted _moveTargetsAndTriggers.h_

## [0.9.13] - 2026-08-04

### Added

- Add a configuration constant that specifies how many suppression rules can be tied per target
- Added a suppresion rule struct that holds:
  - An optional ability ID
  - An optional item ID
  - An optional move ID
  - A variant of the ability, item, and move trigger
  - The effect source
- Added the following to _AbilityEffectTrigger_, _ItemEffectTrigger_, and _MoveEffectTrigger_:
  - An array of suppression rules
  - The amount of suppression rules actually in use

### Changed

- Moved _AbilityEffectTrigger_ from _abilityTargetsAndTriggers.h_ to _abilityMeta.h_
- Moved _ItemEffectTrigger_ from _itemTargetsAndTriggers.h_ to _itemMeta.h_
- Moved _MoveEffectTrigger_ from _moveTargetsAndTriggers.h_ to _moveMeta.h_
- Moved the _EffectSource_ enum from _effectContext.h_ to _effectSourceAndSuppression.h_
- Run _clang-format_ on _typeRegistryConfiguration.cpp_

## [0.9.12] - 2026-08-04

### Added

- Added an _OnSuccessfulHit_ to _AbilityTriggerID_

### Changed

- Changed the _Flinch_ ability to trigger _OnSuccessfulHit_ instead of _OnDamageCalc_ and update the test

## [0.9.11] - 2026-08-04

### Added

- Add a configuration constant that specifies how many statuses a _Pokemon_ can have
- Add if the item is consumable to _ItemMeta_
- Add a getter and setter to grab all the statuses a _Pokemon_ has
- Add in a method to add a status to the _Pokemon_
- Add _ATTR\_NOINLINE_ to _addBuiltin()_
- Add that _CheriBerry_ and _ChestoBerry_ are consumable in _itemRegistry.h_
- Add the status interactions on each of the built in statuses
- Add a file that helps with status interactions:
  - _hasInteraction()_
  - _willBlockIncoming()_
  - _statusAlreadyExists()_
  - _statusReplaceHandler()_
  - _statusRemoveHandler()_
  - _shiftAndGetNextAvailableStatus()_
- Add a _StatusInteractionAction_ that will specify how the statuses will interact with each other
- Add a struct that will hold the _StatusID_ and the _StatusInteractionAction_
- Add a vector of status interactions in _StatusMeta_
- Add tests for the statuses in _pokemon.test.cpp_

### Changed

- Reorder member variables in _BattleSlot_
- Rename _mForceGrounded_ to _mIsGrounded_ and update all call sites
- Change the _Pokemon_ class to have an array of statuses instead of just one _StatusID_
- Update _getStatusID()_ to take in an array index parameter
- Update _burnDamageHandler.cpp_ to use the new status helper functions

### Removed

- Removed the variable in _BattleSlot_ that says if the item was consumed
- Removed the _setStatus()_ method

## [0.9.10] - 2026-08-04

### Added

- Add _Levitate_ and _Elevate_ to the _builtInAbilityID_
- Add _AirBalloon_  to the _builtInItemID_
- Add a value to _BattleSlot_ to force it to be grounded
- Add in new confguration constants:
  - The electric buff in electric terrain base damage value
  - The grass buff in grassy terrain base damage value
  - The psychic buff in psychic terrain base damage value
  - The dragon debuff in misty terrain base damage value
- Add more functions to _effectHandlerHelpers.h_
  - _battleSlotHasType()_
  - _battleSlotHasAbilityByName()_
  - _battleSlotHasAbilityByID()_
  - _battleSlotHasItemByName()_
  - _battleSlotHasItemByID()_
  - _isBattleSlotUngrounded()_
  - _isBattleSlotGrounded()_
- Add _getConstUserBattleSlot()_ and _getConstTargetBattleSlot()_
- Added a source and header terrain handler which inherits from _IEffectHandler_ that will calculate the terrain part of the base damage multiplier calculation.
- Add a test suite for _terrainHandler.h_

### Changed

- Update the Ability constants to be inline
- Change the _DamageFormulaModifiers_ members to be _double_ instead of _float_
- Remove the constness of the _BattleState_ parameter in all _apply_ functions
- _getTeamConst()_ takes the _BattleState_ by reference instead of const reference now
- Update the intermediate _float_ calculations to be _double_ in _baseDamageHandler.cpp_
- The following files now call the const methods instead of the non-const:
  - _accuarcyCheckHandler.cpp_
  - _baseDamageHandler.cpp_
  - _stabHandler.cpp_
  - _typeEffectivenessHandler.cpp_

## [0.9.9] - 2026-08-04

### Fix

- Add missing header in _weatherHandler.cpp

## [0.9.8] - 2026-08-04

### Added

- Add _AirLock_ and _CloudNine_ to the _builtInAbilityID_
- Add _HydroSteam_ to the _builtInMoveID_
- Added a _cloneMetadata()_ method to prevent inlining warnings
- Add in new confguration constants:
  - The fist hit multiplier value for population bomb
  - The consecutive hit multiplier value for population bomb
  - The weather nullification multiplier value
  - The water move in rain multiplier value
  - The fire move in rain multiplier value
  - The fire move in harsh sunlight multiplier value
  - The water move in harsh sunlight multiplier value
- Added a source and header population bomb handler which inherits from _IEffectHandler_ that will calculate the population bomb part of the damage calculation from Bulbapedia.
- Added a source and header weather handler which inherits from _IEffectHandler_ that will calculate the weather part of the damage calculation from Bulbapedia.
- Add a move hit policy file that can differentiate between fixed number of hits and a weighted hit outcome
- Add the following to _MoveTriggerID_
  - _BeforeHit_
  - _OnHit_
  - _AfterHit_
- Add _PopulationBomb_ to the _builtInMultiplierID_
- Add user readable name for _PopulationBomb_ and _Targets_
- Add the built in population bomb and targets multiplier

### Changed

- Change the weather ID and terrain ID variables in _BattleState_ to have a suffix of _ID_ now
- Update documentation for _moveMeta.h_
- Update imports for _moveMeta.h_
- Change the hit distribution variable to be a hit count policy variable
- Update both built in moves trigger from _OnTarget_ to _OnHit_ and their tests
- Changed the _Types_ enum to be called _BuiltInTypeID_ and update all call sites
- Update _moveRegistryConfiguration.cpp_ to initialize each member variable instead of doing a copy

### Fixed
- Renamed the current hit variable in _EffectContext_
- Fix _clang-tidy_ warning in _randomizationHandler.cpp_
- Add forgotten header in _random.test.cpp_

### Removed

- Removed the total hit count in _EffectContext_
- Removed the _OnTarget_ move trigger ID

## [0.9.7] - 2026-08-03

### Added

- Add in the total hits and current hit in _EffectContext_
- Add a hit distribution in _MoveMeta_
- Added a random number getter that will do decimal values
- Add in new tests to cover the decimal random number getter

### Changed

- Add brace initialization to _mTriggers_ in _MoveMeta_

## [0.9.6] - 2026-08-03

### Added

- Add in new confguration constants:
 - The randomization minimum roll
 - The randomization maximum roll
- Add _Randomization_ to the _builtInMultiplierID_
- Add user readable name for _Randomization_
- Add the built in randomization multiplier
- Added a source and header burn handler which inherits from _IEffectHandler_ that will calculate the burn part of the damage calculation from Bulbapedia.

### Changed

- Updated _CRITICAL\_HIT\_MULTIPLIER_ to _CRITICAL\_HIT\_MULTIPLIER\_VALUE_ and all the call sites
- Updated _STAB\_HIT\_MULTIPLIER_ to _STAB\_HIT\_MULTIPLIER\_VALUE_ and all the call sites
- Updated _STAB\_HIT\_MULTIPLIER_ to _STAB\_HIT\_MULTIPLIER\_VALUE_ and all the call sites
- Updated _TARGETS\_HIT\_MULTIPLIER_ to _TARGETS\_HIT\_MULTIPLIER\_VALUE_ and all the call sites

### Removed

- Removed the _ATTR\_MAYBE\_UNUSED_ and _attributeMacros.h_ from:
  - _typeEffectivenessHandler.h_
  - _typeEffectivenessHandler.cpp_

## [0.9.5] - 2026-08-03

### Added

- Add _Guts_ to the _builtInAbilityID_
- Add user readable name for _Guts_
- Add _Facade_ to the _builtInMoveID_
- Add _Burn_ to the _builtInMultiplierID_
- Add user readable name for _Burn_
- Add the built in burn multiplier
- Add in a new configuration constant that will set how much the damage is affected when the _Pokemon_ is burned
- Added a source and header burn handler which inherits from _IEffectHandler_ that will calculate the burn part of the damage calculation from Bulbapedia.

## [0.9.4] - 2026-08-03

### Added

- Add in new configuration constants:
  - The value of a not very effective move hit
  - The value of an effective move hit
  - The value of a super effective hit
  - The value of a no effective hit
- Added a function to convert a _TypeEffectiveness_ to one of the configuration constants

### Changed

- Update the following variables from _float_ to _double_
  - _BASE\_MULTIPLIER\_VALUE_
  - _CRITICAL\_HIT\_MULTIPLIER\_VALUE_
  - _STAB\_HIT\_MULTIPLIER\_VALUE_
  - _TARGETS\_HIT\_MULTIPLIER\_VALUE_
  - _FIXED\_POINT\_MULTIPLIER\_NUMERATOR_
  - _FIXED\_POINT\_MULTIPLIER\_DENOMINATOR_
- Update _effectContext.h_ to change the following from _float_ to _double_
  - _setMultiplier()_
  - _getActiveMultiplier()_
  - _mActiveMultipliers_
- Update _typeEffectivenessHandler.cpp_ to use the _getEffectivenessValue()_ function instead of having the code be commented out

### Fixed

- Add missing import to:
  - _typeRegistryConfiguration.cpp_
  - _typeRegistryConfiguration.test.cpp_
  - _typeRegistry.test.cpp_

### Remove

- Remove the _F_ suffix in:
  - _effectContext.test.cpp_
  - _criticalHitHandler.test.cpp_
  - _stabHandler.test.cpp_
  - _targetsHandler.test.cpp_

## [0.9.3] - 2026-08-03

### Changed

- Update the following to use _TypeEffectiveness_: instead of _TypeEffectivenessID_:
  - _typeRegistryConfiguration.h_
  - _typeRegistryConfiguration.cpp_
  - _typeRegistryConfiguration.test.cpp_
  - _typeRegistry.h_
  - _typeRegistry.test.cpp_
  - _Types/constants.h_
- Renamed _builtInTypeEffectivenessID.h_ to  _typeEffectiveness.h_
- Update imports of _builtInTypeEffectivenessID.h_ to  _typeEffectiveness.h_

### Removed

- Removed the type effectiveness registry
- Removed the source and header file for the type effectiveness configuration
- Removed the configuration constant that specified how many type effectiveness values the registry could hold
- Removed the multiplier value that was targeted towards type effectiveness
- Removed the builtin multiplier value for type effectiveness
- Removed the type effectiveness metadata file
- Removed the type effectiveness provider in:
  - _registryProvider.h_
  - _accuracyCheckHandler.test.cpp_
  - _baseDamageHandler.test.cpp_
  - _criticalHitHandler.test.cpp_
  - _stabHandler.test.cpp_
  - _targetsHandler.test.cpp_

## [0.9.2] - 2026-08-03

### Added

- Add a configuration constant that holds the maximum amount of type effectiveness that the registry can hold
- Add a duplicate type effectiveness and type effectiveness not found error code with their branches implemented in _errorKindToString()_
- Added a file that holds the type effectiveness registry.
  - The actual registry class holds a constructor that initializes all the builtin type effectiveness':
  - Get the type effectiveness metadata
  - Get the type effectiveness ID
  - Get the type effectiveness name
  - Get a non-owning span of all the type effectiveness'
  - Get the next type effectiveness ID
  - Find the internal array index by stable type effectiveness ID
  - If the registry has an type effectiveness:
    - By user readable name
    - By stable type effectiveness ID
  - Set the next type effectiveness ID
  - Increment the next type effectiveness ID
- Added in both the source and header files to configure the type effectiveness registry.
  - It holds a struct that contains the configuration policy.
  - The actual registry configuration class allows the user to:
    - Get the type effectiveness metadata
    - Get the Type Effectiveness ID
    - Get the type effectiveness name
    - Get a non-owning span of all the type effectivenesss
    - Get the amount of type effectivenesss registered
    - If the registry has a type effectiveness:
      - By user readable name
      - By stable Type Effectiveness ID
    - Add a type effectiveness
    - Add in a list of type effectivenesss
    - Rename a type effectiveness
    - Update a type effectiveness:
      - By user readable name
      - By stable Type Effectiveness ID
    - Remove a type effectiveness:
      - By user readable name
      - By stable Type Effectiveness ID
- Add a new type effectiveness multiplier value _Multiplier/constants.h_
- Add a new built in multiplier for type effectiveness
- Add the type effectiveness provider in:
  - _registryProvider.h_
  - _accuracyCheckHandler.test.cpp_
  - _baseDamageHandler.test.cpp_
  - _criticalHitHandler.test.cpp_
  - _stabHandler.test.cpp_
  - _targetsHandler.test.cpp_
- Add builtin _TypeEffectivenessID_ values to _Types/constants.h_
- Added a metadata file for type effectiveness that holds the relevant:
  - User defined name, stable ID, and the effectiveness value
- Added a source and header type effectiveness handler which inherits from _IEffectHandler_ that will calculate the type effectiveness part of the damage calculation from Bulbapedia.

### Changed

- Renamed _builtinAbilityID.h_ to _builtInAbilityID.h_
- Update imports of _builtinAbilityID.h_ to _builtInAbilityID.h_
- Renamed _builtinMultiplierID.h_ to _builtInMultiplierID.h_
- Update imports of _builtinMultiplierID.h_ to _builtInMultiplierID.h_
- Renamed _builtinItemID.h_ to _builtInItemID.h_
- Update imports of _builtinItemID.h_ to _builtInItemID.h_
- Renamed _builtinMoveID.h_ to _builtInMoveID.h_
- Update imports of _builtinMoveID.h_ to _builtInMoveID.h_
- Renamed _builtinStatusID.h_ to _builtInStatusID.h_
- Update imports of _builtinStatusID.h_ to _builtInStatusID.h_
- Renamed _builtinTerrainID.h_ to _builtInTerrainID.h_
- Update imports of _builtinTerrainID.h_ to _builtInTerrainID.h_
- Renamed _builtinWeatherID.h_ to _builtInWeatherID.h_
- Update imports of _builtinWeatherID.h_ to _builtInWeatherID.h_
- Renamed _types.h_ to _builtInTypeID.h_
- Update imports of _types.h_ to _builtInTypeID.h_
- Renamed _typeEffectiveness.h_ to _builtInTypeEffectivenessID.h_
- Update imports of _typeEffectiveness.h_ to _builtInTypeEffectivenessID.h_
- Update the header guard of _typeID.h_
- Update _Detail::TypeIDTag_ to _Detail::TypeID_
- Moved _toTypeID()_ from _typeID.h_ to _builtInTypeID.h_
- Update the following to use _TypeEffectivenessID_ instead of _TypeEffectiveness_:
  - _typeRegistryConfiguration.h_
  - _typeRegistryConfiguration.cpp_
  - _typeRegistryConfiguration.test.cpp_
  - _typeRegistry.h_
  - _typeRegistry.test.cpp_
  - _Types/constants.h_

### Removed

- Removed _ccache_ from:
  - _codeql-analysis.yml_
  - _testing.yml_
  - _Makefile_
  - _makefileDependencies.sh_
- Remove useless imports in _typeID.h_

## [0.9.1] - 2026-08-03

### Added

- Add documentation to:
  - _IDIndexEntry_
  - Member variables of _IDIndexEntry_
  - _findEntryIndexByName()_
  - _findEntryIndexByID()_
  - _insertIDIndex()_
  - _removeIDIndex()_
  - _idIndexEntryLess()_
  - _rebuildIDIndex()_
  - Member variables of _FixedMetadataRegistry_
  - _RegistryProvider_
  - Member variables of _RegistryProvider_
  - _Types_

### Changed

- Update the following to use _@_ doxygen comments instead of _\\_:
  - _attributeMacros.h_
  - _configCat.h_
  - _types.h_
  - _contiguousSequence.h_
  - _overflowProtection.h_

## [0.9.0] - 2026-08-03 (Type Registry with Fixed Metadata Registry Update)

### Added

- Add a mutable and const read-only registry access in _fixedMetadataRegistryConfiguration.h_
- Add a new teplate variable on _FixedMetadataRegistry_ that holds the _NameMember_

### Changed

- Add _ATTR\_PURE_ to:
  - _statStageCacheIndex()_
  - _findIndexByID()_
  - _StableID_ _findEntryIndexByName()_
  - _getActiveMultipliers()_
- Add _ATTR\_NOINLINE_ to:
  - _eraseEntry()_ 
  - _string_view_ _findEntryIndexByName()_
  - _TypeRegistry()_
  - _span_ _addBuiltin_
- Update _TypeEntry_ to have a _m_ prefix on member variables and update all call sites to it
- Update _insertIDIndex_ to get the indexed entries and then use it in the rest of the function instead of _mIDIndex_
- Update _removeIDIndex_ to get the indexed entries and then use it in the rest of the function instead of _mIDIndex_
- Update _rebuiltIDIndex_ to get the indexed entries and then use it in the rest of the function instead of _mIDIndex_
- Update documentation for:
  - _typeRegistryConfiguration.h_
  - _typeRegistry.h_
- Integrate _typeRegistryConfiguration.h_ with _fixedMetadataRegistryConfiguration.h_
  - _getTypeID()_ calls _getID()_
  - _getTypeName()_ calls _getName()_
  - _getRegisteredTypes()_ calls _getRegisteredEntries()_
  - _getAmountRegistered()_ calls _getAmountRegistered()_
  - Both overloads of _hasType()_ call _hasEntry()_
- Integrate _typeRegistry.h_ with _fixedMetadataRegistry.h_
  - _getEntry()_ calls _getEntry()_
  - _getTypeID()_ calls _getID()_
  - _getTypeName()_ calls _getName()_
  - _getAmountRegistered()_ calls _getAmountRegistered()_
  - _getNextTypeID()_ calls _getNextID()_
  - _getRegisteredTypes()_ calls _getRegisteredEntries()_
  - _setEntry()_ calls _setEntry()_
  - _setAmountRegistered()_ calls _setAmountRegistered()_
  - _setNextTypeID()_ calls _setNextID()_
  - _findIndexByTypeID()_ calls _findIndexByID()_
  - Both overloads of _hasType()_ call _hasEntry()_
  - _incrementNextTypeID()_ calls _incrementNextID()_
  - _incrementAmountRegistered()_ calls _incrementAmountRegistered()_
  - _decrementAmountRegistered()_ calls _decrementAmountRegistered()_
  - _addBuiltin()_ calls _addBuiltin()_
- Update _clearRows()_ to get the registry by _getRegistry()_ instead of using the member variable
- Update _typeRegistryConfiguration.cpp_ to call _getRegistry()_ instead of using the member variable
- Run _clang-format_ on:
  - _fixedMetadataRegistry.h_
  - _moveRegistry.h_
  - _multiplierRegistryConfiguration.cpp_
  - _terrainRegistryConfiguration.cpp_
  - _multipliersRegistryConfiguration.test.cpp_
  - _statusRegistryConfiguration.test.cpp_
  - _terrainRegistryConfiguration.test.cpp_
  - _weatherRegistryConfiguration.test.cpp_
  - _accuracyCheckHandler.test.cpp_
  - _baseDamageHandler.test.cpp_
- Fix _clang-tidy_ warnings in:
  - _itemRegistryConfiguration.h_
  - _effectContext.cpp_
  - _accuracyCheckHandler.cpp_
  - _stabHandler.cpp_
  - _moveMeta.cpp_
  - _abilityRegistryConfiguration.test.cpp_
  - _itemRegistryConfiguration.test.cpp_
  - _moveRegistryConfiguration.test.cpp_
  - _multipliersRegistryConfiguration.test.cpp_
  - _statusRegistryConfiguration.test.cpp_
  - _terrainRegistryConfiguration.test.cpp_
  - _typeRegistryConfiguration.test.cpp_
  - _weatherRegistryConfiguration.test.cpp_
  - _effectContext.test.cpp_
  - _accuracyCheckHandler.test.cpp_
  - _baseDamageHandler.test.cpp_
  - _criticalHitHandler.test.cpp_
  - _effectHandlerInterface.test.cpp_
  - _stabHandler.test.cpp_
  - _targetsHandler.test.cpp_
  - _moveMeta.test.cpp_
  - _logger.test.cpp_
  - _random.test.cpp_
- Extract namespace into a using statement in:
  - _abilityRegistryConfiguration.h_
  - _itemRegistryConfiguration.h_
  - _moveRegistryConfiguration.h_
  - _statusRegistryConfiguration.h_
  - _terrainRegistryConfiguration.h_
  - _typeRegistryConfiguration.h_
  - _weatherRegistryConfiguration.h_2
- Update _getName()_ to return the template _NameMember_ instead of _mName_

### Remove

- Removed the member registry variable in _typeRegistryConfiguration.h_
- Removed the following in _typeRegistry.h_
  - _findEntryIndexByName()_
  - _findEntryIndexById()_
  - _mEntries_
  - _mAmountRegistered_
  - _mNextTypeID_

## [0.8.7] - 2026-08-03

### Added

- Add a warnings variable in the _Makefile_
- Add _main.cpp_ to the list of excluded files when compiling benchmark targets
- Add a benchmark file to see how the multiplier functionality of _EffectContext_ performs
- Add a benchmark file to see how combined functions of _EffectTypeID_ perform
- Add a benchmark file to see how finding move id's perform at full capacity
- Add a _statStageCacheIndex()_ function to be used in the other cache functions
- Add a method to get a non-owning read-only view of all active multipliers in the _EffectContext_
- Add an array that will map the built-in multiplier values to their position in the active multipliers spare vector
- Add two functions that get the user and target _BattleSlot_ from the _EffectContext_
- Add _ATTR\_CONST_ to each of the common _EffectTypeID_ functions in _moveMeta.h_
  - Also make them return a non-owning read-only span instead of a vector
  - Also make them all _noexecpt_
- Add the _Base::eraseEntry_ functionality in:
  - _abilityRegistry.h_
  - _itemRegistry.h_
  - _moveRegistry.h_
  - _multiplierRegistry.h_
  - _statusRegistry.h_
  - _terrainRegistry.h_
  - _weatherRegistry.h_
- Add an _eraseEntry()_ function to _fixedMetadataRegistry.h_
- Add a struct that will hold an ID index entry which has a stableID and the entry index position
- Add an array of sorted stable ID indexes used for lookup and a variable to track how many valid entries are in it
- Add _insertIDIndex()_ that will insert an ID-index mapping while preserving sorted order
- Add _removeIDIndex()_ that will remove an ID-index mapping and compact remaining mappings
- Add _idIndexEntryLess()_ which will order two ID-index entries by _StableID_ then entry index
- Add _rebuildIDIndex()_ that will rebuilt and resort the stable ID index from registered entries
- Add _lcov_ exclusions to _typeRegistryConfiguration.cpp_ on branches that aren't reachable
- Add a check to see if the attack stat or defense stat is infinite and if either is less than 0 in _baseDamageHandler.cpp_

### Changed

- Change the warnings when compiling bechmarks to use the benchmark warnings explicitly
- Change the following functions to use _statStageCacheIndex()_ instead of using the same logic:
  - _STAT\_STAGE\_MULTIPLIERS_
  - _ACCURACY\_STAGE\_MULTIPLIERS_
  - _EVASION\_STAGE\_MULTIPLIERS_
- Change the _MAX\_CRITICAL\_HIT\_VALUE_ from _100_ to _99_ due to 0-based counting
- Change _removeEntry()_ to call _eraseEntry()_
- Update documentation in _fixedMetadataRegistry.h_
- _incrementAmountRegistered()_ will insert the ID index into the entries
- _decrementAmountRegistered()_ will remove the ID index into the entries
- _addBuiltin()_ will now call _incrementAmountRegistered()_ instead of manually increasing the member variable
- _findEntryIndexByID()_ will now do a binary-search lookup on the sorted stable ID index
- Rebuild the id index on:
  - _setEntry()_
  - _setAmountRegistered_
- Update _moveRegistry.h_ effect initialization by using _.begin()_ and _.end()_ iterators
- Update both functions in _contiguousSequence.h_ with early returns
- Update the unexpected return of _resetMatchups()_
- Update _setMultiplier()_ to process in built multipliers at _O(1)_ by using the built in multipliers position otherwise it defaults to the _O(n)_ loop 
- Update _resetMultipliers()_ to also fill the built in multiplier positions with _0U_
- Update the float typings to double in _baseDamageHandler.cpp_
- Update the following functions to call the battle slot helper functions from _IEffectHandler_:
  - _accuracyCheckHandler.cpp_
  - _baseDamageHandler.cpp_
  - _stabHandler.cpp_
- Apply _clang-format_ to:
  - _accuracyCheckHandler.cpp_
  - _baseDamageHandler.cpp_
- Update includes in _moveMeta.cpp_
- Update the following to fully cover all lines and branches
  - _abilityRegistryConfiguration.test.cpp_
  - _itemRegistryConfiguration.test.cpp_
  - _typeRegistryConfiguration.test.cpp_
  - _typeRegistryConfiguration.test.cpp_
  - _typeRegistry.test.cpp_
  - _contiguousSequence.test.cpp_
  - _logger.test.cpp_
- Add a test suite for:
  - _moveRegistryConfiguration.h_
  - _multiplierRegistryConfiguration.h_
  - _statusRegistryConfiguration.h_
  - _terrainRegistryConfiguration.h_
  - _weatherRegistryConfiguration.h_
  - _effectContext.h_
  - _accuracyCheckHandler.h_
  - _baseDamageHandler.h_
  - _criticalHitHandler.h_
  - _effectHandlerInterface.h_
  - _stabHandler.h_
  - _targetsHandler.h_
  - _moveMeta.h_
  - _input.h_
  - _random.h_

### Fixed

- Change how _addMetadataBatch()_ works by reverting the registry to what it was before calling the function if any addtion to the registry fails
- Change how _setMatchupRow()_ works by reverting the registry to what it was before calling the function if any addtion to the registry fails
- Change how _setDefensiveColumn()_ works by reverting the registry to what it was before calling the function if any addtion to the registry fails
- Fix both uses of _mutateMetadata()_ to not allow the mutation update if the new name is already in use in the registry
- Fix the asserts in _findEntryIndexByID()_ and _findEntryIndexByName()_ in _typeRegistry.h_ to be _<=_ instead of _<_
- Fix _input.h_ to not use a redundant _T::value\_type_
- Fix the damage calc done in _applyMultiplier()_
- Clamp the resulting damage from _baseDamageHandler.cpp_ to _1.0_, and the numeric limit of an unsigned short
- Fix the critical hit percentage check to be _<_ instead of _<=_ for a true accuracte percentage instead of being percentage + 1

### Removed

- Remove redunant includes in _Configuration/constants.h_

## [0.8.6] - 2026-07-31

### Removed

- Removed _EffectResult_ struct and the member variable of it in _EffectContext_
- Removed if protected in _DamageContext_

## [0.8.5] - 2026-07-31

### Added

- Added a _DamageFormulaModifiers_ that will apply to each of the 6 basic stats
  - Add it to each _BattleSlot_ 
- Use the _DamageFormulaModifiers_ in _baseDamageHandler_ to update the attack/special attack and defense special/defense calcs

### Changed

- Default initialize the sparse multiplier list in _EffectContext_

## [0.8.4] - 2026-07-31

### Changed

- Convert the setters in _Pokemon_ to be constexpr and bring the implemention into _pokemon.h_
- Updated the tests in _pokemon.test.cpp_ to include the level of the _Pokemon_ when creating them

### Removed

- Removed _pokemon.cpp_

## [0.8.3] - 2026-07-31

### Added

- Add new configuration constant variables:
  - The numerator for the fixed point multiplier
  - The denominator for the fixed point multiplier
  - The threshold of when to round down when using the fixed point multiplier
  - The threshold of tolerance for rounding down due to how doubles are stored in memory when using the fixed point multiplier
- Add a function _applyMultiplier()_ to _EffectContext_
  - This will put all multipliers through a fixed point calculation before multiplying it with the base damage
- Add in a _Weather_ multiplier ID
- Add in the user readable name for the weather multiplier ID
- Add in the built in weather multiplier
- Add level to the _Pokemon_ constructors and call _setLevel()_ in the constructor body

### Removed

- Removed the combined multiplier variable in _EffectContext_
- Removed the combined multiplier variable use in _setMultiplier()_ and _resetMultipliers()_

## [0.8.2] - 2026-07-30

### Added

- Add new configuration constant variables:
  - The numerator for the evasion stat stage multiplier
  - The denominator for the evasion stat stage multiplier
  - The numerator for the level damage factor
  - The denominator for the level damage factor
  - The offset for the level damage factor
  - The amount of stages a stat can have
  - The maximum size of the cache for reach caching function.
    - This is based on the amount of stages a stat can have.
- Add a cache file that holds three stat stage multipliers that are cached so fast lookup.
  - The base 6 stats
  - Accuracy
  - Evasion
- Add a _RegistryProvider_ struct that will hold non-owning pointers to all other registries
- Added a _setMultiplier()_ and _resetMultipliers()_ functions to _EffectContext_
- Added a float to hold the combined multiplier that will be added at the end of the damage calculation in _EffectContext_
- For the following handlers (both source and header), add a _RegistryProvider_ parameter:
  - _effectHandlerInterface_
  - _accuracyCheckHandler_
  - _baseDamageHandler_
  - _criticalHitHandler_
  - _stabHandler_
  - _targetsHandler_
- Add a new move trigger ID for _OnTarget_
- Add a level damage factor into the _Pokemon_ class for pseudo-caching and also add a getter for this variable.

### Changed

- Update the type of the parties in _BattleSlot_ from _const Pokemon *_ to _Pokemon *_
- Change the multipliers from an _unordered\_map_ to a sparse multiplier vector in _EffectContext_
- Change both _Pound_ and _Karate Chop_ to have the move trigger _OnTarget_ instead of _OnUse_
- Update _accuracyCheckHandler_ to use the cached _evasion_ and _accuracy_ stage multipliers instead of calculating them on every call
- Update _baseDamageHandler_ to use the cached _stat_ stage multipliers instead of calculating them on every call
- Update _baseDamageHandler_ to get the pseudo-cached level damage factor from the pokemon to not recalculate it on every call to the handler 
- Update _criticalHitHandler_ to call the _setMultiplier()_ method instead of modifying the multiplier map
- Update _stabHandler_ to call the _setMultiplier()_ method instead of modifying the multiplier map
- Update _targetsHandler_ to use the _RegistryProvider_ instead of using the member variable of the _MoveRegistryConfiguration_ 
- Update _targetsHandler_ to call the _setMultiplier()_ method instead of modifying the multiplier map
- Update the _MoveRegistry_ test such that _Pound_ will check for move trigger _OnTarget_ instead of _OnUse_

### Removed

- Removed the current hp in _BattleSlot_
- Removed the _EffectContext_ constructor that base initialized the multipliers map
- Removed the _MoveRegistryConfiguration_ member variable from _TargetsHandler_

## [0.8.1] - 2026-07-30

### Added

- Add new configuration constant variables:
  - The maximum amount of multipliers the registry can hold
  - The base multiplier value
  - The same type attack bonus (STAB) hit multiplier
  - The targets hit multiplier
- Add in a duplicate multiplier and multiplier not found registry error, and add in the switch cases for these errors in _errorKingToString()_
- Added a file that holds the multiplier registry.
  - The actual registry class holds a constructor that initializes all the builtin multipliers:
  - Get the multiplier metadata
  - Get the multiplier ID
  - Get the multiplier name
  - Get a non-owning span of all the multipliers
  - Get the next multiplier ID
  - Find the internal array index by stable multiplier ID
  - If the registry has an multiplier:
    - By user readable name
    - By stable multiplier ID
  - Set the next multiplier ID
  - Increment the next multiplier ID
- Added in both the source and header files to configure the multiplier registry.
  - It holds a struct that contains the configuration policy.
  - The actual registry configuration class allows the user to:
    - Get the multiplier metadata
    - Get the Multiplier ID
    - Get the multiplier name
    - Get a non-owning span of all the multipliers
    - Get the amount of multipliers registered
    - If the registry has a multiplier:
      - By user readable name
      - By stable Multiplier ID
    - Add a multiplier
    - Add in a list of multipliers
    - Rename a multiplier
    - Update a multiplier:
      - By user readable name
      - By stable Multiplier ID
    - Remove a multiplier:
      - By user readable name
      - By stable Multiplier ID
- Create an _EffectContext_ constructor that will initialize the list of multipliers with _BASE\_MULTIPLIER\_VALUE_
- Added _Targets_ and _Stab_ in:
  - _effectType.h_
  - _moveMeta.cpp_
- Added a source and header same type attack bonus (STAB) handler which inherits from _IEffectHandler_ that will calculate the stab part of the damage calculation from Bulbapedia.
- Added a source and header targets handler which inherits from _IEffectHandler_ that will calculate the targets part of the damage calculation from Bulbapedia.
- Add a hashing override for _IDInterface_
- Added a new file to hold the built-in Multiplier IDs and a function to convert built-in Multiplier IDs to the stable Multiplier ID
- Added a _Multiplier/constants.h_ file that holds the user readable names for each Multiplier ID.
- Added a metadata file for Pokemon multipliers that holds the relevant:
  - User defined name and stable ID
- Added in the multiplier registry configuration objects to _main.cpp_
- Add the stable status ID with its getter and setter to _Pokemon_
- Add a member function to check if the _Pokemon_ is fainted
- Add in the missing source files for:
  - _terrainRegistryConfiguration.h_
  - _weatherRegistryConfiguration.h_

### Changed

- Removed the _const_ from the member pokemon variable in _BattleSlot_
- Update _EffectContext_ to no longer track each multiplier but have a stable multiplier ID.
- Reordered effect type IDs in _EffectTypeID_
- Update the documentation of _Status/constants.h_
- Update the _criticalHitHandler.cpp_ file to use the new built in multiplier ID dictionary
- Update the _moveRegistry.test.cpp_ check that _Pound_ from 9 effects to 11.

### Removed

- Removed the item ID, status ID, and if the slot is fainted from _BattleSlot_

## [0.8.0] - 2026-07-30 (Weather and Terrain Registry Update)

### Added

- Add new configuration constant variables:
  - The maximum amount of weathers the registry can hold
  - The maximum amount of terrains the registry can hold
- Add in a duplicate status, duplicate terrain, weather not found, and terrain not found registry error, and add in the switch cases for these errors in _errorKingToString()_
- Added a file that holds the terrain registry.
  - The actual registry class holds a constructor that initializes all the builtin terrains:
  - Get the terrain metadata
  - Get the terrain ID
  - Get the terrain name
  - Get a non-owning span of all the terrains
  - Get the next terrain ID
  - Find the internal array index by stable terrain ID
  - If the registry has an terrain:
    - By user readable name
    - By stable terrain ID
  - Set the next terrain ID
  - Increment the next terrain ID
- Added in the header files to configure the terrain registry.
  - It holds a struct that contains the configuration policy.
  - The actual registry configuration class allows the user to:
    - Get the terrain metadata
    - Get the Terrain ID
    - Get the terrain name
    - Get a non-owning span of all the terrains
    - Get the amount of terrains registered
    - If the registry has a terrain:
    - By user readable name
    - By stable Terrain ID
    - Add a terrain
    - Add in a list of terrains
    - Rename a terrain
    - Update a terrain:
    - By user readable name
    - By stable Terrain ID
    - Remove a terrain:
    - By user readable name
    - By stable Terrain ID
- Added a file that holds the weather registry.
  - The actual registry class holds a constructor that initializes all the builtin weathers:
  - Get the weather metadata
  - Get the weather ID
  - Get the weather name
  - Get a non-owning span of all the weathers
  - Get the next weather ID
  - Find the internal array index by stable weather ID
  - If the registry has an weather:
    - By user readable name
    - By stable weather ID
  - Set the next weather ID
  - Increment the next weather ID
- Added in the header files to configure the weather registry.
  - It holds a struct that contains the configuration policy.
  - The actual registry configuration class allows the user to:
    - Get the weather metadata
    - Get the Weather ID
    - Get the weather name
    - Get a non-owning span of all the weathers
    - Get the amount of weathers registered
    - If the registry has a weather:
    - By user readable name
    - By stable Weather ID
    - Add a weather
    - Add in a list of weathers
    - Rename a weather
    - Update a weather:
    - By user readable name
    - By stable Weather ID
    - Remove a weather:
    - By user readable name
    - By stable Weather ID
- Added a new file to hold the built-in Terrain IDs and a function to convert built-in Terrain IDs to the stable Terrain ID
- Added a _Terrain/constants.h_ file that holds the user readable names for each Terrain ID.
- Added a metadata file for Pokemon terrains that holds the relevant:
  - User defined name and stable ID
- Added a new file to hold the built-in Weather IDs and a function to convert built-in Weather IDs to the stable Weather ID
- Added a _Weather/constants.h_ file that holds the user readable names for each Weather ID.
- Added a metadata file for Pokemon weathers that holds the relevant:
  - User defined name and stable ID
- Added in the terrain and weather registry configuration objects to _main.cpp_

### Changed

- Update _BattleState_ to no longer track each weather and terrain, but have a stable weather ID and stable terrain ID.

### Removed

- Deleted _weather.h_

## [0.7.7] - 2026-07-29

### Changed

- Update documentation in _statusID.h_

## [0.7.6] - 2026-07-29

### Added

- Added _ATTR\_NOINLINE_ to _setEntry()_ in _fixedMetadataRegistry.h_

## [0.7.5] - 2026-07-29

### Added

- Add new configuration constant variables:
  - The minimum accuracy hit value that can be rolled
  - The maximum accuracy hit value that can be rolled
  - The numerator for the accuracy stat stage multiplier
  - The denominator for the accuracy stat stage multiplier
- Added a source and header accuracy check handler which inherits from _IEffectHandler_ that will calculate if the move will hit the target(s).
- Add in missing imports in _criticalHitHandler.cpp_

### Changed

- Updated the _EffectContext_ move accuracy member variable to follow conventions

### Fixed

- Fixed getting the target defense stage multiplier by passing the _targetDefenseStage_ instead of _userAttackStage_

## [0.7.4] - 2026-07-29

### Added

- Add _ccache_ to the install step _codeql-analysis.yml_ and _testing.yml_

## [0.7.3] - 2026-07-29

### Fixed

- Fix the _moveRegistry.test.cpp_ check that _Pound_ had 10 effects instead of 9.

## [0.7.2] - 2026-07-29

### Added

- Added a battle position variable to the _BattleSlot_ struct
- Add new configuration constant variables:
  - The minimum critical hit value that can be rolled
  - The maximum critical hit value that can be rolled
  - The critical hit percentage
  - The critical hit multiplier
  - The numerator for the stat stage multiplier
  - The denominator for the stat stage multiplier
- Add a range override to the _EffectContext_
- Add _IEffectHandler_ which is the base handler that all others will inherit from.
  - It has an apply function that is virtual and all inherited handlers need to implement
- Added a source and header base damage handler which inherits from _IEffectHandler_ that will calculate the base damage part of the damage calculation from Bulbapedia.
- Added a source and header critical hit handler which inherits from _IEffectHandler_ that will calculate the critical hit part of the damage calculation from Bulbapedia.
- Added an effect handler helper file that:
  - Gets the team based on the _Side_ passed in
  - Gets the const team based on the _Side_ passed in
- Added in a level member variable with its getter and setter to _Pokemon_

### Changed

- Changed the variable type of _StatStages_ from unsigned short to signed byte.
- Reordered the members of _EffectResult_ for smaller total struct byte size and updated all call sites to reflect the new order
- Reordered the _EffectTypeID_ IDs in:
  - _effectType.h_
  - _moveMeta.cpp_

### Removed

- Removed the _StatStage_ effect type ID from:
  - _effectType.h_
  - _moveMeta.cpp_

## [0.7.1] - 2026-07-29

### Added

- Add _ccache_ to the _makefileDependencies.sh_ install step
- Add _ccache_ to the _COMPILER_ Makefile variable

## [0.7.0] - 2026-07-29 (Status Registry Update)

### Added

- Added a new file to hold the built-in Status IDs and a function to convert built-in Status IDs to the stable Status ID
- Added a _Status/constants.h_ file that holds the user readable names for each Status ID.
- Added a metadata file for Pokemon statuses that holds the relevant:
  - User defined name and stable ID
- Added a test suite for _fixedMetadataRegistry.h_

### Changed

- Changed _StatusID_ to be a typedef from the _IDInterface_ instead of an enum

## [0.6.4] - 2026-07-29

### Added

- Add a new configuration constant that will specify how many statuses the registry can hold
- Add in a duplicate status and status not found registry error, and add in the switch cases for these errors in _errorKingToString()_
- Added a file that holds the status registry.
  - The actual registry class holds a constructor that initializes all the builtin statuses:
    - Get the status metadata
    - Get the status ID
    - Get the status name
    - Get a non-owning span of all the statuses
    - Get the next status ID
    - Find the internal array index by stable status ID
    - If the registry has an status:
      - By user readable name
      - By stable status ID
    - Set the next status ID
    - Increment the next status ID
- Added in both the source and header files to configure the status registry.
  - It holds a struct that contains the configuration policy.
  - The actual registry configuration class allows the user to:
    - Get the status metadata
    - Get the Status ID
    - Get the status name
    - Get a non-owning span of all the statuses
    - Get the amount of statuses registered
    - If the registry has a status:
      - By user readable name
      - By stable Status ID
    - Add a status
    - Add in a list of statuses
    - Rename a status
    - Update a status:
      - By user readable name
      - By stable Status ID
    - Remove a status:
      - By user readable name
      - By stable Status ID
- Added in the status registry configuration object to _main.cpp_

### Changed

- Reordered the members of _BattleSlot_ for smaller total struct byte size and updated all call sites to reflect the new order
- Reordered the members of _EffectResult_ for smaller total struct byte size and updated all call sites to reflect the new order

## [0.6.3] - 2026-07-28

### Added

- Added a mutation test for _moveRegistry.test.cpp_

## [0.6.2] - 2026-07-28

### Added

- Add _lcov_ exclusions to _abilityRegistry.h_ where branches are impossible to hit
- Add _lcov_ exclusions to _itemRegistry.h_ where branches are impossible to hit
- Add in new tests for _abilityRegistry_, _itemRegistry_, and _moveRegistry_
  - Tests getting the next ability ID
  - Tests getting the amount registered
  - Tests incrementing the next ability ID

### Changed

- Update all hardcoded values in _abilityRegistry.test.cpp_ to use the constant variable values from _Ability/constants.h_
- Update all hardcoded values in _itemRegistry.test.cpp_ to use the constant variable values from _Item/constants.h_
- Update all hardcoded values in _moveRegistry.test.cpp_ to use the constant variable values from _Move/constants.h_
- Update test case naming in _moveRegistry.test.cpp_

## [0.6.1] - 2026-07-28

### Fixed

- Fixed the _moveRegistry.test.cpp_ to make _Pound_ have the _PsychicTerrainPriorityBlock

## [0.6.0] - 2026-07-28 (Move Registry Update)

### Added

- Add in the ability to replace the target ID for abilities:
  - By stable ID
  - By user readable name
- Add in the ability to update an abilities metadata
  - By stable ID
  - By user readable name
- Add in the ability to replace the target ID for items:
  - By stable ID
  - By user readable name
- Add in the ability to update an items metadata
  - By stable ID
  - By user readable name
- Added in both the source and header files to configure the move registry.
  - It holds a struct that contains the configuration policy.
  - The actual registry configuration class allows the user to:
    - Get the move metadata
    - Get the Move ID
    - Get the move name
    - Get a non-owning span of all the moves
    - Get the amount of moves registered
    - If the registry has an move:
      - By user readable name
      - By stable Move ID
    - Add an move
    - Add in a list of moves
    - Set the trigger and effects for a specific move:
      - By user readable name
      - By stable Move ID
    - Rename an move
    - Update a move:
      - By user readable name
      - By stable Move ID
    - Remove an move:
      - By user readable name
      - By stable Move ID
- Add in a duplicate move and move not found registry error, and add in the switch cases for these errors in _errorKingToString()_
- Added a range ID to the _MoveMeta_
- Add a _MoveRangeID_ enum to specify what the move can hit in _moveTargetsAndTriggers.h_
- Add in the self Target ID to the ability _Drizzle_
- Add in the self Target ID to these items:
  - _CheriBerry_
  - _ChestoBerry_
- Add the missing metadata in the builtin moves:
  - Type ID
  - Power
  - Target ID
  - Range ID
  - Accuracy
  - Priority
  - Special
- Add _lcov_ exclusions to _fixedMetadataRegistry.h_ where branches are impossible to hit
- Added in the move registry configuration object to _main.cpp_
- Added a test suite for _moveRegistry.h_

### Changed

- Update documentation of _abilityRegistryConfiguration.h_
- Update documentation of _itemRegistryConfiguration.h_
- Update documentation of _moveRegistry.h_
- Update documentation of _itemRegistryConfiguration.cpp_
- Update all reference of _AbilityDefinition_ to _AbilityMeta_
- Update all reference of _ItemDefinition_ to _ItemMeta_
- Ran _clang-format_ on _effectContext.h_
- Ran _clang-format_ on _moveMeta.cpp_
- Resolve _clang-tidy_ warnings on:
  - _effectType.h_
  - _itemTargetsAndTriggers.h_
  - _moveRegistry.h_
- Change the _Stench_ trigger to be _OnDamageCalc_
  - In _abilityRegistry.h_
  - In _abilityRegistry.test.cpp_

### Removed

- Removed the _AbilityDefinition_ struct
- Removed the _ItemDefinition_ struct

## [0.5.3] - 2026-07-27

### Added

- Add a target ID in _AbilityMeta_
- Add a target ID in _ItemMeta_
- Add a new configuration constant that will specify how many moves the registry can hold
- Add a new psychic terrain effect type
- Add a new trigger and target in _itemTargetsAndTriggers.h_
- Add in a _None_ variant for built in move IDs
- Added in new metadata for _MoveMeta_:
  - The move's type ID
  - The move's power
  - The move's target ID
  - The move's accuracy
  - The move's priority
  - If the move is special
- Added in a few helper functions that will return commonly used sets of _EffectTypeID_
- Added a file that holds the move registry.
  - Inherits from _fixedMetadataRegistry.h_
  - The actual registry class holds a constructor that initializes all the builtin moves
  - Get the move metadata
  - Get the move ID
  - Get the move name
  - Get a non-owning span of all the moves
  - Get the next move ID
  - Find the internal array index by stable move ID
  - If the registry has an move:
    - By user readable name
    - By stable move ID
  - Set the next move ID
  - Increment the next move ID

### Changed

- Changed the symbolic link of _libstdc++.so.6_ to be the one from the install step in _makefileDependencies.sh_

### Removed

- Removed the copying of specific version of _libstdc++.so.x.x.xx_ in _makefileDependencies.sh_

## [0.5.2] - 2026-07-27

### Added

- Added a new file to hold the built-in Move IDs and a function to convert built-in Move IDs to the stable Move ID
- Added a _Move/constants.h_ file that holds the user readable names for each Move ID.
- Added a metadata file for Pokemon moves that holds the relevant:
  - User defined name, stable ID, and a list of triggers with their effects
- Added moves triggers, targets, and a wrapper for holding both.

### Changed

- Changed _MoveID_ to be a typedef from the _IDInterface_ instead of an enum
- Run _clang-format_ on _pokemon.h_

### Fixed

- Fixed namespace resolution in _pokemon.h_ moves fill.

## [0.5.1] - 2026-07-27

### Added

- Added _public_ specifiers in _AbilityRegistryConfigurationPolicy_ and _AbilityDefinition_
- Added _public_ specifiers in _ItemRegistryConfigurationPolicy_ and _ItemDefinition_
- Added _public_ specifiers in _MatchupPair_ and _TypeDefinition_
- Added _public_ specifiers in _TypeEntry_

### Changed

- Update the function definition of _errorKindToString_ to use _ATTR\_NODISCARD_ from _attributeMacros.h_
- Update the function definition of _toTypeID_ to use _ATTR\_NODISCARD_ from _attributeMacros.h_
- Reordered the members of _AbilityMeta_ for smaller total struct byte size and updated all call sites to reflect the new order
- Reordered the members of _BattleSlot_ for smaller total struct byte size and updated all call sites to reflect the new order
- Reordered the members of _AbilityEffectTrigger_ for smaller total struct byte size and updated all call sites to reflect the new order
- Reordered the members of _RegistryError_ for smaller total struct byte size and updated all call sites to reflect the new order
- Reordered the members of _effectContext_ for smaller total struct byte size and updated all call sites to reflect the new order
- Reordered the members of _ItemMeta_ for smaller total struct byte size and updated all call sites to reflect the new order
- Reordered the members of _ItemEffectTrigger_ for smaller total struct byte size and updated all call sites to reflect the new order
- Reordered the members of _Pokemon_ for smaller total struct byte size and updated all call sites to reflect the new order
- Extract _PocketCore::Core::ub_ to a using declaration as its used more than once in _AbilityTargetsAndTriggers.h_
- Extract _PocketCore::Core::ub_ to a using declaration as its used more than once in _builtinAbilityID.h_
- Updated the documentation for _AbilityMeta_
- Updated the documentation for _AbilityRegistryConfiguration_
- Updated the documentation for _AbilityRegistry_
- Updated the documentation for _FixedMetadataRegistry_
- Update _getAbilityMetadata_ to return an _const AbilityMeta *_
- Update _getMetadata_ to return an _const Metadata *_
- Update _getItemMetadata_ to return an _const ItemMeta *_
- Apply _clang-format_ on _abilityRegistryConfiguration.h_
- Apply _clang-format_ on _fixedMetadataRegistryConfiguration.h_
- Apply _clang-format_ on _Configuration/constants.h_
- Make the following functions in _abilityRegistry.h_ and _abilityRegistry.cpp_ return a const value:
  - _getAbilityID()_
  - _getAbilityName()_
  - _getRegisteredAbilities()_
  - _findIndexByAbilityID()_
- Pass the parameter by const reference/const for the following functions in _abilityRegistry.h_ and _abilityRegistry.cpp_
  - _getAbilityID()_
  - _hasAbility()_
- Make the following functions in _abilityRegistryConfiguration.h_ and _abilityRegistryConfiguration.cpp_ return a const value:
  - _getAbilityID()_
  - _getAbilityName()_
  - _getRegisteredAbilities()_
- Pass the parameter by const reference/const for the following functions in _abilityRegistryConfiguration.h_ and _abilityRegistryConfiguration.cpp_
  - _hasAbility()_
  - _addAbilities()_
  - Both overloads of _setAbilityTriggers()_
  - Both overloads of _removeAbility()_
- Make the following functions in _itemRegistryConfiguration.h_ and _itemRegistryConfiguration.cpp_ return a const value:
  - _getItemID()_
  - _getItemName()_
  - _getRegisteredItems()_
  - _findIndexByItemID()_
- Pass the parameter by const reference/const for the following functions in _itemRegistryConfiguration.h_ and _itemRegistryConfiguration.cpp_
  - _getItemID()_
  - _hasItem()_
- Make the following functions in _itemRegistry.h_ and _itemRegistry.cpp_ return a const value:
  - _getItemID()_
  - _getItemName()_
  - _getRegisteredItems()_
- Pass the parameter by const reference/const for the following functions in _itemRegistry.h_ and _itemRegistry.cpp_
  - _getItemID()_
  - _hasItem()_
  - _addItems()_
  - Both overloads of _setItemTriggers()_
  - _renameItem()_
  - _removeItem()_
- Make the following functions in _typeRegistryConfiguration.h_ and _typeRegistryConfiguration.cpp_ return a const value:
  - _getTypeID()_
  - _getTypeName()_
  - _getRegisteredTypes()_
- Pass the parameter by const reference/const for the following functions in _typeRegistryConfiguration.h_ and _typeRegistryConfiguration.cpp_
  - _getMatchup()_
  - _getMatchupRow()_
  - _getTypeID()_
  - _setMatchup()_
  - Both overloads of _setMatchupRow()_
  - Both overloads of _setDefensiveColumn()_
  - _addType()_
  - _addTypes()_
  - All three overloads of _removeType()_
  - _removeTypes()_
  - _renameType()_
  - Both overloads of _resetMatchups()_
  - _hasType()_
  - _rollbackEntries()_
  - _removeEntry()_
  - _resolveIndex()_
- Make the following functions in _typeRegistry.h_ and _typeRegistry.cpp_ return a const value:
  - _getEntry()_
  - _getTypeChartCell()_
  - _getTypeChartRow()_
  - _getRegisteredTypes()_
  - _findIndexByTypeID()_
- Pass the parameter by const reference/const for the following functions in _typeRegistry.h_ and _typeRegistry.cpp_
  - _getTypeChartRow()_
  - _getTypeID()_
  - _hasType()_
  - _findEntryIndexByName()_
  - Both overloads of _addBuiltin()_
- Make the following functions in _fixedMetadataRegistryConfiguration.h_ and _fixedMetadataRegistryConfiguration.cpp_ return a const value:
  - _getID()_
  - _getName()_
  - _getRegisteredEntries()_
  - _renameMetadata()_
  - Both overloads of _removeMetadata()_
  - _resolveIndex()_
  - _makeNotFoundError()_
- Pass the parameter by const reference/const for the following functions in _fixedMetadataRegistryConfiguration.h_ and _fixedMetadataRegistryConfiguration.cpp_
  - _getID()_
  - _hasEntry()_
  - _addMetadata()_
  - _addMetadataBatch()_
  - Both overloads of _mutateMetadata()_
  - _renameMetadata()_
  - _removeMetadata()_
  - _makeNotFoundError()_
- Make the following functions in _fixedMetadataRegistry.h_ and _fixedMetadataRegistry.cpp_ return a const value:
  - _getID()_
  - _getName()_
  - _getRegisteredEntries()_
  - _findIndexByID()_
- Pass the parameter by const reference/const for the following functions in _fixedMetadataRegistry.h_ and _fixedMetadataRegistry.cpp_
  - _getID()_
  - _hasEntry()_
  - _findEntryIndexByName()_
- Pass the parameter by _&&_ in _addBuiltin_

## [0.5.0] - 2026-07-27 (Item and FixedMetadata Registry Update)

### Added

- Create a _fixedMetadataRegistryConfiguration.h_ file that all _*RegistryConfiguration.h_ files will inherit from
  - It has a method to get the registries metadata
  - It has a method to get the registries stable ID
  - It has a method to get the registries user readable name
  - It has a method to get the all the entries in the registry
  - It has a method to get the amount of entries in the registry
  - It has a method to determine if the registry has an entry:
    - By stable ID
    - By user readable name
  - It has a method to add metadata to the registry
  - It has a method to add batch amounts of metadata to the registry
  - It has a method to mutate a copy of the registered metadata and write it back:
    - By stable ID
    - By user readable name
  - It has a method to rename the user readable name of the metadata
  - It has a method to remove metadata from the registry:
    - By stable ID
    - By user readable name
- Create a _fixedMetadataRegistry.h_ file that all _*Registry.h_ files will inherit from
  - It has a method to get an entries metadata from the registry:
    - By stable ID
    - By registry array index
  - It has a method to get the registries stable ID
  - It has a method to get the registries user readable name
  - It has a method to get the all the entries in the registry
  - It has a method to get the amount of entries in the registry
  - It has a method to get the next stable ID in the registry
  - It has a method to get the internal array index in the registry by stable ID
  - It has a method to determine if the registry has an entry:
    - By stable ID
    - By user readable name
  - It has a method to set a specific entries metadata in the registry
  - It has a method to set the amount of entries registered in the registry
  - It has a method to set the next stable ID in the registry
  - It has a method to increment the amount registered in the registry
  - It has a method to decrement the amount registered in the registry
  - It has a method to increment the next stable ID in the registry
  - It has a protected constructor to be used be all children that inherit from it
  - It has a protected method that will initialize the built-in entries
- Added in both the source and header files to configure the item registry.
  - It holds a struct that contains the user readable name, and the trigger and effect definitions.
  - The actual registry configuration class allows the user to:
    - Get the item metadata
    - Get the Item ID
    - Get the item name
    - Get a non-owning span of all the items
    - Get the amount of items registered
    - If the registry has an item:
      - By user readable name
      - By stable Item ID
    - Add an item
    - Add in a list of items
    - Set the trigger and effects for a specific item:
      - By user readable name
      - By stable Item ID
    - Rename an item
    - Remove an item:
      - By user readable name
      - By stable Item ID
- Added a Ability registry configuration policy that holds:
  - The configuration user name
  - The configuration type
  - The duplicate registry error
  - The not found registry error
- Add in item errors in _RegistryError_ and updated the _errorKingToString_ with the new errors
- Added in the ability registry configuration and item registry configuration objects to _main.cpp_
- Added a test suite for _itemRegistryConfiguration.h_
- Added a test suite for _itemRegistry.h_

### Changed

- _AbilityRegistryConfiguration_ now inherits from _FixedMetadataRegistryConfiguration_:
  - _getAbilityMetadata()_ now calls the parent _getMetadata()_ function
  - _getAbilityID()_ now calls the parent _getID()_ function
  - _getAbilityName()_ now calls the parent _getName()_ function
  - _getRegisteredAbilities()_ now calls the parent _getRegisteredEntries()_ function
  - _getAmountRegistered()_ now calls the parent _getAmountRegistered()_ function
  - Both overloads of _hasAbility()_ now calls the parent _hasEntry()_ function
- _AbilityRegistry_ now inherits from _FixedMetadataRegistry_:
  - _getAbilityMetadata()_ now calls the parent _getMetadata()_ function
  - _getAbilityID()_ now calls the parent _getID()_ function
  - _getAbilityName()_ now calls the parent _getName()_ function
  - _getRegisteredAbilities()_ now calls the parent _getRegisteredEntries()_ function
  - _getNextAbilityID()_ now calls the parent _getNextID()_ function
  - _findIndexByAbilityID()_ now calls the parent _findIndexByID()_ function
  - Both overloads of _hasAbility()_ now calls the parent _hasEntry()_ function
  - _setNextAbilityID()_ now calls the parent _setNextID()_ function
  - _incrementNextAbilityID()_ now calls the parent _incrementNextID()_ function
- _ItemRegistry_ now inherits from _FixedMetadataRegistry_:
  - _getItemMetadata()_ now calls the parent _getMetadata()_ function
  - _getItemID()_ now calls the parent _getID()_ function
  - _getItemName()_ now calls the parent _getName()_ function
  - _getRegisteredAbilities()_ now calls the parent _getRegisteredEntries()_ function
  - _getNextItemID()_ now calls the parent _getNextID()_ function
  - _findIndexByItemID()_ now calls the parent _findIndexByID()_ function
  - Both overloads of _hasItem()_ now calls the parent _hasEntry()_ function
  - _setNextItemID()_ now calls the parent _setNextID()_ function
  - _incrementNextItemID()_ now calls the parent _incrementNextID()_ function
- Update the _abilityRegistryConfiguration.cpp_ to use the new parent _fixedMetadataConfiguration.h_ functions in:
  - _addAbility()_ now calls _addMetadata()_
  - _addAbilities()_ now calls _addMetadataBatch()_
  - Both overloads _setAbilityTriggers()_ now calls _mutateMetadata()_
  - _renameAbility()_ now calls _renameMetadata()_
  - Both overloads of _removeAbility()_ now calls _removeMetadata()_
- Update _pokemon.test.cpp_ to use default initialization for item IDs

### Fixed

- Fixed the header guard in _Ability/constants.h_
- Fixed the header guard in _Item/constants.h_
- Update return type of from unsigned byte to unsigned short in _typeRegistryConfiguration.test.cpp_:
- Update return type of from unsigned byte to unsigned short in _typeRegistry.test.cpp_:
- Update return type of from unsigned byte to unsigned short in _idInterface.test.cpp_:

### Removed

- Remove the functions from _AbilityRegistryConfiguration_:
  - Both overloads of _resolveIndex()_
  - _removeEntry()_
- Remove the member variable from _AbilityRegistryConfiguration_
- Remove the functions from _AbilityRegistry_:
  - _getEntry()_
  - _getAmountRegistered()_
  - _setEntry()_
  - _setAmountRegistered()_
  - _incrementAmountRegistered()_
  - _decrementAmountRegistered()_
  - _findEntryIndexByName()_
  - _findEntryIndexByID()_
  - _addBuiltin()_
- Remove the functions from _ItemRegistry_:
  - _getEntry()_
  - _getAmountRegistered()_
  - _setEntry()_
  - _setAmountRegistered()_
  - _incrementAmountRegistered()_
  - _decrementAmountRegistered()_
  - _findEntryIndexByName()_
  - _findEntryIndexByID()_
  - _addBuiltin()_
- Remove the member variables from _ItemRegistry_
- Remove the member variables from _AbilityRegistry_

## [0.4.1] - 2026-07-27

### Added

- Added a new _IDInterface_ class that each stable ID tag will inherit from
- Added a new file to hold the built-in Item IDs and a function to convert built-in Item IDs to the stable Item ID
- Added a _Item/constants.h_ file that holds the user readable names for each Item ID.
- Added item triggers, targets, and a wrapper for holding both.
- Added a metadata file for Pokemon items that holds the relevant:
  - User defined name, stable item ID, and a list of triggers with their effects
- Added a file that holds the item registry.
  - The actual registry class holds a constructor that initializes all the builtin items
    - Get the item metadata
    - Get the item ID
    - Get the item name
    - Get a non-owning span of all the items
    - Get the amount of items registered
    - Get the next item ID
    - Find the internal array index by stable item ID
    - If the registry has an item:
      - By user readable name
      - By stable item ID
    - Set an item ID's metadata
    - Set the amount registered
    - Set the next item ID
    - Increment the amount registered
    - Decrement the amount registered
    - Increment the next item ID
- Create a unique empty struct _AbilityIDTag_ for each _AbilityID_
- Added new configuration constants:
  - The maximum amount of abilities each Pokemon can have
  - The maximum amount of items the item registry can hold
  - The maximum amount of items each Pokemon can have
- Add _StatusRemove_ to _effectType.h_
- Added a test suite for _idInterface.h_

### Changed

- Update the function definition of _toAbilityID_ to use _ATTR\_NODISCARD_ from _attributeMacros.h_
- Changed _AbilityID_ to be a typedef from the _IDInterface_ instead of a class
- Changed _ItemID_ to be a typedef from the _IDInterface_ instead of an enum
- Changed _TypeID_ to be a typedef from the _IDInterface_ instead of class
- Update format of _abilityRegistryConfiguration.h_
- Update return type of the following functions from unsigned byte to unsigned short in _abilityRegistryConfiguration.h_:
  - _getEntry()_
  - _getAbilityID()_
  - _getAmountRegistered()_
  - _getNextAbilityID()_
  - _findIndexByAbilityID()_
  - _setEntry()_
  - _setAmountRegistered()_
  - _setNextAbilityID()_
  - Both overloads of _findEntryIndexByID()_
- Update return type of the header functions from unsigned byte to unsigned short in _abilityRegistryConfiguration.cpp_:
- Update return type of the following member variables from unsigned byte to unsigned short in _abilityRegistryConfiguration.h_:
  - _mAmountRegistered_
  - _mNextAbilityID_
- Update return type of the following functions from unsigned byte to unsigned short in _abilityRegistryConfiguration.h_:
  - _getAmountRegistered()_
  - Both overloads of _resolveIndex()_
  - _removeEntry()_
- Update return type of the following functions from unsigned byte to unsigned short in _typeRegistryConfiguration.h_:
  - _getAmountRegistered()_
  - _addType()_
  - _clearRows()_
  - _rollbackEntries()_
  - _removeEntry()_
  - _resolveIndex()_
- Update return type of the header functions from unsigned byte to unsigned short in _typeRegistryConfiguration.cpp_:
- Update return type of the following functions from unsigned byte to unsigned short in _typeRegistry.h_:
  - _getEntry()_
  - _getTypeChartCell()_
  - _getTypeChartRow()_
  - _getTypeID()_
  - _getTypeName()_
  - _getAmountRegistered()_
  - _setEntry()_
  - _setTypeChartCell()_
  - _setTypeChartRow()_
  - _setAmountRegistered()_
  - _findIndexByTypeID()_
  - _findEntryIndexByName()_
  - _findEntryIndexById()_
- Update return type of the following member variables from unsigned byte to unsigned short in _typeRegistry.h_:
  - _mAmountRegistered_
  - _mNextTypeID_
- Reorder some of the constant values in _Configuration/constants.h_
- Update the amount of abilities that can exist from _64_ to _1000_

### Fixed

- Fix the namespace resolution for the registry member variable in _abilityRegistryConfiguration.h_
- Fix the namespace resolution within _abilityRegistryConfiguration.cpp_
- Fix the namespace resolution within _abilityRegistry.h_
- Fix the namespace resolution within _abilityRegistry.test.cpp_

### Remove

- Removed the default initializer of the _mItemID_ in _pokemon.h_

## [0.4.0] - 2026-07-27 (Ability Registry Update)

### Added

- Added a constant value to specify no Ability ID
- Added a new file to hold the built-in Ability IDs and a function to convert built-in Ability IDs to the stable Ability ID
- Added a file that holds the ability registry.
  - The actual registry class holds a constructor that initializes all the builtin abilities:
    - Get the ability metadata
    - Get the ability ID
    - Get the ability name
    - Get a non-owning span of all the abilities
    - Get the amount of abilities registered
    - Get the next ability ID
    - Find the internal array index by stable ability ID
    - If the registry has an ability:
      - By user readable name
      - By stable Ability ID
    - Set an ability ID's metadata
    - Set the amount registered
    - Set the next ability ID
    - Increment the amount registered
    - Decrement the amount registered
    - Increment the next ability ID
- Added in both the source and header files to configure the ability registry.
  - It holds a struct that contains the user readable name, and the trigger and effect definitions.
  - The actual registry configuration class allows the user to:
    - Get the ability metadata
    - Get the ability ID
    - Get the ability name
    - Get a non-owning span of all the abilities
    - Get the amount of abilities registered
    - If the registry has an ability:
      - By user readable name
      - By stable Ability ID
    - Add an ability
    - Add in a list of abilities
    - Set the trigger and effects for a specific ability:
      - By user readable name
      - By stable Ability ID
    - Rename an ability
    - Remove an ability:
      - By user readable name
      - By stable Ability ID
- Added in more configuration constants:
  - Can now set the max amount of abilities in the ability registry
  - Can now set the max amount of types a pokemon can hold
  - Can now set the max amount of moves a pokemon can hold
- Add in ability errors in _RegistryError_ and updated the _errorKingToString_ with the new errors
- Add in an array of types to the Pokemon class.
  - Add in a types array getter/setter and individual type getter/setter.
- Added _typeID.h_:
  - Holds a class that contains the stable type ID
  - A constant variable to specify no type ID
  - Has a function to convert one of the type enum values to the stable Ability ID
- Added a test suite for _abilityRegistryConfiguration.h_
- Added a test suite for _abilityRegistry.h_
- Added a test suite for _pokemon.h_
- Added in new tests in _typeRegistryConfiguration.test.cpp_

### Changed

- Changed _AbilityID_ from an enum class to a class to hold a stable typed id specifically for abilities
- Update the check in _addType_ to also check if the _getNextTypeID()_ is the _NO\_TYPE\_ID_
- Update documentation for _abilityMeta.h_
- Fix _clang-tidy_ warnings in _abilityTargetsAndTriggers.h_
- Fix _clang-tidy_ warnings in _effectContext.h_
- Fix _clang-tidy_ warnings in _effectType.h_
- Fix _clang-tidy_ warnings in _itemID.h_
- Fix _clang-tidy_ warnings in _moveID.h_
- Fix _clang-tidy_ warnings in _statusID.h_
- Fix _clang-tidy_ warnings in _pokemon.h_
- Fix _clang-tidy_ warnings in _Types/constants.h_
- Fix _clang-tidy_ warnings in _timer.h_
- Fix _clang-tidy_ warnings in _random.h_
- Fix _clang-tidy_ warnings in _weather.h_
- Fix _clang-tidy_ warnings in _main.cpp_
- Fix _clang-tidy_ warnings in _typeRegistryConfiguration.test.cpp_
- Fix _clang-tidy_ warnings in _timer.test.cpp_
- Update documentation for _Configuration/constants.h_
- Update documentation for _typeRegistry.h_
- In _typeRegistryConfiguration.h_ and _typeRegistryConfiguration.cpp_ update the return types from unsigned byte to the stable Type ID.
- Run _clang-format_ on _typeRegistryConfiguration.cpp_
- In _typeRegistry.h_ update the return types from unsigned byte to the stable Type ID.
- In _effectContext.h_, change the type of _mMoveTypeID_ from unsigned byte to _TypeID_
- Update _pokemon.h_:
  - The constructors now take in an array of type IDs.
  - Change all move count occurrences with _MAX\_MOVES\_PER\_POKEMON_
- Change all move count occurrences with _MAX\_MOVES\_PER\_POKEMON_ in _pokemon.cpp_
- Update _typeRegistryConfiguration.test.cpp_ to use the new stable type ID
- Update _typeRegistry.test.cpp_ to use the new stable type ID

### Removed

- Removed the _getAbilityMetadata_ function

## [0.3.2] - 2026-07-27

### Added

- Added _*.log_ to _.gitignore_
- Added a _clearRows_ method to _typeRegistryConfiguration.h_ that will clear clear the offensive row and defensive column for a type index
- Added _lcov_ exclusions in _typeRegistryConfiguration.cpp_ for branches that are not hittable

### Changed

- Run _clang-format_ on _typeRegistryConfiguration.cpp_
- Updated both of the _resetMatchup_ functions to use the new _clearRows_ method

### Removed

- Removed redundant _arrayIndex_ check in _renameType_ because there is already a check that the _typeID_ exists, thus the _arrayIndex_ has to exist.

## [0.3.1] - 2026-07-26

### Removed

- Removed redundant hooks folder

## [0.3.0] - 2026-07-26 (Pokemon & Ability, Item, and Status IDs)

### Added

- Added the _COMPILER\_VERSION_ to the makefile targets:
  - _run_
  - _run_tidy_
- Added a stable ID for Pokemon moves
- Added a stable ID for Pokemon items
- Added a stable ID for Pokemon abilities
- Added a stable ID for Pokemon statuses
- Added ability triggers, targets, and a wrapper for holding both.
- Added a constants file for Pokemon abilities that holds the user defined names of the abilities.
- Added a metadata file for Pokemon abilities that holds the relevant:
  - User defined name, and a list of triggers with their effects
- Created a header and implementation files for the Pokemon class:
  - It holds the 6 basic stats:
    - Attack
    - Defense
    - Health
    - Special Attack
    - Special Defense
    - Speed
  - It holds a stable Item ID
  - It holds a stable Ability ID
  - It holds an array of four stable Move ID
  - It holds the user defined name of the Pokemon
  - It has getters and setters for all of those
  - It also holds a member method to use PP
- Added a file that contains battle related information
  - It holds a _BattleSlot_ that contains the Pokemon and the pokemon's following info:
    - The stable Item ID it holds
    - The stable Status ID it has
    - The stable Move ID if it was choice locked.
    - If it's protected, flinched, fainted, and if the item was consumed.
    - The counters for toxic, sleep, and protection.
    - The current hp of the pokemon.
    - If it has any speed boost.
    - A struct of stat stages that apply to all 6 stats.
  - It holds a _BattleState_ that contains bother sides pokemon and their _BattleSlots_
    - It also holds the spikes, toxic spikes, and stealth rock state per side.
    - It holds if the battle has been started
    - It holds if the weather is: rain, sun, and/or sandstorm.
    - It holds if the terrain is: electric, grassy, misty, or psychic.
- Added a file that contains context that holds relating to the current action of the battle.
  - It holds an enum that differentiates between the two battling sides.
  - It holds the source enum of what is owning this context.
  - It holds a context just for damage related information
    - What the recoil ratio is to apply back to the Pokemon who used the move
    - The total damage to apply
    - If the move is a critical hit
    - If the move missed
    - If the move is protected against.
    - If the move should apply damage
    - If the state should continue
  - It holds after move use data such as sleep turns, the status to apply, and attack and defense stat changes
  - The effect context which houses the damage context struct, the effect result struct, and the effect source enum.
    - It also holds the multipliers for abilities, items, and critical hits.
    - The user and target index.
    - The move base power, type ID, and accuracy
    - The Move ID and Ability ID being applied
    - The side of both the user and targets
    - If the move is special
- A file that specifies the effect type ID
- Added a file that specifies an enum of weathers

### Changed

- Updated the macro allowed regex to also include _ATTR\__ in the _.clang-tidy_ file
- Changed _TEST\_INTEGRATIONS\_SOURCES_, _TEST\_MOCKS\_SOURCES_, and _BENCHMARKS\_SOURCES_ to use wildcard for safe file querying
- Updated the namespace in _configFlags.h_ to use _PocketCore_ instead of _Pokemon_
- Updated the namespace in _Configuration/constants.h_ to use _PocketCore_ instead of _Pokemon_
- Updated the namespace in _typeRegistryConfiguration.h_ to use _PocketCore_ instead of _Pokemon_
- Updated the namespace in _typeRegistryConfiguration.cpp_ to use _PocketCore_ instead of _Pokemon_
- Updated the namespace in _typeRegistryConfiguration.test.cpp_ to use _PocketCore_ instead of _Pokemon_
- Updated the namespace in _cconcepts.h_ to use _PocketCore_ instead of _Pokemon_
- Updated the namespace in _cconcepts.test.cpp_ to use _PocketCore_ instead of _Pokemon_
- Updated the namespace in _configCat.h_ to use _PocketCore_ instead of _Pokemon_
- Updated the namespace in _configCat.cpp_ to use _PocketCore_ instead of _Pokemon_
- Updated the namespace in _Registry/constants.h_ to use _PocketCore_ instead of _Pokemon_
- Updated the namespace in _typeRegistry.h_ to use _PocketCore_ instead of _Pokemon_
- Updated the namespace in _typeRegistry.test.cpp_ to use _PocketCore_ instead of _Pokemon_
- Moved the _mTypeChart_ member variable to be below _mEntries_ due to byte size
- Updated the namespace in _Types/constants.h_ to use _PocketCore_ instead of _Pokemon_
- Updated the namespace in _typeEffectiveness.h_ to use _PocketCore_ instead of _Pokemon_
- Updated the namespace in _types.h_ to use _PocketCore_ instead of _Pokemon_
- Updated the namespace in _timer.h_ to use _PocketCore_ instead of _Pokemon_
- Updated the namespace in _timer.test.cpp_ to use _PocketCore_ instead of _Pokemon_
- Updated the namespace in _contiguousSequence.h_ to use _PocketCore_ instead of _Pokemon_
- Updated the namespace in _contiguousSequence.test.cpp_ to use _PocketCore_ instead of _Pokemon_
- Updated the namespace in _Logging/constants.h_ to use _PocketCore_ instead of _Pokemon_
- Updated the namespace in _logger.h_ to use _PocketCore_ instead of _Pokemon_
- Updated the namespace in _logger.cpp_ to use _PocketCore_ instead of _Pokemon_
- Updated the namespace in _logger.test.cpp_ to use _PocketCore_ instead of _Pokemon_
- Updated the namespace in _floatUtility.h_ to use _PocketCore_ instead of _Pokemon_
- Updated the namespace in _floatUtility.test.cpp_ to use _PocketCore_ instead of _Pokemon_
- Updated the namespace in _overflowProtection.h_ to use _PocketCore_ instead of _Pokemon_
- Updated the namespace in _overflowProtection.test.cpp_ to use _PocketCore_ instead of _Pokemon_
- Updated the namespace in _input.h_ to use _PocketCore_ instead of _Pokemon_
- Updated the namespace in _random.h_ to use _PocketCore_ instead of _Pokemon_
- Updated the namespace in _main.h_ to use _PocketCore_ instead of _Pokemon_

## [0.2.19] - 2026-05-01

### Added

- Added _lcov_ exclusions to _typeRegistryConfiguration.cpp_ on branches where it's impossible to hit
- Added a logging step in _resetMatchups_ if the type was not found for the _typeName_ parameter
- Added a test file for the _typeRegistryConfiguration_

### Changed

- Changed the default error in _errorKindToString_ to _UnknownError_
- Changed all instances of _*Id_ to _*ID_ in _typeRegistryConfiguration.h_
- Changed all instances of _*Id_ to _*ID_ in _Registry/constants.h_
- Changed all instances of _*Id_ to _*ID_ in _typeRegistry.h_
- Changed all instances of _*Id_ to _*ID_ in _typeRegistryConfiguration.cpp_
- Changed all instances of _*Id_ to _*ID_ in _typeRegistry.test.cpp_

### Removed

- Removed the BatchMismatch from _RegistryError_
- Removed the duplicate check in _addType_
- Removed the redundant check in _rollbackEntries_ that would see if the _amountRegistered_ was greater than the _MAX_TYPES_ allowed
  - This is already guarded before this
- Removed the redundant check in _resolveIndex_ that would see if the registry could find the array index by typeID.
  - This is guaranteed to work since the previous check makes sure that the typeID exists in the registry.

## [0.2.18] - 2026-05-01

### Added

- Added _-Winline_ from _GCC\_WARNINGS_ in the Makefile
- Add the attribute _ATTR\_NOINLINE_ to _attributeMacros.h_
- Added reasoning on the _timer.h_ ctors and dtor deletros
- Added reasoning on the _logger.h_ ctors and dtor deletors

### Changed

- The _getTypeId_ function now also has the _ATTR\_NOINLINE_

### Fixed

- Fixed a typo in _typedefs.h_

### Removed

- Removed _sfloat_ from _typedefs.h_

## [0.2.17] - 2026-04-30

### Added

- Added a concept that will check if a type is an enum
- Added commented out code in _typedefs.h_ that will convert an enum value to the string name of it
  - Waiting on clangd linter support for reflection before using it in the project

### Changed

- Updated _COMPILER\_VERSION_ to _c++26_ in the Makefile

### Removed

- Removed _-Winline_ from _GCC\_WARNINGS_ in the Makefile

## [0.2.16] - 2026-04-30

### Changed

- Updated libstd++ static object version from _6.0.34_ to _6.0.35_ in _makefileDependencies.sh_

## [0.2.15] - 2026-04-30

### Changed

- Updated GCC version from _15.2.0_ to _16.1.0_ in _makefileDependencies.sh_

## [0.2.14] - 2026-04-29

### Added

- Added _bear_ to the list of packages to install in _makefileDependencies.sh_

## [0.2.13] - 2026-04-29

### Removed

- Removed env variables in the _codeql-analysis.yml_

## [0.2.12] - 2026-04-29

### Added

- Added env variables in the _codeql-analysis.yml_
- Added a step to ensure the CodeQL TRAP directory exists, and will create it if not

## [0.2.11] - 2026-04-29

### Added

- Added _build-mode: manual_ to _codeql-analysis.yml_

## [0.2.10] - 2026-04-29

### Changed

- Updated the checkout action from _v4_ to _v6_

## [0.2.9] - 2026-04-29

### Changed

- Now run the codeql analysis before saving back to cache

## [0.2.8] - 2026-04-29

### Changed

- Updated the _codeql-analysis.yml_ file to build off the CodeQL template
- Updated the runner on _testing.yml_ to use _ubuntu-latest_

## [0.2.7] - 2026-04-29

### Changed

- Updated the codeql init action from _v3_ to _v4_

## [0.2.6] - 2026-04-29

### Changed

- Updated the codeql analyze action from _v2_ to _v4_

## [0.2.5] - 2026-04-29

### Added

- Create a _local/cmake/pkgconfig_ folder if it does not exist in the install _spdlog_ from cache step in both:
  - _testing.yml_
  - _codeql-analysis.yml_

### Changed

- Copy all files in the cache's _/lib/cmake/spdlog_ to local

## [0.2.4] - 2026-04-29

### Added

- Added _catch2_ to the install testing framework step in both:
  - _testing.yml_
  - _codeql-analysis.yml_

### Changed

- Changed the name of the test install step from _Install Google Mock_ to _Install Testing Frameworks_ in both:
  - _testing.yml_
  - _codeql-analysis.yml_

## [0.2.3] - 2026-04-29

### Changed

- Give sudo permissions when creating the _local/lib/pkgconfig_ folder

## [0.2.2] - 2026-04-29

### Added

- Create a _local/lib/pkgconfig_ folder if it does not exist in the install _spdlog_ from cache step in both:
  - _testing.yml_
  - _codeql-analysis.yml_

## [0.2.1] - 2026-04-29

### Changed

- In the _testing.yml_ and _codeql-analysis.yml_:
  - Updated where spdlog caches gets installed to
  - Now copies folders from the gcc cache install into local

## [0.2.0] - 2026-04-29 (Type Registry Conversion)

### Changed

- Updated documentation for configuration constants file.
- Renamed _configuration.h_ to _typeRegistryConfiguration.h_
- Documentation update for _typeRegistryConfiguration.h_
- Renamed _configuration.cpp_ to _typeRegistryConfiguration.cpp_
- Reorder functions in _typeRegistryConfiguration.h_ and _typeRegistryConfiguration.cpp_
- Update imports in _main.cpp_
- Add clang-tidy exclusions in _typeRegistry.test.cpp_

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

- Added _-D${TEST\_STANDARD}_ to the _clang-tidy_ command within the _tidy_ Makefile target
- Added _-D${TEST\_STANDARD}_ to the _run-clang-tidy_ command within the _run_tidy_ Makefile target

### Changed

- Updated the _TEST\_STANDARD_ Makefile variable to _catch2_ by default
- Converted the google tests for the following test files into catch2 tests:
  - _cconcepts.test.cpp_
  - _timer.test.cpp_
  - _contiguousSequence.test.cpp_
  - _logger.test.cpp_
  - _floatUtility.test.cpp_
  - _overflowProtection.test.cpp_

## [0.1.4] - 2026-04-22

### Added

- Added _-D${TEST\_STANDARD}_ to the _COMPILER\_FLAGS\_TEST_ Makefile variable

### Changed

- Moved the _TEST\_STANDARD_ Makefile variable closer to the top of the file
- Changed the _testMain.cpp_ file to conditionally include either the catch2 or googletest libraries depending on what the _TEST\_STANDARD_ is set to
  - This will also then execute either the catch2 or googletest test runner depending on what the _TEST\_STANDARD_ is set to

## [0.1.3] - 2026-04-22

### Added

- Added a _TEST\_STANDARD_ variable in the Makefile that defaults to _googletest_
- Added a _GOOGLE\_TEST\_LIBRARIES_ Makefile variable that contains the libraries required for Google Test to work if the _TEST\_STANDARD_ is set to _googletest_ otherwise it will be empty
- Added a _Catch2\_LIBRARIES_ Makefile variable that contains the libraries required for Catch2 to work if the _TEST\_STANDARD_ is set to _catch2_ otherwise it will be empty
- Added a _GOOGLE\_TEST\_EXECUTION\_FLAGS_ Makefile variable that contains the execution flags for Google Test if the _TEST\_STANDARD_ is set to _googletest_ otherwise it will be empty
- Added a _CATCH2\_EXECUTION\_FLAGS_ Makefile variable that contains the execution flags for Catch2 if the _TEST\_STANDARD_ is set to _catch2_ otherwise it will be empty

### Changed

- Updated the _TEST\_LIBRARIES_ Makefile variable to both _GOOGLE\_TEST\_LIBRARIES_ and _Catch2\_LIBRARIES_
- Updated the _TEST\_EXECUTION\_FLAGS_ Makefile variable to both _GOOGLE\_TEST\_EXECUTION\_FLAGS_ and _CATCH2\_EXECUTION\_FLAGS_

## [0.1.2] - 2026-04-22

### Added

- Added in _catch2_ to the list of makefile dependencies
- Added in the _Catch2_ library in the _TEST_LIBRARIES_ Makefile variable
- Added _lcov_ exclusions that match assert and _LCOV\_EXCL\_BR_
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

- Removed redundant folder path finding on _GENHTML\_OUTPUT\_FOLDER_ Makefile variable
- Removed redundant folder path finding on _PROFILE\_FOLDER_ Makefile variable

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
