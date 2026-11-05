/*! @file natureHelpers.test.cpp
	@brief C++ file for running tests for the free function Nature helpers.
	@date 11/05/2026
	@since 0.12.53
	@version 0.12.53
	@author Matthew Moore
*/

#include "Nature/natureHelpers.h"

#include <functional>
#include <vector>

#include "Configuration/natureRegistryConfiguration.h"
#include "Core/typedefs.h"
#include "Nature/builtInNatureID.h"
#include "Nature/natureID.h"
#include "Registry/natureRegistry.h"
#include "Utility/Random/random.testHelper.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Configuration::NatureRegistryConfiguration;
using PocketCore::Core::us;
using PocketCore::Nature::BuiltinNatureID;
using PocketCore::Nature::getRandomNatureIDFromConfiguration;
using PocketCore::Nature::NatureID;
using PocketCore::Nature::NO_NATURE_ID;
using PocketCore::Nature::toNatureID;
using PocketCore::Registry::Nature::NatureRegistry;
using PocketCore::Testing::FixedRandom;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("getRandomNatureIDFromConfiguration")
{
	std::unique_ptr<NatureRegistryConfiguration> configRegistryStorage{std::make_unique<NatureRegistryConfiguration>()};
	const NatureRegistry &natureRegistry{configRegistryStorage->getRuntimeRegistry()};

	WHEN("calling when there are no valid nature IDs in the configuration")
	{
		configRegistryStorage->removeAllNatures();

		FixedRandom<us> randomValue{0};
		NatureID result{getRandomNatureIDFromConfiguration(natureRegistry, std::ref(randomValue))};

		THEN("it returns NO_NATURE_ID")
		{
			CHECK((result == NO_NATURE_ID));
		}
	}

	WHEN("calling with no excluded nature IDs")
	{
		FixedRandom<us> randomValue{3};
		NatureID result{getRandomNatureIDFromConfiguration(natureRegistry, std::ref(randomValue))};

		THEN("it returns a nature ID")
		{
			CHECK((result == toNatureID(BuiltinNatureID::Adamant)));
		}
	}

	WHEN("calling with excluded nature IDs")
	{
		FixedRandom<us> randomValue{3};
		std::vector<NatureID> excludedIDs{toNatureID(BuiltinNatureID::Adamant)};

		NatureID result{getRandomNatureIDFromConfiguration(natureRegistry, std::ref(randomValue), excludedIDs)};

		THEN("it will filter out the excluded ID and then grab a nature ID")
		{
			CHECK((result == toNatureID(BuiltinNatureID::Naughty)));
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
