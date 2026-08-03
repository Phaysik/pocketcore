/*! @file typeRegistry.h
	@brief Provides a compile-time registry for Pokemon types with fixed-capacity storage and lookup.
	@date 03/25/2026
	@version x.x.x
	@since x.x.x
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
#include "Types/typeEffectivenessID.h"
#include "Types/typeID.h"

#include "constants.h" // IWYU pragma: keep

namespace PocketCore::Registry::Types
{
	using PocketCore::Core::us;

	using PocketCore::Configuration::MAX_TYPES;
	using PocketCore::Types::NO_TYPE_EFFECTIVENESS_ID;
	using PocketCore::Types::TypeEffectivenessID;
	using PocketCore::Types::toTypeID;
	using PocketCore::Types::TypeID;

	namespace Types = PocketCore::Types;

	/*! @struct TypeEntry Registry/typeRegistry.h
		@brief A compile-time pair mapping a type ID to its display name.
	*/
	struct TypeEntry
	{
		public:
			/*! @brief The display name for the type. */
			std::string_view mName{};

			/*! @brief The stable identifier for a built-in or user-defined type. */
			TypeID mTypeID{};
	};

	/*! @class TypeRegistry Registry/typeRegistry.h
		@brief A constexpr registry storing Pokemon types with their effectiveness chart in fixed-capacity arrays.
		@details Stores up to @ref MAX_TYPES entries in fixed-size arrays with no dynamic allocation. Built-in @ref Types enum values are
	   pre-registered by the constructor. Custom type registration and removal are handled externally by
	   @ref PocketCore::Configuration::Configuration, which mutates the registry through its public getters and setters.
		@note All lookup operations are O(n) where n is the number of registered types due to linear search over a fixed-size array. This is
	   acceptable because n is bounded by @ref MAX_TYPES.
	*/
	class TypeRegistry
		: private PocketCore::Registry::FixedMetadataRegistry<TypeEntry, TypeID, MAX_TYPES, &TypeEntry::mTypeID, &TypeEntry::mName>
	{
		public:
			using Base = PocketCore::Registry::FixedMetadataRegistry<TypeEntry, TypeID, MAX_TYPES, &TypeEntry::mTypeID, &TypeEntry::mName>;

			// MARK: Constructor

			/*! @brief Constructs a registry pre-populated with all built-in @ref Types.
				@details Registers the 18 standard Pokemon types (Normal through Fairy) and Stellar with IDs matching their @ref Types enum
			   values and populates the corresponding rows of the effectiveness chart.
			*/
			ATTR_NOINLINE explicit constexpr TypeRegistry() : Base{static_cast<us>(toTypeID(Types::Types::Stellar).getValue() + 1U)}
			{
				// LCOV_EXCL_BR_START — Built-in types are guaranteed to be registered, so branch coverage for the addBuiltin precondition
				// is not applicable.
				addBuiltin(Types::NORMAL_TYPE_MATCHUP, Types::Types::Normal, Types::NORMAL_NAME);
				addBuiltin(Types::FIGHTING_TYPE_MATCHUP, Types::Types::Fighting, Types::FIGHTING_NAME);
				addBuiltin(Types::FLYING_TYPE_MATCHUP, Types::Types::Flying, Types::FLYING_NAME);
				addBuiltin(Types::POISON_TYPE_MATCHUP, Types::Types::Poison, Types::POISON_NAME);
				addBuiltin(Types::GROUND_TYPE_MATCHUP, Types::Types::Ground, Types::GROUND_NAME);
				addBuiltin(Types::ROCK_TYPE_MATCHUP, Types::Types::Rock, Types::ROCK_NAME);
				addBuiltin(Types::BUG_TYPE_MATCHUP, Types::Types::Bug, Types::BUG_NAME);
				addBuiltin(Types::GHOST_TYPE_MATCHUP, Types::Types::Ghost, Types::GHOST_NAME);
				addBuiltin(Types::STEEL_TYPE_MATCHUP, Types::Types::Steel, Types::STEEL_NAME);
				addBuiltin(Types::FIRE_TYPE_MATCHUP, Types::Types::Fire, Types::FIRE_NAME);
				addBuiltin(Types::WATER_TYPE_MATCHUP, Types::Types::Water, Types::WATER_NAME);
				addBuiltin(Types::GRASS_TYPE_MATCHUP, Types::Types::Grass, Types::GRASS_NAME);
				addBuiltin(Types::ELECTRIC_TYPE_MATCHUP, Types::Types::Electric, Types::ELECTRIC_NAME);
				addBuiltin(Types::PSYCHIC_TYPE_MATCHUP, Types::Types::Psychic, Types::PSYCHIC_NAME);
				addBuiltin(Types::ICE_TYPE_MATCHUP, Types::Types::Ice, Types::ICE_NAME);
				addBuiltin(Types::DRAGON_TYPE_MATCHUP, Types::Types::Dragon, Types::DRAGON_NAME);
				addBuiltin(Types::DARK_TYPE_MATCHUP, Types::Types::Dark, Types::DARK_NAME);
				addBuiltin(Types::FAIRY_TYPE_MATCHUP, Types::Types::Fairy, Types::FAIRY_NAME);
				addBuiltin(Types::Types::Stellar, Types::STELLAR_NAME);
				// LCOV_EXCL_BR_STOP
			}

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

			// MARK: Getters

			/*! @brief Returns the entry at the given index.
				@pre @p index < @ref MAX_TYPES.
				@param[in] index The index into the entries array.
				@return A copy of the @ref TypeEntry at that index.
			*/
			ATTR_NODISCARD constexpr const TypeEntry &getEntry(const us index) const
			{
				return Base::getEntry(index);
			}

			/*! @brief Returns a single cell from the type chart.
				@pre @p row < @ref MAX_TYPES and @p col < @ref MAX_TYPES.
				@param[in] row The attacking-type index.
				@param[in] col The defending-type index.
				@return The @ref TypeEffectivenessID value at that cell.
			*/
			ATTR_NODISCARD constexpr TypeEffectivenessID getTypeChartCell(const us row, const us col) const
			{
				assert(row < mTypeChart.size() && ROW_OOB_GET_TYPE_CHART_CELL.data());
				assert(col < mTypeChart.at(row).size() && COL_OOB_GET_TYPE_CHART_CELL.data());

				return mTypeChart.at(row).at(col);
			}

			/*! @brief Returns an entire row from the type chart.
				@pre @p row < @ref MAX_TYPES.
				@param[in] row The row index.
				@return A copy of the full effectiveness-ID row for the given type.
			*/
			ATTR_NODISCARD constexpr const std::array<TypeEffectivenessID, MAX_TYPES> &getTypeChartRow(const us row) const
			{
				assert(row < mTypeChart.size() && ROW_OOB_GET_TYPE_CHART_ROW.data());

				return mTypeChart.at(row);
			}

			/*! @brief Looks up a type ID by its display name.
				@note Time complexity is O(n) where n is @ref getAmountRegistered.
				@param[in] name The display name to search for.
				@return The type ID wrapped in std::optional if found, or std::nullopt if no type with that name is registered.
			*/
			ATTR_NODISCARD ATTR_NOINLINE constexpr std::optional<TypeID> getTypeID(const std::string_view &name) const
			{
				return Base::getID(name);
			}

			/*! @brief Looks up a type's display name by its ID.
				@note Time complexity is O(n) where n is @ref getAmountRegistered.
				@param[in] typeID The type ID to search for.
				@return The name wrapped in std::optional if found, or std::nullopt if no type with that ID is registered.
			*/
			ATTR_NODISCARD constexpr std::optional<std::string_view> getTypeName(const TypeID typeID) const
			{
				return Base::getName(typeID);
			}

			/*! @brief Returns the total number of registered types (built-in + custom).
				@return The count of registered types.
			*/
			ATTR_NODISCARD constexpr us getAmountRegistered() const noexcept
			{
				return Base::getAmountRegistered();
			}

			/*! @brief Returns the next type ID that will be assigned to a newly registered type.
				@return The next available stable type ID.
			*/
			ATTR_NODISCARD constexpr TypeID getNextTypeID() const noexcept
			{
				return TypeID{Base::getNextID()};
			}

			/*! @brief Returns a read-only span over all currently registered type entries.
				@details The returned span covers the first @ref getAmountRegistered elements of the internal entries array.
				@return A span of @ref TypeEntry covering all valid registered entries.
			*/
			ATTR_NODISCARD constexpr const std::span<const TypeEntry> getRegisteredTypes() const noexcept
			{
				return Base::getRegisteredEntries();
			}

			// MARK: Setters

			/*! @brief Sets the entry at the given index.
				@pre @p index < @ref MAX_TYPES.
				@param[in] index The index into the entries array.
				@param[in] entry The @ref TypeEntry to store.
			*/
			constexpr void setEntry(const us index, const TypeEntry &entry)
			{
				Base::setEntry(index, entry);
			}

			/*! @brief Sets a single cell in the type chart.
				@pre @p row < @ref MAX_TYPES and @p col < @ref MAX_TYPES.
				@param[in] row The attacking-type index.
				@param[in] col The defending-type index.
				@param[in] value The @ref TypeEffectivenessID value to store.
			*/
			constexpr void setTypeChartCell(const us row, const us col, const TypeEffectivenessID value)
			{
				assert(row < mTypeChart.size() && ROW_OOB_SET_TYPE_CHART_CELL.data());
				assert(col < mTypeChart.at(row).size() && COL_OOB_SET_TYPE_CHART_CELL.data());

				mTypeChart.at(row).at(col) = value;
			}

			/*! @brief Replaces an entire row in the type chart.
				@pre @p row < @ref MAX_TYPES.
				@param[in] row The row index.
				@param[in] chart The full row of @ref TypeEffectivenessID values to assign.
			*/
			constexpr void setTypeChartRow(const us row, const std::array<TypeEffectivenessID, MAX_TYPES> &chart)
			{
				assert(row < mTypeChart.size() && ROW_OOB_SET_TYPE_CHART_ROW.data());

				mTypeChart.at(row) = chart;
			}

			/*! @brief Sets the number of registered types.
				@param[in] amount The new count.
			*/
			constexpr void setAmountRegistered(const us amount) noexcept
			{
				Base::setAmountRegistered(amount);
			}

			/*! @brief Sets the next type ID counter.
				@param[in] nextId The value to assign to the next-type-ID counter.
			*/
			constexpr void setNextTypeID(const TypeID nextId) noexcept
			{
				Base::setNextID(nextId.getValue());
			}

			// MARK: Member Functions

			/*! @brief Finds the internal array index of a type given its stable type ID.
				@note Time complexity is O(n) where n is @ref getAmountRegistered.
				@param[in] typeID The stable type ID to search for.
				@return The array index wrapped in std::optional if found, or std::nullopt if no type with that ID is registered.
			*/
			ATTR_NODISCARD constexpr const std::optional<us> findIndexByTypeID(const TypeID typeID) const
			{
				return Base::findIndexByID(typeID);
			}

			/*! @brief Checks whether a type with the given name is registered.
				@note O(n) where n is @ref getAmountRegistered.
				@param[in] name The display name to check.
				@return True if a type with that name exists in the registry, false otherwise.
			*/
			ATTR_NODISCARD constexpr bool hasType(const std::string_view &name) const
			{
				return Base::hasEntry(name);
			}

			/*! @brief Checks whether a type with the given ID is registered.
				@note O(n) where n is @ref getAmountRegistered.
				@param[in] typeID The type ID to check.
				@return True if a type with that ID exists in the registry, false otherwise.
			*/
			ATTR_NODISCARD constexpr bool hasType(const TypeID typeID) const
			{
				return Base::hasEntry(typeID);
			}

			/*! @brief Increments the next type ID counter by one. */
			constexpr void incrementNextTypeID() noexcept
			{
				Base::incrementNextID();
			}

			/*! @brief Increments the number of registered types by one. */
			constexpr void incrementAmountRegistered() noexcept
			{
				Base::incrementAmountRegistered();
			}

			/*! @brief Decrements the number of registered types by one. */
			constexpr void decrementAmountRegistered() noexcept
			{
				Base::decrementAmountRegistered();
			}

			/*! @brief Registers a built-in enum type without a type chart row.
				@details Stores the entry via the shared fixed metadata base and increments the registered count.
			   Used for types like Stellar that have no offensive matchup data.
				@pre @ref getAmountRegistered() < @ref MAX_TYPES.
				@post @ref getAmountRegistered() is incremented by one.
				@param[in] type The built-in @ref Types enum value.
				@param[in] name The display name for the type.
			*/
			constexpr void addBuiltin(const Types::Types type, const std::string_view &name)
			{
				Base::addBuiltin(TypeEntry{.mName = name, .mTypeID = toTypeID(type)});
			}

			/*! @overload void addBuiltin(const Types type, std::string_view name)
				@brief Registers a built-in enum type together with its effectiveness row.
				@details Stores the type chart row in @ref mTypeChart then delegates to the name-only overload.
				@pre @ref getAmountRegistered() < @ref MAX_TYPES.
				@post @ref getAmountRegistered() is incremented by one and the corresponding @ref mTypeChart row is populated.
				@param[in] offensiveMatchups The full row of effectiveness-ID values for this type against all others.
				@param[in] type The built-in @ref Types enum value.
				@param[in] name The display name for the type.
			*/
			ATTR_NOINLINE constexpr void addBuiltin(const std::span<const TypeEffectivenessID> &offensiveMatchups,
													const Types::Types type, const std::string_view &name)
			{
				const us amountRegistered{Base::getAmountRegistered()};
				assert(offensiveMatchups.size() <= mTypeChart.at(amountRegistered).size() && MATCHUPS_EXCEED_COLUMNS_ADD_BUILTIN.data());

				for (std::size_t i{0}; i < offensiveMatchups.size(); ++i)
				{
					mTypeChart.at(amountRegistered).at(i) = offensiveMatchups.at(i);
				}

				addBuiltin(type, name);
			}

		private:
			/*! @brief Fixed-capacity 2-D array encoding the effectiveness of each type (row) attacking every other type (column).
				@details Indexed as mTypeChart[attacker][defender]. Uninitialized slots contain @ref NO_TYPE_EFFECTIVENESS_ID.
			*/
			std::array<std::array<TypeEffectivenessID, MAX_TYPES>, MAX_TYPES> mTypeChart{};
	};
} // namespace PocketCore::Registry::Types

#endif
