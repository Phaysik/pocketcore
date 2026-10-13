/*! @file abilityValidation.test.cpp
	@brief C++ file for running tests for the Ability validation functions.
	@date 10/13/2026
	@since 0.12.51
	@version 0.12.51
	@author Matthew Moore
*/

#include "Validation/Ability/abilityValidation.h"

#include <array>
#include <expected>
#include <memory>

#include "Ability/abilityID.h"
#include "Ability/builtInAbilityID.h"
#include "Configuration/constants.h"
#include "Core/typedefs.h"
#include "Registry/abilityRegistry.h"
#include "Validation/Ability/abilityError.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Ability::AbilityID;
using PocketCore::Ability::BuiltinAbilityID;
using PocketCore::Ability::toAbilityID;
using PocketCore::Configuration::MAX_ABILITIES_PER_POKEMON;
using PocketCore::Core::us;
using PocketCore::Registry::Ability::AbilityRegistry;
using PocketCore::Validation::Ability::AbilityError;
using PocketCore::Validation::Ability::validateAbilityMetadata;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("validateAbilityMetadata")
{
	std::unique_ptr<AbilityRegistry> registryStorage{std::make_unique<AbilityRegistry>()};
	AbilityRegistry &registry{*registryStorage};

	GIVEN("an array of valid metadata")
	{
		std::array<AbilityID, MAX_ABILITIES_PER_POKEMON> validMetadata{toAbilityID(BuiltinAbilityID::AirLock)};

		std::expected<void, AbilityError> result{validateAbilityMetadata(validMetadata, registry)};

		THEN("the metadata is valid")
		{
			REQUIRE(result.has_value());
		}
	}

	GIVEN("an array with invalid metadata")
	{
		std::array<AbilityID, MAX_ABILITIES_PER_POKEMON> invalidMetadata{AbilityID{200}};

		std::expected<void, AbilityError> result{validateAbilityMetadata(invalidMetadata, registry)};

		THEN("the metadata is invalid")
		{
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error() == AbilityError::MISSING_ABILITY_METADATA));
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
