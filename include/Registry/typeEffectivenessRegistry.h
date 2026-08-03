/*! @file typeEffectivenessRegistry.h
	@brief Provides fixed-capacity storage and lookup for built-in and user-defined type-effectiveness entries.
	@date 07/27/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_REGISTRY_TYPEEFFECTIVENESS_REGISTRY_H
#define INCLUDE_REGISTRY_TYPEEFFECTIVENESS_REGISTRY_H

#include <optional>
#include <span>
#include <string_view>

#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Registry/fixedMetadataRegistry.h"
#include "Types/builtInTypeEffectivenessID.h"
#include "Types/constants.h"
#include "Types/typeEffectivenessID.h"
#include "Types/typeEffectivenessMeta.h"

namespace PocketCore::Registry::Types
{
	using PocketCore::Configuration::TYPE_EFFECTIVENESS_MAX_AMOUNT;
	using PocketCore::Core::us;
	using PocketCore::Types::BuiltInTypeEffectivenessID;
	using PocketCore::Types::toTypeEffectivenessID;
	using PocketCore::Types::TypeEffectivenessID;
	using PocketCore::Types::TypeEffectivenessMeta;

	/*! @class TypeEffectivenessRegistry Registry/typeEffectivenessRegistry.h
		@brief Stores built-in and user-defined type-effectiveness metadata in fixed-capacity storage.
		@details Built-in type-effectiveness entries are registered during construction with IDs derived from
		@ref BuiltInTypeEffectivenessID.
	   Configuration code may append, replace, or remove entries through the low-level mutators while battle-time callers use
	   allocation-free lookup operations.
		@note Lookup operations are O(n), where n is bounded by @ref TYPE_EFFECTIVENESS_MAX_AMOUNT.
		@date 07/27/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	class TypeEffectivenessRegistry
		: private PocketCore::Registry::FixedMetadataRegistry<TypeEffectivenessMeta, TypeEffectivenessID, TYPE_EFFECTIVENESS_MAX_AMOUNT,
															  &TypeEffectivenessMeta::mTypeEffectivenessID>
	{
		private:
			using Base
				= PocketCore::Registry::FixedMetadataRegistry<TypeEffectivenessMeta, TypeEffectivenessID, TYPE_EFFECTIVENESS_MAX_AMOUNT,
															  &TypeEffectivenessMeta::mTypeEffectivenessID>;

		public:
			// LCOV_EXCL_START - If the built in additions fail, the program wouldn't work anyway
			/*! @brief Constructs a registry populated with every @ref BuiltInTypeEffectivenessID. */
			ATTR_NOINLINE explicit constexpr TypeEffectivenessRegistry()
				: Base{static_cast<us>(toTypeEffectivenessID(BuiltInTypeEffectivenessID::SE).getValue() + 1U)}
			{
				addBuiltin({
					.mName = PocketCore::Types::TYPE_EFFECTIVENESS_NAME_NE,
					.mTypeEffectivenessValue = 0.0F,
					.mTypeEffectivenessID = toTypeEffectivenessID(BuiltInTypeEffectivenessID::NE),
				});
				addBuiltin({
					.mName = PocketCore::Types::TYPE_EFFECTIVENESS_NAME_NVE,
					.mTypeEffectivenessID = toTypeEffectivenessID(BuiltInTypeEffectivenessID::NVE),
				});
				addBuiltin({
					.mName = PocketCore::Types::TYPE_EFFECTIVENESS_NAME_E,
					.mTypeEffectivenessID = toTypeEffectivenessID(BuiltInTypeEffectivenessID::E),
				});
				addBuiltin({
					.mName = PocketCore::Types::TYPE_EFFECTIVENESS_NAME_SE,
					.mTypeEffectivenessID = toTypeEffectivenessID(BuiltInTypeEffectivenessID::SE),
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

			/*! @brief Looks up typeEffectiveness metadata by stable ID.
				@param[in] typeEffectivenessID The stable typeEffectiveness identifier.
				@return A non-owning pointer to metadata if registered, or nullptr otherwise. The pointer remains valid until replacement or
			   registry destruction.
			*/
			ATTR_NODISCARD constexpr const TypeEffectivenessMeta *getTypeEffectivenessMetadata(
				const TypeEffectivenessID typeEffectivenessID) const
			{
				return getMetadata(typeEffectivenessID);
			}

			/*! @brief Looks up a type-effectiveness ID by display name.
				@param[in] name The case-sensitive display name.
				@return The stable ID if registered, or std::nullopt otherwise.
			*/
			ATTR_NODISCARD constexpr const std::optional<TypeEffectivenessID> getTypeEffectivenessID(const std::string_view &name) const
			{
				return getID(name);
			}

			/*! @brief Looks up a type-effectiveness display name by stable ID.
				@param[in] typeEffectivenessID The stable typeEffectiveness identifier.
				@return The display name if registered, or std::nullopt otherwise.
			*/
			ATTR_NODISCARD constexpr const std::optional<std::string_view> getTypeEffectivenessName(
				const TypeEffectivenessID typeEffectivenessID) const
			{
				return getName(typeEffectivenessID);
			}

			/*! @brief Returns all currently registered typeEffectiveness definitions.
				@return A read-only span that remains valid until the registry is mutated or destroyed.
			*/
			ATTR_NODISCARD constexpr const std::span<const TypeEffectivenessMeta> getRegisteredTypeEffectivenesss() const noexcept
			{
				return getRegisteredEntries();
			}

			/*! @brief Returns the next stable ID assigned to a custom typeEffectiveness.
				@return The underlying numeric value of the next typeEffectiveness ID.
			*/
			ATTR_NODISCARD constexpr us getNextTypeEffectivenessID() const noexcept
			{
				return getNextID();
			}

			/*! @brief Finds an internal array index by stable typeEffectiveness ID.
				@param[in] typeEffectivenessID The stable typeEffectiveness identifier.
				@return The internal index if registered, or std::nullopt otherwise.
			*/
			ATTR_NODISCARD constexpr const std::optional<us> findIndexByTypeEffectivenessID(
				const TypeEffectivenessID typeEffectivenessID) const
			{
				return findIndexByID(typeEffectivenessID);
			}

			/*! @brief Checks whether a type-effectiveness name is registered.
				@param[in] name The case-sensitive display name.
				@return True if the name is registered, otherwise false.
			*/
			ATTR_NODISCARD constexpr bool hasTypeEffectiveness(const std::string_view &name) const
			{
				return hasEntry(name);
			}

			/*! @brief Checks whether a type-effectiveness ID is registered.
				@param[in] typeEffectivenessID The stable typeEffectiveness identifier.
				@return True if the ID is registered, otherwise false.
			*/
			ATTR_NODISCARD constexpr bool hasTypeEffectiveness(const TypeEffectivenessID typeEffectivenessID) const
			{
				return hasEntry(typeEffectivenessID);
			}

			/*! @brief Sets the next custom typeEffectiveness ID counter.
				@param[in] nextID The next underlying ID value.
			*/
			constexpr void setNextTypeEffectivenessID(const us nextID) noexcept
			{
				setNextID(nextID);
			}

			/*! @brief Increments the next custom typeEffectiveness ID counter. */
			constexpr void incrementNextTypeEffectivenessID() noexcept
			{
				incrementNextID();
			}
	};
} // namespace PocketCore::Registry::Types

#endif
