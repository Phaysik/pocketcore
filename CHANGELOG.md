# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/2.0.0/), and this project adheres to _vX.Y.Z_ versioning where _X_ represents an _edition_, _Y_ represents an _update_, and _Z_ represents an _addendum_.

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
- Reordered the members of _AbilityMeta_ for smaller total struct byte size and updated all callsites to reflect the new order
- Reordered the members of _BattleSlot_ for smaller total struct byte size and updated all callsites to reflect the new order
- Reordered the members of _AbilityEffectTrigger_ for smaller total struct byte size and updated all callsites to reflect the new order
- Reordered the members of _RegistryError_ for smaller total struct byte size and updated all callsites to reflect the new order
- Reordered the members of _effectContext_ for smaller total struct byte size and updated all callsites to reflect the new order
- Reordered the members of _ItemMeta_ for smaller total struct byte size and updated all callsites to reflect the new order
- Reordered the members of _ItemEffectTrigger_ for smaller total struct byte size and updated all callsites to reflect the new order
- Reordered the members of _Pokemon_ for smaller total struct byte size and updated all callsites to reflect the new order
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
    - Get the item ID
    - Get the item name
    - Get a non-owning span of all the items
    - Get the amount of items registered
    - If the registry has an item:
      - By user readable name
      - By stable item ID
    - Add an item
    - Add in a list of items
    - Set the trigger and effects for a specific item:
      - By user readable name
      - By stable item ID
    - Rename an item
    - Remove an item:
      - By user readable name
      - By stable item ID
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
  - The actual registry class holds a constructor that initializes all the builtin items:
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
  - Change all move count occurences with _MAX\_MOVES\_PER\_POKEMON_
- Change all move count occurences with _MAX\_MOVES\_PER\_POKEMON_ in _pokemon.cpp_
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
- Added abilitiy triggers, targets, and a wrapper for holding both.
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
- Added reasoning on the _timer.h_ ctors and dtor deletors
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
  - _codeql-anaylsis.yml_

### Changed

- Copy all files in the cache's _/lib/cmake/spdlog_ to local

## [0.2.4] - 2026-04-29

### Added

- Added _catch2_ to the install testing framework step in both:
  - _testing.yml_
  - _codeql-anaylsis.yml_
  
### Changed

- Changed the name of the test install step from _Install Google Mock_ to _Install Testing Frameworks_ in both:
  - _testing.yml_
  - _codeql-anaylsis.yml_

## [0.2.3] - 2026-04-29

### Changed

- Give sudo permissions when creating the _local/lib/pkgconfig_ folder

## [0.2.2] - 2026-04-29

### Added

- Create a _local/lib/pkgconfig_ folder if it does not exist in the install _spdlog_ from cache step in both:
  - _testing.yml_
  - _codeql-anaylsis.yml_

## [0.2.1] - 2026-04-29

### Changed

- In the _testing.yml_ and _codeql-anaylsis.yml_:
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
