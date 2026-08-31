/*! @file typeRegistry.h
	@brief Provides a compile-time registry for Pokemon types with fixed-capacity storage and lookup.
	@date 08/26/2026
	@since 0.1.0
	@version 0.12.5
	@author Matthew Moore
*/

#ifndef INCLUDE_TYPES_TYPE_REGISTRY_H
#define INCLUDE_TYPES_TYPE_REGISTRY_H

#include <array>
#include <cassert>
#include <optional>
#include <span>
#include <string_view>

#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Registry/fixedMetadataRegistry.h"
#include "Types/builtInTypeID.h"
#include "Types/constants.h"
#include "Types/typeEffectiveness.h"
#include "Types/typeID.h"
#include "Types/typeMeta.h"

#include "constants.h" // IWYU pragma: keep

namespace PocketCore::Registry::Type
{
	using PocketCore::Core::us;

	using PocketCore::Configuration::MAX_TYPES;
	using PocketCore::Registry::FixedMetadataRegistry;
	using PocketCore::Type::BUG_NAME;
	using PocketCore::Type::BUG_TYPE_MATCHUP;
	using PocketCore::Type::BuiltInTypeID;
	using PocketCore::Type::DARK_NAME;
	using PocketCore::Type::DARK_TYPE_MATCHUP;
	using PocketCore::Type::DRAGON_NAME;
	using PocketCore::Type::DRAGON_TYPE_MATCHUP;
	using PocketCore::Type::ELECTRIC_NAME;
	using PocketCore::Type::ELECTRIC_TYPE_MATCHUP;
	using PocketCore::Type::FAIRY_NAME;
	using PocketCore::Type::FAIRY_TYPE_MATCHUP;
	using PocketCore::Type::FIGHTING_NAME;
	using PocketCore::Type::FIGHTING_TYPE_MATCHUP;
	using PocketCore::Type::FIRE_NAME;
	using PocketCore::Type::FIRE_TYPE_MATCHUP;
	using PocketCore::Type::FLYING_NAME;
	using PocketCore::Type::FLYING_TYPE_MATCHUP;
	using PocketCore::Type::GHOST_NAME;
	using PocketCore::Type::GHOST_TYPE_MATCHUP;
	using PocketCore::Type::GRASS_NAME;
	using PocketCore::Type::GRASS_TYPE_MATCHUP;
	using PocketCore::Type::GROUND_NAME;
	using PocketCore::Type::GROUND_TYPE_MATCHUP;
	using PocketCore::Type::ICE_NAME;
	using PocketCore::Type::ICE_TYPE_MATCHUP;
	using PocketCore::Type::NORMAL_NAME;
	using PocketCore::Type::NORMAL_TYPE_MATCHUP;
	using PocketCore::Type::POISON_NAME;
	using PocketCore::Type::POISON_TYPE_MATCHUP;
	using PocketCore::Type::PSYCHIC_NAME;
	using PocketCore::Type::PSYCHIC_TYPE_MATCHUP;
	using PocketCore::Type::ROCK_NAME;
	using PocketCore::Type::ROCK_TYPE_MATCHUP;
	using PocketCore::Type::STEEL_NAME;
	using PocketCore::Type::STEEL_TYPE_MATCHUP;
	using PocketCore::Type::STELLAR_NAME;
	using PocketCore::Type::toTypeID;
	using PocketCore::Type::TypeEffectiveness;
	using PocketCore::Type::TypeID;
	using PocketCore::Type::TypeMeta;
	using PocketCore::Type::WATER_NAME;
	using PocketCore::Type::WATER_TYPE_MATCHUP;

	/*! @class TypeRegistry Registry/typeRegistry.h
		@brief A constexpr registry storing Pokemon types with their effectiveness chart in fixed-capacity arrays.
		@details Stores up to @ref MAX_TYPES entries in fixed-size arrays with no dynamic allocation. Built-in @ref Types enum values are
	   pre-registered by the constructor. Custom type registration and removal are handled externally by
	   @ref PocketCore::Configuration::Configuration, which mutates the registry through its public getters and setters.
		@note All lookup operations are O(n) where n is the number of registered types due to linear search over a fixed-size array. This is
	   acceptable because n is bounded by @ref MAX_TYPES.
		@date 08/26/2026
		@since 0.1.0
		@version 0.12.5
	*/
	class TypeRegistry : private FixedMetadataRegistry<TypeMeta, TypeID, MAX_TYPES, &TypeMeta::mTypeID, &TypeMeta::mName>
	{
		public:
			using Base = FixedMetadataRegistry<TypeMeta, TypeID, MAX_TYPES, &TypeMeta::mTypeID, &TypeMeta::mName>;

			// MARK: Constructor

			/*! @brief Constructs a registry pre-populated with all built-in @ref Types.
				@details Registers the 18 standard Pokemon types (Normal through Fairy) and Stellar with IDs matching their @ref Types enum
			   values and populates the corresponding rows of the effectiveness chart.
				@since 0.1.0
				@version 0.12.5
			*/
			ATTR_NOINLINE explicit constexpr TypeRegistry() : Base{toTypeID(BuiltInTypeID::FinalType).getValue()}
			{
				// LCOV_EXCL_BR_START - Built-in types are guaranteed to be registered, so branch coverage for the addBuiltin precondition
				// is not applicable.
				addBuiltin({
					.mOffensiveMatchups = NORMAL_TYPE_MATCHUP,
					.mName = NORMAL_NAME,
					.mTypeID = toTypeID(BuiltInTypeID::Normal),
				});
				addBuiltin({
					.mOffensiveMatchups = FIGHTING_TYPE_MATCHUP,
					.mName = FIGHTING_NAME,
					.mTypeID = toTypeID(BuiltInTypeID::Fighting),
				});
				addBuiltin({
					.mOffensiveMatchups = FLYING_TYPE_MATCHUP,
					.mName = FLYING_NAME,
					.mTypeID = toTypeID(BuiltInTypeID::Flying),
				});
				addBuiltin({
					.mOffensiveMatchups = POISON_TYPE_MATCHUP,
					.mName = POISON_NAME,
					.mTypeID = toTypeID(BuiltInTypeID::Poison),
				});
				addBuiltin({
					.mOffensiveMatchups = GROUND_TYPE_MATCHUP,
					.mName = GROUND_NAME,
					.mTypeID = toTypeID(BuiltInTypeID::Ground),
				});
				addBuiltin({
					.mOffensiveMatchups = ROCK_TYPE_MATCHUP,
					.mName = ROCK_NAME,
					.mTypeID = toTypeID(BuiltInTypeID::Rock),
				});
				addBuiltin({
					.mOffensiveMatchups = BUG_TYPE_MATCHUP,
					.mName = BUG_NAME,
					.mTypeID = toTypeID(BuiltInTypeID::Bug),
				});
				addBuiltin({
					.mOffensiveMatchups = GHOST_TYPE_MATCHUP,
					.mName = GHOST_NAME,
					.mTypeID = toTypeID(BuiltInTypeID::Ghost),
				});
				addBuiltin({
					.mOffensiveMatchups = STEEL_TYPE_MATCHUP,
					.mName = STEEL_NAME,
					.mTypeID = toTypeID(BuiltInTypeID::Steel),
				});
				addBuiltin({
					.mOffensiveMatchups = FIRE_TYPE_MATCHUP,
					.mName = FIRE_NAME,
					.mTypeID = toTypeID(BuiltInTypeID::Fire),
				});
				addBuiltin({
					.mOffensiveMatchups = WATER_TYPE_MATCHUP,
					.mName = WATER_NAME,
					.mTypeID = toTypeID(BuiltInTypeID::Water),
				});
				addBuiltin({
					.mOffensiveMatchups = GRASS_TYPE_MATCHUP,
					.mName = GRASS_NAME,
					.mTypeID = toTypeID(BuiltInTypeID::Grass),
				});
				addBuiltin({
					.mOffensiveMatchups = ELECTRIC_TYPE_MATCHUP,
					.mName = ELECTRIC_NAME,
					.mTypeID = toTypeID(BuiltInTypeID::Electric),
				});
				addBuiltin({
					.mOffensiveMatchups = PSYCHIC_TYPE_MATCHUP,
					.mName = PSYCHIC_NAME,
					.mTypeID = toTypeID(BuiltInTypeID::Psychic),
				});
				addBuiltin({
					.mOffensiveMatchups = ICE_TYPE_MATCHUP,
					.mName = ICE_NAME,
					.mTypeID = toTypeID(BuiltInTypeID::Ice),
				});
				addBuiltin({
					.mOffensiveMatchups = DRAGON_TYPE_MATCHUP,
					.mName = DRAGON_NAME,
					.mTypeID = toTypeID(BuiltInTypeID::Dragon),
				});
				addBuiltin({
					.mOffensiveMatchups = DARK_TYPE_MATCHUP,
					.mName = DARK_NAME,
					.mTypeID = toTypeID(BuiltInTypeID::Dark),
				});
				addBuiltin({
					.mOffensiveMatchups = FAIRY_TYPE_MATCHUP,
					.mName = FAIRY_NAME,
					.mTypeID = toTypeID(BuiltInTypeID::Fairy),
				});
				addBuiltin({.mName = STELLAR_NAME, .mTypeID = toTypeID(BuiltInTypeID::Stellar)});
				// LCOV_EXCL_BR_STOP
			}

			using Base::decrementAmountRegistered;
			using Base::eraseEntry;
			using Base::findIndexByID;
			using Base::getAmountRegistered;
			using Base::getEntry;
			using Base::getID;
			using Base::getMetadata;
			using Base::getMutableEntry;
			using Base::getName;
			using Base::getNextID;
			using Base::getRegisteredEntries;
			using Base::hasEntry;
			using Base::incrementAmountRegistered;
			using Base::incrementNextID;
			using Base::setAmountRegistered;
			using Base::setEntry;
			using Base::setNextID;

			// MARK: Getters

			/*! @brief Returns a single cell from the type chart.
				@pre @p row < @ref MAX_TYPES and @p col < @ref MAX_TYPES.
				@param[in] row The attacking-type index.
				@param[in] col The defending-type index.
				@return The @ref TypeEffectiveness value at that cell.
				@since 0.1.0
				@version 0.12.5
			*/
			ATTR_NODISCARD constexpr TypeEffectiveness getTypeChartCell(const us row, const us col) const
			{
				assert(row < MAX_TYPES && ROW_OOB_GET_TYPE_CHART_CELL.data());
				assert(col < MAX_TYPES && COL_OOB_GET_TYPE_CHART_CELL.data());

				return getEntry(row).mOffensiveMatchups.at(col);
			}

			/*! @brief Returns an entire row from the type chart.
				@pre @p row < @ref MAX_TYPES.
				@param[in] row The row index.
				@return A copy of the full effectiveness row for the given type.
				@since 0.1.0
				@version 0.12.5
			*/
			ATTR_NODISCARD constexpr const std::array<TypeEffectiveness, MAX_TYPES> &getTypeChartRow(const us row) const
			{
				assert(row < MAX_TYPES && ROW_OOB_GET_TYPE_CHART_ROW.data());

				return getEntry(row).mOffensiveMatchups;
			}

			/*! @brief Looks up a type ID by its display name.
				@note Time complexity is O(n) where n is @ref getAmountRegistered.
				@param[in] name The display name to search for.
				@return The type ID wrapped in std::optional if found, or std::nullopt if no type with that name is registered.
				@since 0.1.0
				@version 0.12.5
			*/
			ATTR_NODISCARD ATTR_NOINLINE constexpr std::optional<TypeID> getTypeID(const std::string_view &name) const
			{
				return getID(name);
			}

			/*! @brief Looks up a type's display name by its ID.
				@note Time complexity is O(n) where n is @ref getAmountRegistered.
				@param[in] typeID The type ID to search for.
				@return The name wrapped in std::optional if found, or std::nullopt if no type with that ID is registered.
				@since 0.1.0
				@version 0.12.5
			*/
			ATTR_NODISCARD constexpr std::optional<std::string_view> getTypeName(const TypeID typeID) const
			{
				return getName(typeID);
			}

			/*! @brief Returns the next type ID that will be assigned to a newly registered type.
				@return The next available stable type ID.
				@since 0.1.0
				@version 0.12.5
			*/
			ATTR_NODISCARD constexpr TypeID getNextTypeID() const noexcept
			{
				return TypeID{getNextID()};
			}

			/*! @brief Returns a read-only span over all currently registered type entries.
				@details The returned span covers the first @ref getAmountRegistered elements of the internal entries array.
				@return A span of @ref TypeMeta covering all valid registered entries.
				@since 0.1.0
				@version 0.12.5
			*/
			ATTR_NODISCARD constexpr const std::span<const TypeMeta> getRegisteredTypes() const noexcept
			{
				return getRegisteredEntries();
			}

			// MARK: Setters

			/*! @brief Sets a single cell in the type chart.
				@pre @p row < @ref MAX_TYPES and @p col < @ref MAX_TYPES.
				@param[in] row The attacking-type index.
				@param[in] col The defending-type index.
				@param[in] value The @ref TypeEffectiveness value to store.
				@since 0.1.0
				@version 0.12.5
			*/
			constexpr void setTypeChartCell(const us row, const us col, const TypeEffectiveness value)
			{
				assert(row < MAX_TYPES && ROW_OOB_SET_TYPE_CHART_CELL.data());
				assert(col < MAX_TYPES && COL_OOB_SET_TYPE_CHART_CELL.data());

				getMutableEntry(row).mOffensiveMatchups.at(col) = value;
			}

			/*! @brief Replaces an entire row in the type chart.
				@pre @p row < @ref MAX_TYPES.
				@param[in] row The row index.
				@param[in] chart The full row of @ref TypeEffectiveness values to assign.
				@since 0.1.0
				@version 0.12.5
			*/
			constexpr void setTypeChartRow(const us row, const std::array<TypeEffectiveness, MAX_TYPES> &chart)
			{
				assert(row < MAX_TYPES && ROW_OOB_SET_TYPE_CHART_ROW.data());

				getMutableEntry(row).mOffensiveMatchups = chart;
			}

			/*! @brief Sets the next type ID counter.
				@param[in] nextId The value to assign to the next-type-ID counter.
				@since 0.1.0
				@version 0.12.5
			*/
			constexpr void setNextTypeID(const TypeID nextId) noexcept
			{
				setNextID(nextId.getValue());
			}

			// MARK: Member Functions

			/*! @brief Finds the internal array index of a type given its stable type ID.
				@note Time complexity is O(n) where n is @ref getAmountRegistered.
				@param[in] typeID The stable type ID to search for.
				@return The array index wrapped in std::optional if found, or std::nullopt if no type with that ID is registered.
				@since 0.1.0
				@version 0.12.5
			*/
			ATTR_NODISCARD constexpr const std::optional<us> findIndexByTypeID(const TypeID typeID) const
			{
				return findIndexByID(typeID);
			}

			/*! @brief Checks whether a type with the given name is registered.
				@note O(n) where n is @ref getAmountRegistered.
				@param[in] name The display name to check.
				@return True if a type with that name exists in the registry, false otherwise.
				@since 0.1.0
				@version 0.12.5
			*/
			ATTR_NODISCARD constexpr bool hasType(const std::string_view &name) const
			{
				return hasEntry(name);
			}

			/*! @brief Checks whether a type with the given ID is registered.
				@note O(n) where n is @ref getAmountRegistered.
				@param[in] typeID The type ID to check.
				@return True if a type with that ID exists in the registry, false otherwise.
				@since 0.1.0
				@version 0.12.5
			*/
			ATTR_NODISCARD constexpr bool hasType(const TypeID typeID) const
			{
				return hasEntry(typeID);
			}

			/*! @brief Increments the next type ID counter by one.
				@since 0.1.0
				@version 0.12.5
			 */
			constexpr void incrementNextTypeID() noexcept
			{
				incrementNextID();
			}
	};
} // namespace PocketCore::Registry::Type

#endif
