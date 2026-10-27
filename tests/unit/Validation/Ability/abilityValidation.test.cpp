/*! @file abilityValidation.test.cpp
	@brief C++ file for running tests for the Ability validation functions.
	@date 10/27/2026
	@since 0.12.51
	@version 0.12.53
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
#include "EggGroup/builtInEggGroupID.h"
#include "Pokemon/builtInPokemonID.h"
#include "Pokemon/pokemon.h"
#include "Pokemon/pokemon.testHelper.h"
#include "Pokemon/pokemonMeta.h"
#include "Registry/abilityRegistry.h"
#include "Validation/Ability/abilityError.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Ability::AbilityID;
using PocketCore::Ability::BuiltinAbilityID;
using PocketCore::Ability::toAbilityID;
using PocketCore::Configuration::MAX_ABILITIES_PER_POKEMON;
using PocketCore::Core::us;
using PocketCore::EggGroup::BuiltinEggGroupID;
using PocketCore::EggGroup::NO_EGG_GROUP_ID;
using PocketCore::EggGroup::toEggGroupID;
using PocketCore::Pokemon::BuiltinPokemonID;
using PocketCore::Pokemon::Pokemon;
using PocketCore::Pokemon::PokemonGender;
using PocketCore::Pokemon::toPokemonID;
using PocketCore::Registry::Ability::AbilityRegistry;
using PocketCore::Testing::makePokemon;
using PocketCore::Validation::Ability::AbilityError;
using PocketCore::Validation::Ability::hasTwoFemaleParents;
using PocketCore::Validation::Ability::hasTwoMaleParents;
using PocketCore::Validation::Ability::hasValidEggGroup;
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

	GIVEN("an array with too many value")
	{
		std::vector<AbilityID> invalidMetadata{AbilityID{200}, AbilityID{200}};

		std::expected<void, AbilityError> result{validateAbilityMetadata(invalidMetadata, registry)};

		THEN("the metadata is invalid")
		{
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error() == AbilityError::TOO_MANY_ABILITIES));
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

SCENARIO("hasValidEggGroup")
{
	Pokemon parent1{makePokemon({.mMetadata = {.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Grass)}}})};
	Pokemon parent2{
		makePokemon({.mMetadata = {.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Dragon), toEggGroupID(BuiltinEggGroupID::Grass)}}}),
	};

	WHEN("calling when parent1 has the NoEggs Egg Group ID")
	{
		parent1 = makePokemon({.mMetadata = {.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::NoEggs)}}});

		THEN("it returns PARENT_CANNOT_REPRODUCE")
		{
			std::expected<void, AbilityError> result{hasValidEggGroup(parent1, parent2)};

			REQUIRE(!result.has_value());
			CHECK((result.error() == AbilityError::PARENT_CANNOT_REPRODUCE));
		}
	}

	WHEN("calling when parent2 has the NoEggs Egg Group ID")
	{
		parent2 = makePokemon({.mMetadata = {.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::NoEggs)}}});

		THEN("it returns PARENT_CANNOT_REPRODUCE")
		{
			std::expected<void, AbilityError> result{hasValidEggGroup(parent1, parent2)};

			REQUIRE(!result.has_value());
			CHECK((result.error() == AbilityError::PARENT_CANNOT_REPRODUCE));
		}
	}

	WHEN("calling when both parents are Ditto")
	{
		parent1 = makePokemon(
			{.mMetadata = {.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Ditto)}, .mPokemonID = toPokemonID(BuiltinPokemonID::Ditto)}});
		parent2 = makePokemon(
			{.mMetadata = {.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Ditto)}, .mPokemonID = toPokemonID(BuiltinPokemonID::Ditto)}});

		THEN("it returns BOTH_PARENTS_ARE_DITTO")
		{
			std::expected<void, AbilityError> result{hasValidEggGroup(parent1, parent2)};

			REQUIRE(!result.has_value());
			CHECK((result.error() == AbilityError::BOTH_PARENTS_ARE_DITTO));
		}
	}

	GIVEN("calling when only parent1 is a ditto")
	{
		parent1 = makePokemon(
			{.mMetadata = {.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Ditto)}, .mPokemonID = toPokemonID(BuiltinPokemonID::Ditto)}});

		WHEN("calling when parent2 has no Egg Group ID")
		{
			parent2 = makePokemon({.mMetadata = {.mEggGroupIDs = {NO_EGG_GROUP_ID}}});

			THEN("it returns PARENT_HAS_NO_VALID_EGG_GROUP")
			{
				std::expected<void, AbilityError> result{hasValidEggGroup(parent1, parent2)};

				REQUIRE(!result.has_value());
				CHECK((result.error() == AbilityError::PARENT_HAS_NO_VALID_EGG_GROUP));
			}
		}

		WHEN("calling when parent2 does not have the NoEggs or Ditto Egg Group ID")
		{
			THEN("it succeeds")
			{
				std::expected<void, AbilityError> result{hasValidEggGroup(parent1, parent2)};

				REQUIRE(result.has_value());
			}
		}
	}

	GIVEN("calling when only parent2 is a ditto")
	{
		parent2 = makePokemon(
			{.mMetadata = {.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Ditto)}, .mPokemonID = toPokemonID(BuiltinPokemonID::Ditto)}});

		WHEN("calling when parent1 has no Egg Group ID")
		{
			parent1 = makePokemon({.mMetadata = {.mEggGroupIDs = {NO_EGG_GROUP_ID}}});

			THEN("it returns PARENT_HAS_NO_VALID_EGG_GROUP")
			{
				std::expected<void, AbilityError> result{hasValidEggGroup(parent1, parent2)};

				REQUIRE(!result.has_value());
				CHECK((result.error() == AbilityError::PARENT_HAS_NO_VALID_EGG_GROUP));
			}
		}

		WHEN("calling when parent1 does not have the NoEggs or Ditto Egg Group ID")
		{
			THEN("it succeeds")
			{
				std::expected<void, AbilityError> result{hasValidEggGroup(parent1, parent2)};

				REQUIRE(result.has_value());
			}
		}
	}

	GIVEN("non-ditto Pokemon")
	{
		WHEN("calling with Pokemon that do not share an Egg Group ID")
		{
			parent2 = makePokemon({.mMetadata = {.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Fairy)}}});

			THEN("it returns NO_COMMON_EGG_GROUP")
			{
				std::expected<void, AbilityError> result{hasValidEggGroup(parent1, parent2)};

				REQUIRE(!result.has_value());
				CHECK((result.error() == AbilityError::NO_COMMON_EGG_GROUP));
			}
		}

		WHEN("calling with Pokemon that do share an Egg Group ID")
		{
			THEN("it succeeds")
			{
				std::expected<void, AbilityError> result{hasValidEggGroup(parent1, parent2)};

				REQUIRE(result.has_value());
			}
		}
	}
}

SCENARIO("hasTwoMaleParents")
{
	Pokemon parent1{makePokemon({.mGender = PokemonGender::Male})};
	Pokemon parent2{makePokemon({.mGender = PokemonGender::Female})};

	GIVEN("invalid inputs")
	{
		parent2.setGender(PokemonGender::Male);
		THEN("it returns SAME_SEX_PARENTS_CANT_REPRODUCE")
		{
			std::expected<void, AbilityError> result{hasTwoMaleParents(parent1, parent2)};

			REQUIRE(!result.has_value());
			CHECK((result.error() == AbilityError::SAME_SEX_PARENTS_CANT_REPRODUCE));
		}
	}

	GIVEN("valid inputs")
	{
		WHEN("parent1 is Female and parent2 is Male")
		{
			parent1.setGender(PokemonGender::Female);
			parent2.setGender(PokemonGender::Male);
			std::expected<void, AbilityError> result{hasTwoMaleParents(parent1, parent2)};

			REQUIRE(result.has_value());
		}

		THEN("parent1 is Male and parent2 is Female")
		{
			std::expected<void, AbilityError> result{hasTwoMaleParents(parent1, parent2)};

			REQUIRE(result.has_value());
		}
	}
}

SCENARIO("hasTwoFemaleParents")
{
	Pokemon parent1{makePokemon({.mGender = PokemonGender::Male})};
	Pokemon parent2{makePokemon({.mGender = PokemonGender::Female})};

	GIVEN("invalid inputs")
	{
		parent1.setGender(PokemonGender::Female);
		THEN("it returns SAME_SEX_PARENTS_CANT_REPRODUCE")
		{
			std::expected<void, AbilityError> result{hasTwoFemaleParents(parent1, parent2)};

			REQUIRE(!result.has_value());
			CHECK((result.error() == AbilityError::SAME_SEX_PARENTS_CANT_REPRODUCE));
		}
	}

	GIVEN("valid inputs")
	{
		WHEN("parent1 is Female and parent2 is Male")
		{
			parent1.setGender(PokemonGender::Female);
			parent2.setGender(PokemonGender::Male);
			std::expected<void, AbilityError> result{hasTwoFemaleParents(parent1, parent2)};

			REQUIRE(result.has_value());
		}

		THEN("parent1 is Male and parent2 is Female")
		{
			std::expected<void, AbilityError> result{hasTwoFemaleParents(parent1, parent2)};

			REQUIRE(result.has_value());
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
