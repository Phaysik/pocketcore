# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/2.0.0/), and this project adheres to _vX.Y.Z_ versioning where _X_ represents an _edition_, _Y_ represents an _update_, and _Z_ represents an _addendum_.

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
- Create a [constants](https://github.com/Phaysik/pocketcore/blob/c4cd662884c59b9cef41f4269be3b596ec4f2321/include/Registry/constants.h) file that defines registry error assert message strings.
- Create a [type effectiveness](https://github.com/Phaysik/pocketcore/blob/c4cd662884c59b9cef41f4269be3b596ec4f2321/include/Types/typeEffectiveness.h) file that holds the 5 possible states of type effectiveness.
  - NOT_DEFINED - A sentinel value for uninitialized matchups
  - NE - No Effect (0x multiplier)
  - NVE - Not Very Effective (0.5x multiplier)
  - E - Effective (1x multiplier)
  - SE - Super Effective (2x multiplier)
- Create a [type enum](https://github.com/Phaysik/pocketcore/blob/c4cd662884c59b9cef41f4269be3b596ec4f2321/include/Types/types.h) file that holds the enum of all standard pokemon types.
- Create a [type registry](https://github.com/Phaysik/pocketcore/blob/c4cd662884c59b9cef41f4269be3b596ec4f2321/include/Registry/typeRegistry.h) file.
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
- Create a [constants](https://github.com/Phaysik/pocketcore/commit/c4cd662884c59b9cef41f4269be3b596ec4f2321#diff-ff336b6ba3a85666517eb56ec5985e594fffafccf34618de2c8a970a07042e7b) file that holds the metadata for the builtin types and their type matchups.
- Create a [constants](https://github.com/Phaysik/pocketcore/blob/c4cd662884c59b9cef41f4269be3b596ec4f2321/include/Configuration/constants.h) file that defines configuration constants for the application and registry error metadata.
  - Holds a RegistryError enum to specify any error that could happen while adding types to the registry.
  - A single struct to convert the error enum to a human readable string.
  - An enum that can be used to specify what to fill unspecified type matchups as:
    - Neutral (1x)
    - NotDefined (NOT_DEFINED)
- Create a [type configuration file](https://github.com/Phaysik/pocketcore/blob/c4cd662884c59b9cef41f4269be3b596ec4f2321/include/Configuration/configuration.h) that allows users to:
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
