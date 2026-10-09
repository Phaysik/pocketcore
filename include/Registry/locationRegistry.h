/*! @file locationRegistry.h
	@brief Provides fixed-capacity storage and lookup for built-in and user-defined locations.
	@date 10/09/2026
	@since 0.12.51
	@version 0.12.51
	@author Matthew Moore
*/

#ifndef INCLUDE_REGISTRY_LOCATION_REGISTRY_H
#define INCLUDE_REGISTRY_LOCATION_REGISTRY_H

#include <optional>
#include <span>
#include <string_view>

#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Location/builtInLocationID.h"
#include "Location/constants.h"
#include "Location/locationID.h"
#include "Location/locationMeta.h"
#include "Registry/fixedMetadataRegistry.h"

namespace PocketCore::Registry::Location
{
	using PocketCore::Configuration::MAX_LOCATIONS;
	using PocketCore::Core::us;
	using PocketCore::Location::BuiltinLocationID;
	using PocketCore::Location::LOCATION_NAME_NONE;
	using PocketCore::Location::LOCATION_NAME_ROUTE1;
	using PocketCore::Location::LocationID;
	using PocketCore::Location::LocationMeta;
	using PocketCore::Location::toLocationID;
	using PocketCore::Registry::FixedMetadataRegistry;

	/*! @class LocationRegistry Registry/locationRegistry.h
		@brief Stores built-in and user-defined location metadata in fixed-capacity storage.
		@details Built-in locations are registered during construction with IDs derived from @ref BuiltinLocationID. Configuration code may
	   append, replace, or remove entries through the low-level mutators while battle-time callers use allocation-free lookup operations.
		@note Lookup operations are O(n), where n is bounded by @ref MAX_LOCATIONS.
		@date 10/09/2026
		@since 0.12.51
		@version 0.12.51
		@author Matthew Moore
	*/
	class LocationRegistry : private FixedMetadataRegistry<LocationMeta, LocationID, MAX_LOCATIONS, &LocationMeta::mLocationID>
	{
		private:
			using Base = FixedMetadataRegistry<LocationMeta, LocationID, MAX_LOCATIONS, &LocationMeta::mLocationID>;

		public:
			/*! @brief Compares two LocationRegistry instances for equality.
				@param[in] other The other registry to compare with.
				@return true if the registries are equal, false otherwise.
				@since 0.12.51
				@version 0.12.51
			*/
			ATTR_NODISCARD constexpr bool operator==(const LocationRegistry &other) const noexcept = default;

			// LCOV_EXCL_START - If the built in additions fail, the program wouldn't work anyway

			/*! @brief Constructs a registry populated with every @ref BuiltinLocationID.
				@since 0.12.51
				@version 0.12.51
			 */
			ATTR_NOINLINE explicit constexpr LocationRegistry() : Base{toLocationID(BuiltinLocationID::FinalLocation).getValue()}
			{
				addBuiltin({.mName = std::string(LOCATION_NAME_NONE), .mLocationID = toLocationID(BuiltinLocationID::None)});
				addBuiltin({
					.mName = std::string(LOCATION_NAME_ROUTE1),
					.mLocationID = toLocationID(BuiltinLocationID::Route1),
				});
			}

			// LCOV_EXCL_STOP

		protected:
			using Base::addEntry;
			using Base::createCheckpoint;
			using Base::decrementAmountRegistered;
			using Base::eraseEntry;
			using Base::incrementAmountRegistered;
			using Base::restoreCheckpoint;
			using Base::setAmountRegistered;
			using Base::setEntry;

		public:
			using Base::findIndexByID;
			using Base::getAmountRegistered;
			using Base::getEntry;
			using Base::getID;
			using Base::getMetadata;
			using Base::getName;
			using Base::getNextID;
			using Base::getRegisteredEntries;
			using Base::hasEntry;

			/*! @brief Looks up location metadata by stable ID.
				@param[in] locationID The stable location identifier.
				@return A non-owning pointer to metadata if registered, or nullptr otherwise. The pointer remains valid until replacement or
			   registry destruction.
				@since 0.12.51
				@version 0.12.51
			*/
			ATTR_NODISCARD constexpr const LocationMeta *getLocationMetadata(const LocationID locationID) const
			{
				return getMetadata(locationID);
			}

			/*! @brief Looks up an location ID by display name.
				@param[in] name The case-sensitive display name.
				@return The stable ID if registered, or std::nullopt otherwise.
				@since 0.12.51
				@version 0.12.51
			*/
			ATTR_NODISCARD constexpr const std::optional<LocationID> getLocationID(const std::string_view &name) const
			{
				return getID(name);
			}

			/*! @brief Looks up an location display name by stable ID.
				@param[in] locationID The stable location identifier.
				@return The display name if registered, or std::nullopt otherwise.
				@since 0.12.51
				@version 0.12.51
			*/
			ATTR_NODISCARD constexpr const std::optional<std::string_view> getLocationName(const LocationID locationID) const
			{
				return getName(locationID);
			}

			/*! @brief Returns all currently registered location definitions.
				@return A read-only span that remains valid until the registry is mutated or destroyed.
				@since 0.12.51
				@version 0.12.51
			*/
			ATTR_NODISCARD constexpr const std::span<const LocationMeta> getRegisteredLocations() const noexcept
			{
				return getRegisteredEntries();
			}

			/*! @brief Returns the next stable ID assigned to a custom location.
				@return The underlying numeric value of the next location ID.
				@since 0.12.51
				@version 0.12.51
			*/
			ATTR_NODISCARD constexpr us getNextLocationID() const noexcept
			{
				return getNextID();
			}

			/*! @brief Finds an internal array index by stable location ID.
				@param[in] locationID The stable location identifier.
				@return The internal index if registered, or std::nullopt otherwise.
				@since 0.12.51
				@version 0.12.51
			*/
			ATTR_NODISCARD constexpr const std::optional<us> findIndexByLocationID(const LocationID locationID) const
			{
				return findIndexByID(locationID);
			}

			/*! @brief Checks whether an location name is registered.
				@param[in] name The case-sensitive display name.
				@return True if the name is registered, otherwise false.
				@since 0.12.51
				@version 0.12.51
			*/
			ATTR_NODISCARD constexpr bool hasLocation(const std::string_view &name) const
			{
				return hasEntry(name);
			}

			/*! @brief Checks whether an location ID is registered.
				@param[in] locationID The stable location identifier.
				@return True if the ID is registered, otherwise false.
				@since 0.12.51
				@version 0.12.51
			*/
			ATTR_NODISCARD constexpr bool hasLocation(const LocationID locationID) const
			{
				return hasEntry(locationID);
			}
	};
} // namespace PocketCore::Registry::Location

#endif
