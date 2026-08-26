/*! @file multiplierRegistry.h
	@brief Provides fixed-capacity storage and lookup for built-in and user-defined multipliers.
	@date 07/30/2026
	@since 0.8.1
	@version 0.8.1
	@author Matthew Moore
*/

#ifndef INCLUDE_REGISTRY_MULTIPLIER_REGISTRY_H
#define INCLUDE_REGISTRY_MULTIPLIER_REGISTRY_H

#include <optional>
#include <span>
#include <string_view>

#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Effect/builtInEffectID.h"
#include "Multiplier/builtInMultiplierID.h"
#include "Multiplier/constants.h"
#include "Multiplier/multiplierID.h"
#include "Multiplier/multiplierMeta.h"
#include "Registry/fixedMetadataRegistry.h"

namespace PocketCore::Registry::Multiplier
{
	using PocketCore::Configuration::MAX_MULTIPLIERS;
	using PocketCore::Core::us;
	using PocketCore::Effect::BuiltinEffectID;
	using PocketCore::Multiplier::BuiltinMultiplierID;
	using PocketCore::Multiplier::MultiplierApplicationPolicy;
	using PocketCore::Multiplier::MultiplierID;
	using PocketCore::Multiplier::MultiplierMeta;
	using PocketCore::Multiplier::toMultiplierID;
	using PocketCore::Registry::FixedMetadataRegistry;

	/*! @class MultiplierRegistry Registry/multiplierRegistry.h
		@brief Stores built-in and user-defined multiplier metadata in fixed-capacity storage.
		@details Built-in multipliers are registered during construction with IDs derived from @ref BuiltinMultiplierID. Configuration code
	   may append, replace, or remove entries through the low-level mutators while battle-time callers use allocation-free lookup
	   operations.
		@note Lookup operations are O(n), where n is bounded by @ref MAX_MULTIPLIERS.
		@date 07/30/2026
		@since 0.8.1
		@version 0.8.1
		@author Matthew Moore
	*/
	class MultiplierRegistry : private FixedMetadataRegistry<MultiplierMeta, MultiplierID, MAX_MULTIPLIERS, &MultiplierMeta::mMultiplierID>
	{
		private:
			using Base = FixedMetadataRegistry<MultiplierMeta, MultiplierID, MAX_MULTIPLIERS, &MultiplierMeta::mMultiplierID>;

		public:
			// LCOV_EXCL_START - If the built in additions fail, the program wouldn't work anyway
			/*! @brief Constructs a registry populated with every @ref BuiltinMultiplierID.
				@since 0.8.1
				@version 0.8.1
			 */
			ATTR_NOINLINE explicit constexpr MultiplierRegistry() : Base{toMultiplierID(BuiltinMultiplierID::FinalMultiplier).getValue()}
			{
				addBuiltin({
					.mName = PocketCore::Multiplier::MULTIPLIER_NAME_NONE,
					.mMultiplierID = toMultiplierID(BuiltinMultiplierID::None),
				});
				addBuiltin({
					.mName = PocketCore::Multiplier::MULTIPLIER_NAME_ABILITY,
					.mMultiplierID = toMultiplierID(BuiltinMultiplierID::Ability),
					.mApplicationPolicy = MultiplierApplicationPolicy::Other,
				});
				addBuiltin({
					.mName = PocketCore::Multiplier::MULTIPLIER_NAME_ITEM,
					.mMultiplierID = toMultiplierID(BuiltinMultiplierID::Item),
					.mApplicationPolicy = MultiplierApplicationPolicy::Other,
				});
				addBuiltin({
					.mName = PocketCore::Multiplier::MULTIPLIER_NAME_TARGETS,
					.mMultiplierID = toMultiplierID(BuiltinMultiplierID::Targets),
				});
				addBuiltin({
					.mName = PocketCore::Multiplier::MULTIPLIER_NAME_POPULATION_BOMB,
					.mMultiplierID = toMultiplierID(BuiltinMultiplierID::PopulationBomb),
				});
				addBuiltin({
					.mName = PocketCore::Multiplier::MULTIPLIER_NAME_WEATHER,
					.mMultiplierID = toMultiplierID(BuiltinMultiplierID::Weather),
				});
				addBuiltin({
					.mName = PocketCore::Multiplier::MULTIPLIER_NAME_CRITICAL,
					.mMultiplierID = toMultiplierID(BuiltinMultiplierID::Critical),
					.mApplicationPolicy = MultiplierApplicationPolicy::Floor,
				});
				addBuiltin({
					.mName = PocketCore::Multiplier::MULTIPLIER_NAME_RANDOMIZATION,
					.mMultiplierID = toMultiplierID(BuiltinMultiplierID::Randomization),
					.mApplicationPolicy = MultiplierApplicationPolicy::Floor,
				});
				addBuiltin({
					.mName = PocketCore::Multiplier::MULTIPLIER_NAME_STAB,
					.mMultiplierID = toMultiplierID(BuiltinMultiplierID::Stab),
				});
				addBuiltin({
					.mName = PocketCore::Multiplier::MULTIPLIER_NAME_TYPE_EFFECTIVENESS,
					.mMultiplierID = toMultiplierID(BuiltinMultiplierID::TypeEffectiveness),
					.mApplicationPolicy = MultiplierApplicationPolicy::Floor,
				});
				addBuiltin({
					.mName = PocketCore::Multiplier::MULTIPLIER_NAME_BURN,
					.mMultiplierID = toMultiplierID(BuiltinMultiplierID::Burn),
				});
			}

			// LCOV_EXCL_STOP

			using Base::decrementAmountRegistered;
			using Base::eraseEntry;
			using Base::findIndexByID;
			using Base::getAmountRegistered;
			using Base::getEntry;
			using Base::getID;
			using Base::getMetadata;
			using Base::getName;
			using Base::getNextID;
			using Base::getRegisteredEntries;
			using Base::hasEntry;
			using Base::incrementAmountRegistered;
			using Base::incrementNextID;
			using Base::setAmountRegistered;
			using Base::setEntry;
			using Base::setNextID;

			/*! @brief Looks up multiplier metadata by stable ID.
				@param[in] multiplierID The stable multiplier identifier.
				@return A non-owning pointer to metadata if registered, or nullptr otherwise. The pointer remains valid until replacement or
			   registry destruction.
				@since 0.8.1
				@version 0.8.1
			*/
			ATTR_NODISCARD constexpr const MultiplierMeta *getMultiplierMetadata(const MultiplierID multiplierID) const
			{
				return getMetadata(multiplierID);
			}

			/*! @brief Looks up an multiplier ID by display name.
				@param[in] name The case-sensitive display name.
				@return The stable ID if registered, or std::nullopt otherwise.
				@since 0.8.1
				@version 0.8.1
			*/
			ATTR_NODISCARD constexpr const std::optional<MultiplierID> getMultiplierID(const std::string_view &name) const
			{
				return getID(name);
			}

			/*! @brief Looks up an multiplier display name by stable ID.
				@param[in] multiplierID The stable multiplier identifier.
				@return The display name if registered, or std::nullopt otherwise.
				@since 0.8.1
				@version 0.8.1
			*/
			ATTR_NODISCARD constexpr const std::optional<std::string_view> getMultiplierName(const MultiplierID multiplierID) const
			{
				return getName(multiplierID);
			}

			/*! @brief Returns all currently registered multiplier definitions.
				@return A read-only span that remains valid until the registry is mutated or destroyed.
				@since 0.8.1
				@version 0.8.1
			*/
			ATTR_NODISCARD constexpr const std::span<const MultiplierMeta> getRegisteredMultipliers() const noexcept
			{
				return getRegisteredEntries();
			}

			/*! @brief Returns the next stable ID assigned to a custom multiplier.
				@return The underlying numeric value of the next multiplier ID.
				@since 0.8.1
				@version 0.8.1
			*/
			ATTR_NODISCARD constexpr us getNextMultiplierID() const noexcept
			{
				return getNextID();
			}

			/*! @brief Finds an internal array index by stable multiplier ID.
				@param[in] multiplierID The stable multiplier identifier.
				@return The internal index if registered, or std::nullopt otherwise.
				@since 0.8.1
				@version 0.8.1
			*/
			ATTR_NODISCARD constexpr const std::optional<us> findIndexByMultiplierID(const MultiplierID multiplierID) const
			{
				return findIndexByID(multiplierID);
			}

			/*! @brief Checks whether an multiplier name is registered.
				@param[in] name The case-sensitive display name.
				@return True if the name is registered, otherwise false.
				@since 0.8.1
				@version 0.8.1
			*/
			ATTR_NODISCARD constexpr bool hasMultiplier(const std::string_view &name) const
			{
				return hasEntry(name);
			}

			/*! @brief Checks whether an multiplier ID is registered.
				@param[in] multiplierID The stable multiplier identifier.
				@return True if the ID is registered, otherwise false.
				@since 0.8.1
				@version 0.8.1
			*/
			ATTR_NODISCARD constexpr bool hasMultiplier(const MultiplierID multiplierID) const
			{
				return hasEntry(multiplierID);
			}

			/*! @brief Sets the next custom multiplier ID counter.
				@param[in] nextID The next underlying ID value.
				@since 0.8.1
				@version 0.8.1
			*/
			constexpr void setNextMultiplierID(const us nextID) noexcept
			{
				setNextID(nextID);
			}

			/*! @brief Increments the next custom multiplier ID counter.
				@since 0.8.1
				@version 0.8.1
			 */
			constexpr void incrementNextMultiplierID() noexcept
			{
				incrementNextID();
			}
	};
} // namespace PocketCore::Registry::Multiplier

#endif
