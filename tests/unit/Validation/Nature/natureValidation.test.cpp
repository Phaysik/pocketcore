/*! @file natureValidation.test.cpp
	@brief C++ file for running tests for the Nature validation functions.
	@date 10/21/2026
	@since 0.12.51
	@version 0.12.51
	@author Matthew Moore
*/

#include "Validation/Nature/natureValidation.h"

#include <array>
#include <expected>
#include <memory>

#include "Configuration/constants.h"
#include "Core/typedefs.h"
#include "Nature/builtInNatureID.h"
#include "Nature/natureID.h"
#include "Registry/natureRegistry.h"
#include "Validation/Nature/natureError.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Configuration::MAX_NATURES_PER_POKEMON;
using PocketCore::Core::us;
using PocketCore::Nature::BuiltinNatureID;
using PocketCore::Nature::NatureID;
using PocketCore::Nature::toNatureID;
using PocketCore::Registry::Nature::NatureRegistry;
using PocketCore::Validation::Nature::NatureError;
using PocketCore::Validation::Nature::validateNatureMetadata;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("validateNatureMetadata")
{
	std::unique_ptr<NatureRegistry> registryStorage{std::make_unique<NatureRegistry>()};
	NatureRegistry &registry{*registryStorage};

	GIVEN("an array of valid metadata")
	{
		std::array<NatureID, MAX_NATURES_PER_POKEMON> validMetadata{toNatureID(BuiltinNatureID::Hasty)};

		std::expected<void, NatureError> result{validateNatureMetadata(validMetadata, registry)};

		THEN("the metadata is valid")
		{
			REQUIRE(result.has_value());
		}
	}

	GIVEN("an array with too many value")
	{
		std::vector<NatureID> invalidMetadata{NatureID{200}, NatureID{200}};

		std::expected<void, NatureError> result{validateNatureMetadata(invalidMetadata, registry)};

		THEN("the metadata is invalid")
		{
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error() == NatureError::MISSING_NATURE_METADATA));
		}
	}

	GIVEN("an array with invalid metadata")
	{
		std::array<NatureID, MAX_NATURES_PER_POKEMON> invalidMetadata{NatureID{200}};

		std::expected<void, NatureError> result{validateNatureMetadata(invalidMetadata, registry)};

		THEN("the metadata is invalid")
		{
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error() == NatureError::MISSING_NATURE_METADATA));
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
