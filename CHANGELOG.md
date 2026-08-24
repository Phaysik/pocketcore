# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/2.0.0/), and this project adheres to _vX.Y.Z_ versioning where _X_ represents an _edition_, _Y_ represents an _update_, and _Z_ represents an _addendum_.

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
[0.2.0]: https://github.com/Phaysik/pocketcore/compare/0.1.0...v0.2.0
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
[0.1.0]: https://github.com/Phaysik/pocketcore/releases/tag/0.1.0
