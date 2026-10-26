/*! @file eggGroupRegistry.h
	@brief Provides fixed-capacity storage and lookup for built-in and user-defined eggGroups.
	@date 10/26/2026
	@since 0.12.53
	@version 0.12.53
	@author Matthew Moore
*/

#ifndef INCLUDE_REGISTRY_EGGGROUP_REGISTRY_H
#define INCLUDE_REGISTRY_EGGGROUP_REGISTRY_H

#include <optional>
#include <span>
#include <string>
#include <string_view>

#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "EggGroup/builtInEggGroupID.h"
#include "EggGroup/constants.h"
#include "EggGroup/eggGroupID.h"
#include "EggGroup/eggGroupMeta.h"
#include "Registry/fixedMetadataRegistry.h"

namespace PocketCore::Registry::EggGroup
{
	using PocketCore::Configuration::MAX_EGG_GROUPS;
	using PocketCore::Core::us;
	using PocketCore::EggGroup::BuiltinEggGroupID;
	using PocketCore::EggGroup::EGG_GROUP_NAME_AMORPHOUS;
	using PocketCore::EggGroup::EGG_GROUP_NAME_BUG;
	using PocketCore::EggGroup::EGG_GROUP_NAME_DITTO;
	using PocketCore::EggGroup::EGG_GROUP_NAME_DRAGON;
	using PocketCore::EggGroup::EGG_GROUP_NAME_FAIRY;
	using PocketCore::EggGroup::EGG_GROUP_NAME_FIELD;
	using PocketCore::EggGroup::EGG_GROUP_NAME_FLYING;
	using PocketCore::EggGroup::EGG_GROUP_NAME_GRASS;
	using PocketCore::EggGroup::EGG_GROUP_NAME_HUMAN_LIKE;
	using PocketCore::EggGroup::EGG_GROUP_NAME_MINERAL;
	using PocketCore::EggGroup::EGG_GROUP_NAME_MONSTER;
	using PocketCore::EggGroup::EGG_GROUP_NAME_NO_EGGS;
	using PocketCore::EggGroup::EGG_GROUP_NAME_NONE;
	using PocketCore::EggGroup::EGG_GROUP_NAME_WATER1;
	using PocketCore::EggGroup::EGG_GROUP_NAME_WATER2;
	using PocketCore::EggGroup::EGG_GROUP_NAME_WATER3;
	using PocketCore::EggGroup::EggGroupID;
	using PocketCore::EggGroup::EggGroupMeta;
	using PocketCore::EggGroup::toEggGroupID;
	using PocketCore::Registry::FixedMetadataRegistry;

	/*! @class EggGroupRegistry Registry/eggGroupRegistry.h
		@brief Stores built-in and user-defined eggGroup metadata in fixed-capacity storage.
		@details Built-in eggGroups are registered during construction with IDs derived from @ref BuiltinEggGroupID. Configuration code may
	   append, replace, or remove owned metadata through protected mutators while battle-time callers use allocation-free lookup operations.
		@note Stable-ID lookups are O(log n), while name lookups are O(n), where n is bounded by @ref MAX_EGG_GROUPS.
		@date 10/26/2026
		@since 0.12.53
		@version 0.12.53
		@author Matthew Moore
	*/
	class EggGroupRegistry : private FixedMetadataRegistry<EggGroupMeta, EggGroupID, MAX_EGG_GROUPS, &EggGroupMeta::mEggGroupID>
	{
		private:
			using Base = FixedMetadataRegistry<EggGroupMeta, EggGroupID, MAX_EGG_GROUPS, &EggGroupMeta::mEggGroupID>;

		public:
			/*! @brief Compares two EggGroupRegistry instances for equality.
				@param[in] other The other registry to compare with.
				@return true if the registries are equal, false otherwise.
				@since 0.12.53
				@version 0.12.53
			*/
			ATTR_NODISCARD constexpr bool operator==(const EggGroupRegistry &other) const noexcept = default;

			// LCOV_EXCL_START - If the built in additions fail, the program wouldn't work anyway

			/*! @brief Constructs a registry populated with all built-in eggGroups, including None but excluding the FinalEggGroup sentinel.
				@since 0.12.53
				@version 0.12.53
			 */
			ATTR_NOINLINE explicit constexpr EggGroupRegistry()
			{
				addBuiltin({.mName = std::string(EGG_GROUP_NAME_NONE), .mEggGroupID = toEggGroupID(BuiltinEggGroupID::None)});
				addBuiltin({.mName = std::string(EGG_GROUP_NAME_AMORPHOUS), .mEggGroupID = toEggGroupID(BuiltinEggGroupID::Amorphous)});
				addBuiltin({.mName = std::string(EGG_GROUP_NAME_BUG), .mEggGroupID = toEggGroupID(BuiltinEggGroupID::Bug)});
				addBuiltin({.mName = std::string(EGG_GROUP_NAME_DRAGON), .mEggGroupID = toEggGroupID(BuiltinEggGroupID::Dragon)});
				addBuiltin({.mName = std::string(EGG_GROUP_NAME_FAIRY), .mEggGroupID = toEggGroupID(BuiltinEggGroupID::Fairy)});
				addBuiltin({.mName = std::string(EGG_GROUP_NAME_FIELD), .mEggGroupID = toEggGroupID(BuiltinEggGroupID::Field)});
				addBuiltin({.mName = std::string(EGG_GROUP_NAME_FLYING), .mEggGroupID = toEggGroupID(BuiltinEggGroupID::Flying)});
				addBuiltin({.mName = std::string(EGG_GROUP_NAME_GRASS), .mEggGroupID = toEggGroupID(BuiltinEggGroupID::Grass)});
				addBuiltin({.mName = std::string(EGG_GROUP_NAME_HUMAN_LIKE), .mEggGroupID = toEggGroupID(BuiltinEggGroupID::HumanLike)});
				addBuiltin({.mName = std::string(EGG_GROUP_NAME_MINERAL), .mEggGroupID = toEggGroupID(BuiltinEggGroupID::Mineral)});
				addBuiltin({.mName = std::string(EGG_GROUP_NAME_MONSTER), .mEggGroupID = toEggGroupID(BuiltinEggGroupID::Monster)});
				addBuiltin({.mName = std::string(EGG_GROUP_NAME_WATER1), .mEggGroupID = toEggGroupID(BuiltinEggGroupID::Water1)});
				addBuiltin({.mName = std::string(EGG_GROUP_NAME_WATER2), .mEggGroupID = toEggGroupID(BuiltinEggGroupID::Water2)});
				addBuiltin({.mName = std::string(EGG_GROUP_NAME_WATER3), .mEggGroupID = toEggGroupID(BuiltinEggGroupID::Water3)});
				addBuiltin({.mName = std::string(EGG_GROUP_NAME_DITTO), .mEggGroupID = toEggGroupID(BuiltinEggGroupID::Ditto)});
				addBuiltin({.mName = std::string(EGG_GROUP_NAME_NO_EGGS), .mEggGroupID = toEggGroupID(BuiltinEggGroupID::NoEggs)});
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

			/*! @brief Looks up eggGroup metadata by stable ID.
				@param[in] eggGroupID The stable eggGroup identifier.
				@return A non-owning pointer to registry-owned metadata if registered, or nullptr otherwise. The pointer must not be
			   retained across registry mutations or destruction, as mutations can replace metadata or compact storage.
				@since 0.12.53
				@version 0.12.53
			*/
			ATTR_NODISCARD constexpr const EggGroupMeta *getEggGroupMetadata(const EggGroupID eggGroupID) const
			{
				return getMetadata(eggGroupID);
			}

			/*! @brief Looks up a eggGroup ID by display name.
				@param[in] name The case-sensitive display name.
				@return The stable ID if registered, or std::nullopt otherwise.
				@since 0.12.53
				@version 0.12.53
			*/
			ATTR_NODISCARD constexpr const std::optional<EggGroupID> getEggGroupID(const std::string_view name) const
			{
				return getID(name);
			}

			/*! @brief Looks up a eggGroup display name by stable ID.
				@param[in] eggGroupID The stable eggGroup identifier.
				@return A non-owning view of the registry-owned display name if registered, or std::nullopt otherwise. The view must not be
			   retained across registry mutations or destruction.
				@since 0.12.53
				@version 0.12.53
			*/
			ATTR_NODISCARD constexpr const std::optional<std::string_view> getEggGroupName(const EggGroupID eggGroupID) const
			{
				return getName(eggGroupID);
			}

			/*! @brief Returns all currently registered eggGroup definitions.
				@return A read-only span that remains valid until the registry is mutated or destroyed.
				@since 0.12.53
				@version 0.12.53
			*/
			ATTR_NODISCARD constexpr const std::span<const EggGroupMeta> getRegisteredEggGroups() const noexcept
			{
				return getRegisteredEntries();
			}

			/*! @brief Returns the next stable ID assigned to a custom eggGroup.
				@return The underlying numeric value of the next eggGroup ID.
				@since 0.12.53
				@version 0.12.53
			*/
			ATTR_NODISCARD constexpr us getNextEggGroupID() const noexcept
			{
				return getNextID();
			}

			/*! @brief Finds an internal array index by stable eggGroup ID.
				@param[in] eggGroupID The stable eggGroup identifier.
				@return The internal index if registered, or std::nullopt otherwise.
				@since 0.12.53
				@version 0.12.53
			*/
			ATTR_NODISCARD constexpr const std::optional<us> findIndexByEggGroupID(const EggGroupID eggGroupID) const
			{
				return findIndexByID(eggGroupID);
			}

			/*! @brief Checks whether a eggGroup name is registered.
				@param[in] name The case-sensitive display name.
				@return True if the name is registered, otherwise false.
				@since 0.12.53
				@version 0.12.53
			*/
			ATTR_NODISCARD constexpr bool hasEggGroup(const std::string_view name) const
			{
				return hasEntry(name);
			}

			/*! @brief Checks whether a eggGroup ID is registered.
				@param[in] eggGroupID The stable eggGroup identifier.
				@return True if the ID is registered, otherwise false.
				@since 0.12.53
				@version 0.12.53
			*/
			ATTR_NODISCARD constexpr bool hasEggGroup(const EggGroupID eggGroupID) const
			{
				return hasEntry(eggGroupID);
			}
	};
} // namespace PocketCore::Registry::EggGroup

#endif
