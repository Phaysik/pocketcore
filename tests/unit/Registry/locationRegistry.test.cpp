/*! @file locationRegistry.test.cpp
	@brief C++ file for running tests for the LocationRegistry.
	@date 10/09/2026
	@since 0.12.51
	@version 0.12.51
	@author Matthew Moore
*/

#include "Registry/locationRegistry.h"

#include <expected>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <utility>

#include "Configuration/locationRegistryConfiguration.h"
#include "Core/typedefs.h"
#include "Location/builtInLocationID.h"
#include "Location/constants.h"
#include "Location/locationID.h"
#include "Location/locationMeta.h"
#include "Registry/registryError.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Configuration::LocationRegistryConfiguration;
using PocketCore::Core::ub;
using PocketCore::Location::BuiltinLocationID;
using PocketCore::Location::LOCATION_NAME_NONE;
using PocketCore::Location::LOCATION_NAME_ROUTE1;
using PocketCore::Location::LocationID;
using PocketCore::Location::LocationMeta;
using PocketCore::Location::NO_LOCATION_ID;
using PocketCore::Location::toLocationID;
using PocketCore::Registry::Location::LocationRegistry;
using PocketCore::Registry::RegistryErrorInfo;

template <typename Registry>
concept PubliclyStructurallyMutable = requires(Registry &registry) { registry.setAmountRegistered(0); };

static_assert(!PubliclyStructurallyMutable<LocationRegistry>);

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("LocationRegistry")
{
	LocationRegistry registry{};
	ub finalLocationUnderlyingValue{std::to_underlying(BuiltinLocationID::FinalLocation)};

	GIVEN("a default constructed location registry")
	{
		THEN("Route 1 has the appropriate properties")
		{
			LocationMeta expected{
				.mName = std::string(LOCATION_NAME_ROUTE1),
				.mLocationID = toLocationID(BuiltinLocationID::Route1),
			};

			const LocationMeta *actual{registry.getLocationMetadata(toLocationID(BuiltinLocationID::Route1))};

			CHECK((expected == *actual));
		}
	}

	GIVEN("getLocationMetadata")
	{
		THEN("unknown IDs are absent")
		{
			CHECK((registry.getLocationMetadata(LocationID{200}) == nullptr));
		}

		THEN("the metadata is retrieved when accessed by a valid Location ID")
		{
			LocationMeta expected{
				.mName = std::string(LOCATION_NAME_NONE),
				.mLocationID = toLocationID(BuiltinLocationID::None),
			};

			CHECK((expected == *registry.getLocationMetadata(NO_LOCATION_ID)));
		}
	}

	GIVEN("getLocationID")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(registry.getLocationID("Unknown").has_value());
		}

		THEN("the Location ID is retrieved by valid Location name")
		{
			std::optional<LocationID> locationID{registry.getLocationID(LOCATION_NAME_NONE)};

			REQUIRE(locationID.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((locationID.value() == toLocationID(BuiltinLocationID::None)));
		}
	}

	GIVEN("getLocationName")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(registry.getLocationName(LocationID{200}).has_value());
		}

		THEN("a registered location name is returned by stable ID")
		{
			std::optional<std::string_view> locationName{registry.getLocationName(toLocationID(BuiltinLocationID::None))};

			REQUIRE(locationName.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((locationName.value() == LOCATION_NAME_NONE));
		}
	}

	GIVEN("getAmountRegistered")
	{
		THEN("the registered span contains the exact amount of built-in entries")
		{
			CHECK((registry.getAmountRegistered() == finalLocationUnderlyingValue));
		}
	}

	GIVEN("getEntry")
	{
		THEN("an invalid internal array index has no metadata")
		{
			CHECK((registry.getEntry(2'000) == nullptr));
		}

		THEN("a valid internal array index has metadata")
		{
			LocationMeta expected{
				.mName = std::string(LOCATION_NAME_NONE),
				.mLocationID = toLocationID(BuiltinLocationID::None),
			};

			const LocationMeta *locationMeta{registry.getEntry(0)};

			REQUIRE((locationMeta != nullptr));
			CHECK((*locationMeta == expected));
		}
	}

	GIVEN("getRegisteredLocations")
	{
		THEN("the amount of locations returned matches the amount that are built-in")
		{
			CHECK((registry.getRegisteredLocations().size() == finalLocationUnderlyingValue));
		}
	}

	GIVEN("getNextLocationID")
	{
		THEN("the next available stable Location ID is after all built in location IDs")
		{
			CHECK((registry.getNextLocationID() == finalLocationUnderlyingValue));
		}
	}

	GIVEN("findIndexByLocationID")
	{
		THEN("an unknown stable ID has no internal index")
		{
			std::optional<ub> locationIndex{registry.findIndexByLocationID(LocationID{200})};
			CHECK_FALSE(locationIndex.has_value());
		}

		THEN("the internal array index is retrieved by valid Location ID")
		{
			std::optional<ub> locationIndex{registry.findIndexByLocationID(NO_LOCATION_ID)};

			REQUIRE(locationIndex.has_value());
			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((locationIndex.value() == 0));
		}
	}

	GIVEN("hasLocation")
	{
		WHEN("calling the string_view overload")
		{
			THEN("an unknown location name has no entry")
			{
				CHECK_FALSE(registry.hasLocation("Unknown"));
			}

			THEN("a known location name has an entry")
			{
				CHECK(registry.hasLocation(LOCATION_NAME_NONE));
			}
		}

		WHEN("calling the LocationID overload")
		{
			THEN("an unknown location ID has no entry")
			{
				CHECK_FALSE(registry.hasLocation(LocationID{200}));
			}

			THEN("a known location ID has an entry")
			{
				CHECK(registry.hasLocation(NO_LOCATION_ID));
			}
		}
	}

	WHEN("operator==")
	{
		GIVEN("two default constructed registries")
		{
			LocationRegistry other{};

			THEN("they are equal")
			{
				CHECK((registry == other));
			}
		}

		GIVEN("for an entry added in one registry")
		{
			LocationRegistryConfiguration other{};
			std::expected<LocationID, RegistryErrorInfo> result{other.addLocation({.mName = "test"})};

			THEN("they are not equal")
			{
				REQUIRE(result.has_value());
				CHECK((registry != other.getRuntimeRegistry()));
			}
		}

		GIVEN("for an entry removed in one registry")
		{
			LocationRegistryConfiguration other{};
			std::expected<LocationID, RegistryErrorInfo> result{other.removeLocation(toLocationID(BuiltinLocationID::None))};

			THEN("they are not equal")
			{
				REQUIRE(result.has_value());
				CHECK((registry != other.getRuntimeRegistry()));
			}
		}

		GIVEN("for an entry modified in one registry")
		{
			LocationRegistryConfiguration other{};
			std::expected<void, RegistryErrorInfo> result{
				other.updateLocation(toLocationID(BuiltinLocationID::None), {.mName = "test"}),
			};

			THEN("they are not equal")
			{
				REQUIRE(result.has_value());
				CHECK((registry != other.getRuntimeRegistry()));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
