/*! @file moveValidation.test.cpp
	@brief C++ file for running tests for the Move validation functions.
	@date 10/13/2026
	@since 0.12.51
	@version 0.12.51
	@author Matthew Moore
*/

#include "Validation/Move/moveValidation.h"

#include <array>
#include <expected>
#include <memory>

#include "Configuration/constants.h"
#include "Core/typedefs.h"
#include "Move/builtInMoveID.h"
#include "Move/moveID.h"
#include "Registry/moveRegistry.h"
#include "Validation/Move/moveError.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Configuration::MAX_MOVES_PER_POKEMON;
using PocketCore::Core::us;
using PocketCore::Move::BuiltinMoveID;
using PocketCore::Move::MoveID;
using PocketCore::Move::toMoveID;
using PocketCore::Registry::Move::MoveRegistry;
using PocketCore::Validation::Move::MoveError;
using PocketCore::Validation::Move::validateMoveMetadata;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("validateMoveMetadata")
{
	std::unique_ptr<MoveRegistry> registryStorage{std::make_unique<MoveRegistry>()};
	MoveRegistry &registry{*registryStorage};

	GIVEN("an array of valid metadata")
	{
		std::array<MoveID, MAX_MOVES_PER_POKEMON> validMetadata{toMoveID(BuiltinMoveID::Pound)};

		std::expected<void, MoveError> result{validateMoveMetadata(validMetadata, registry)};

		THEN("the metadata is valid")
		{
			REQUIRE(result.has_value());
		}
	}

	GIVEN("an array with invalid metadata")
	{
		std::array<MoveID, MAX_MOVES_PER_POKEMON> invalidMetadata{MoveID{200}};

		std::expected<void, MoveError> result{validateMoveMetadata(invalidMetadata, registry)};

		THEN("the metadata is invalid")
		{
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error() == MoveError::MISSING_MOVE_METADATA));
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
