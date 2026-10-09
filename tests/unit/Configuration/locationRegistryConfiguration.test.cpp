/*! @file locationRegistryConfiguration.test.cpp
	@brief C++ file for running tests for the LocationRegistryConfiguration.
	@date 10/09/2026
	@since 0.12.51
	@version 0.12.51
	@author Matthew Moore
*/

#include "Configuration/locationRegistryConfiguration.h"

#include <expected>
#include <format>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "Configuration/constants.h"
#include "Core/typedefs.h"
#include "Location/builtInLocationID.h"
#include "Location/constants.h"
#include "Location/locationID.h"
#include "Location/locationMeta.h"
#include "Registry/locationRegistry.h"
#include "Registry/registryError.h"
#include "Utility/Debug/Logging/logging.testHelper.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Configuration::LocationRegistryConfiguration;
using PocketCore::Configuration::MAX_LOCATIONS;
using PocketCore::Configuration::RegistryError;
using PocketCore::Core::ub;
using PocketCore::Core::us;
using PocketCore::Location::BuiltinLocationID;
using PocketCore::Location::LOCATION_NAME_NONE;
using PocketCore::Location::LOCATION_NAME_ROUTE1;
using PocketCore::Location::LocationID;
using PocketCore::Location::LocationMeta;
using PocketCore::Location::NO_LOCATION_ID;
using PocketCore::Location::PokemonOrigin;
using PocketCore::Location::toLocationID;
using PocketCore::Registry::Location::LocationRegistry;
using PocketCore::Registry::RegistryErrorInfo;
using PocketCore::Testing::ensureLoggerInitialized;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("LocationRegistryConfiguration")
{
	ensureLoggerInitialized("location registry configuration test", "locationRegistryConfiguration_test.log");

	LocationRegistryConfiguration config{};
	LocationRegistry registry{};
	ub finalLocationUnderlyingValue{std::to_underlying(BuiltinLocationID::FinalLocation)};

	GIVEN("getAmountRegistered")
	{
		THEN("the default registry returns the expected amount of built-in locations")
		{
			CHECK((config.getAmountRegistered() == finalLocationUnderlyingValue));
		}
	}

	GIVEN("getRuntimeRegistry")
	{
		THEN("the runtime registry is the same as the default registry")
		{
			CHECK((config.getRuntimeRegistry() == registry));
		}
	}

	GIVEN("getLocationMetadata")
	{
		THEN("unknown IDs are absent")
		{
			CHECK((config.getLocationMetadata(LocationID{200}) == nullptr));
		}

		THEN("the metadata is retrieved when accessed by a valid Location ID")
		{
			LocationMeta expected{
				.mName = std::string(LOCATION_NAME_NONE),
				.mLocationID = toLocationID(BuiltinLocationID::None),
			};

			CHECK((expected == *config.getLocationMetadata(NO_LOCATION_ID)));
		}
	}

	GIVEN("getLocationID")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(config.getLocationID("Unknown").has_value());
		}

		THEN("the Location ID is retrieved by valid Location name")
		{
			std::optional<LocationID> locationID{config.getLocationID(LOCATION_NAME_NONE)};

			REQUIRE(locationID.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((locationID.value() == toLocationID(BuiltinLocationID::None)));
		}
	}

	GIVEN("getLocationName")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(config.getLocationName(LocationID{200}).has_value());
		}

		THEN("a registered location name is returned by stable ID")
		{
			std::optional<std::string_view> locationName{config.getLocationName(toLocationID(BuiltinLocationID::None))};

			REQUIRE(locationName.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((locationName.value() == LOCATION_NAME_NONE));
		}
	}

	GIVEN("getRegisteredLocations")
	{
		THEN("the amount of locations returned matches the amount that are built-in")
		{
			CHECK((config.getRegisteredLocations().size() == finalLocationUnderlyingValue));
		}
	}

	GIVEN("hasLocation")
	{
		WHEN("calling the string_view overload")
		{
			THEN("an unknown location name has no entry")
			{
				CHECK_FALSE(config.hasLocation("Unknown"));
			}

			THEN("a known location name has an entry")
			{
				CHECK(config.hasLocation(LOCATION_NAME_NONE));
			}
		}

		WHEN("calling the LocationID overload")
		{
			THEN("an unknown location ID has no entry")
			{
				CHECK_FALSE(config.hasLocation(LocationID{200}));
			}

			THEN("a known location ID has an entry")
			{
				CHECK(config.hasLocation(NO_LOCATION_ID));
			}
		}
	}

	GIVEN("addLocation")
	{
		WHEN("trying to add an location past the capacity")
		{
			us newLocationCount{finalLocationUnderlyingValue};

			for (us i{0}; i < MAX_LOCATIONS - finalLocationUnderlyingValue; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				LocationMeta definition{.mName = name};
				std::expected<LocationID, RegistryErrorInfo> result{config.addLocation(definition)};

				REQUIRE(result.has_value());

				LocationID assignedID{result.value()};
				CHECK((assignedID.getValue() == newLocationCount++));
			}

			std::string name{std::format("String_{:04}", MAX_LOCATIONS + 1)};

			LocationMeta definition{.mName = name};
			std::expected<LocationID, RegistryErrorInfo> result{config.addLocation(definition)};

			THEN("registration reports a max capacity and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::MaxCapacity));
				CHECK((config.getAmountRegistered() == MAX_LOCATIONS));
			}
		}

		WHEN("an location whose name is already in use is added")
		{
			LocationMeta definition{.mName = std::string(LOCATION_NAME_ROUTE1)};
			std::expected<LocationID, RegistryErrorInfo> result{config.addLocation(definition)};

			THEN("registration reports a duplicate location and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateLocation));
				CHECK((config.getAmountRegistered() == finalLocationUnderlyingValue));
			}
		}

		WHEN("a unique location definition is added")
		{
			LocationMeta definition{
				.mName = "TestLocationName",
				.mOrigin = PokemonOrigin::Egg,
			};

			std::expected<LocationID, RegistryErrorInfo> result{config.addLocation(definition)};

			THEN("it receives the first custom stable ID and owns its trigger data")
			{
				REQUIRE(result.has_value());
				LocationID assignedID{result.value()};
				CHECK((assignedID.getValue() == finalLocationUnderlyingValue));

				const LocationMeta *metadata{config.getLocationMetadata(assignedID)};

				REQUIRE((metadata != nullptr));
				definition.mLocationID = assignedID;
				CHECK((*metadata == definition));
			}
		}
	}

	GIVEN("addLocations")
	{
		WHEN("trying to add an location past the capacity")
		{
			std::vector<LocationMeta> locationMetas;

			for (us i{0}; i < MAX_LOCATIONS - finalLocationUnderlyingValue; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				locationMetas.push_back({.mName = name});
			}

			std::string name{std::format("String_{:04}", MAX_LOCATIONS + 1)};

			locationMetas.push_back({.mName = name});
			std::expected<void, RegistryErrorInfo> result{config.addLocations(locationMetas)};

			THEN("registration reports a max capacity and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::MaxCapacity));
				CHECK((config.getAmountRegistered() == finalLocationUnderlyingValue));
			}
		}

		WHEN("an location whose name is already in use is added")
		{
			std::vector<LocationMeta> locationMetas{};

			for (us i{0}; i < 20; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				locationMetas.push_back({.mName = name});
			}

			locationMetas.push_back({.mName = std::string(LOCATION_NAME_ROUTE1)});
			const LocationRegistry beforeBatch{config.getRuntimeRegistry()};
			std::expected<void, RegistryErrorInfo> result{config.addLocations(locationMetas)};

			THEN("registration reports a duplicate location and the registry is rollback to the checkpoint before the erroneous addition")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateLocation));
				CHECK((config.getAmountRegistered() == finalLocationUnderlyingValue));
				CHECK((config.getAmountRegistered() == finalLocationUnderlyingValue));
				CHECK((config.getRuntimeRegistry() == beforeBatch));
				CHECK((config.getRuntimeRegistry().getNextLocationID() == beforeBatch.getNextLocationID()));
				CHECK_FALSE(config.hasLocation("String_0000"));
			}
		}

		WHEN("a unique location definition is added")
		{
			std::vector<LocationMeta> locationMetas{};

			locationMetas.push_back({.mName = "TestLocationName"});

			std::expected<void, RegistryErrorInfo> result{config.addLocations(locationMetas)};

			THEN("the registry reports no error and all the location definitions are added")
			{
				REQUIRE(result.has_value());
				CHECK((config.getAmountRegistered() == finalLocationUnderlyingValue + 1));
			}
		}
	}

	GIVEN("renameLocation")
	{
		WHEN("calling with an invalid location name")
		{
			std::expected<void, RegistryErrorInfo> result{config.renameLocation("ThisIsInvalid", "NewName")};

			THEN("the registry reports an error and there is no update to the registry")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::LocationNotFound));
				CHECK((config.getAmountRegistered() == finalLocationUnderlyingValue));
			}
		}

		WHEN("trying to rename to a name already in the registry")
		{
			std::expected<void, RegistryErrorInfo> result{
				config.renameLocation(LOCATION_NAME_NONE, LOCATION_NAME_ROUTE1),
			};

			THEN("the registry reports an error and there is no update to the registry")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateLocation));
				CHECK((config.getAmountRegistered() == finalLocationUnderlyingValue));
			}
		}

		WHEN("updating an existing location definition")
		{
			std::expected<void, RegistryErrorInfo> result{config.renameLocation(LOCATION_NAME_NONE, "NewName")};

			THEN("the registry reports no error and the name is appropriately updated")
			{
				REQUIRE(result.has_value());
				CHECK((config.getAmountRegistered() == finalLocationUnderlyingValue));
				CHECK((config.getLocationMetadata(toLocationID(BuiltinLocationID::None))->mName == "NewName"));
			}
		}
	}

	GIVEN("updateLocation")
	{
		LocationMeta definition{.mName = "TestLocationName"};

		WHEN("calling the string_view overload")
		{
			WHEN("calling with an invalid location name")
			{
				std::expected<void, RegistryErrorInfo> result{config.updateLocation("ThisIsInvalid", definition)};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::LocationNotFound));
					CHECK((config.getAmountRegistered() == finalLocationUnderlyingValue));
				}
			}

			WHEN("trying to rename to a name already in the registry")
			{
				std::expected<void, RegistryErrorInfo> result{
					config.updateLocation(LOCATION_NAME_NONE, {.mName = std::string(LOCATION_NAME_ROUTE1)}),
				};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::DuplicateLocation));
					CHECK((config.getAmountRegistered() == finalLocationUnderlyingValue));
				}
			}

			WHEN("updating an existing location definition")
			{
				std::expected<void, RegistryErrorInfo> result{config.updateLocation(LOCATION_NAME_NONE, definition)};

				THEN("the registry reports no error and the target is appropriately updated")
				{
					REQUIRE(result.has_value());
					CHECK((config.getAmountRegistered() == finalLocationUnderlyingValue));
					CHECK((config.getLocationMetadata(toLocationID(BuiltinLocationID::None))->mName == "TestLocationName"));
				}
			}
		}

		WHEN("calling the LocationID overload")
		{
			WHEN("calling with an invalid location name")
			{
				std::expected<void, RegistryErrorInfo> result{config.updateLocation(LocationID{200}, definition)};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::LocationNotFound));
					CHECK((config.getAmountRegistered() == finalLocationUnderlyingValue));
				}
			}

			WHEN("trying to rename to a name already in the registry")
			{
				std::expected<void, RegistryErrorInfo> result{
					config.updateLocation(toLocationID(BuiltinLocationID::None), {.mName = std::string(LOCATION_NAME_ROUTE1)}),
				};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::DuplicateLocation));
					CHECK((config.getAmountRegistered() == finalLocationUnderlyingValue));
				}
			}

			WHEN("updating an existing location definition")
			{
				std::expected<void, RegistryErrorInfo> result{config.updateLocation(toLocationID(BuiltinLocationID::None), definition)};

				THEN("the registry reports no error and the target is appropriately updated")
				{
					REQUIRE(result.has_value());
					CHECK((config.getAmountRegistered() == finalLocationUnderlyingValue));
					CHECK((config.getLocationMetadata(toLocationID(BuiltinLocationID::None))->mName == "TestLocationName"));
				}
			}
		}
	}

	GIVEN("removeLocation")
	{
		WHEN("calling the string_view overload")
		{
			WHEN("calling with an unknown location name")
			{
				std::expected<LocationID, RegistryErrorInfo> result{config.removeLocation("Unknown")};

				THEN("the result is an error and the registry is not updated")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::LocationNotFound));
					CHECK((config.getAmountRegistered() == finalLocationUnderlyingValue));
				}
			}

			WHEN("calling with a known location name")
			{
				std::expected<LocationID, RegistryErrorInfo> result{config.removeLocation(LOCATION_NAME_NONE)};

				THEN("the result is a success and the registry is updated")
				{
					REQUIRE(result.has_value());
					CHECK((result.value() == toLocationID(BuiltinLocationID::None)));
					CHECK((config.getAmountRegistered() == finalLocationUnderlyingValue - 1));
				}
			}
		}

		WHEN("calling the LocationID overload")
		{
			WHEN("calling with an unknown location ID")
			{
				std::expected<LocationID, RegistryErrorInfo> result{config.removeLocation(LocationID{200})};

				THEN("the result is an error and the registry is not updated")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::LocationNotFound));
					CHECK((config.getAmountRegistered() == finalLocationUnderlyingValue));
				}
			}

			WHEN("calling with a known location ID")
			{
				std::expected<LocationID, RegistryErrorInfo> result{config.removeLocation(toLocationID(BuiltinLocationID::None))};

				THEN("the result is a success and the registry is updated")
				{
					REQUIRE(result.has_value());
					CHECK((result.value() == toLocationID(BuiltinLocationID::None)));
					CHECK((config.getAmountRegistered() == finalLocationUnderlyingValue - 1));
				}
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
