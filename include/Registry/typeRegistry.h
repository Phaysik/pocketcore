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
#include "Types/constants.h"
#include "Types/typeEffectiveness.h"
#include "Types/types.h"

#include "constants.h" // IWYU pragma: keep

namespace Pokemon::Registry::Types
{
	using Pokemon::Core::ub;

	using Pokemon::Configuration::MAX_TYPES;

	namespace Types = Pokemon::Types;

	/*! @struct TypeEntry Registry/typeRegistry.h
		@brief A compile-time pair mapping a type ID to its display name.
	*/
	struct TypeEntry
	{
			/*! @var typeId
				@brief The numeric type identifier.
			*/
			ub typeId{};

			/*! @var name
				@brief The display name for the type.
			*/
			std::string_view name{};
	};

	/*! @class TypeRegistry Registry/typeRegistry.h
		@brief A constexpr registry storing Pokemon types with their effectiveness chart in fixed-capacity arrays.
		@details Stores up to @ref MAX_TYPES entries in fixed-size arrays with no dynamic allocation. Built-in @ref Types enum values are
	   pre-registered by the constructor. Custom type registration and removal are handled externally by
	   @ref Pokemon::Configuration::Configuration, which mutates the registry through its public getters and setters.
		@note All lookup operations are O(n) where n is the number of registered types due to linear search over a fixed-size array. This is
	   acceptable because n is bounded by @ref MAX_TYPES.
	*/
	class TypeRegistry
	{
		public:
			// MARK: Constructor

			/*! @brief Constructs a registry pre-populated with all built-in @ref Types.
				@details Registers the 18 standard Pokemon types (Normal through Fairy) and Stellar with IDs matching their @ref Types enum
			   values and populates the corresponding rows of the effectiveness chart.
			*/
			explicit constexpr TypeRegistry()
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

				// Initialize next type ID to the first available ID after the built-in types. This ensures that custom types receive stable
				// IDs that do not conflict with built-in ones, even if some built-in types are removed later.

				// NOLINTNEXTLINE(cppcoreguidelines-prefer-member-initializer)
				mNextTypeId = mAmountRegistered;
			}

			// MARK: Getters

			/*! @brief Returns the entry at the given index.
				@pre @p index < @ref MAX_TYPES.
				@param[in] index The index into the entries array.
				@return A copy of the @ref TypeEntry at that index.
			*/
			ATTR_NODISCARD constexpr TypeEntry getEntry(const ub index)
			{
				assert(index < mEntries.size() && INDEX_OOB_GET_ENTRY.data());

				return mEntries.at(index);
			}

			/*! @brief Returns a single cell from the type chart.
				@pre @p row < @ref MAX_TYPES and @p col < @ref MAX_TYPES.
				@param[in] row The attacking-type index.
				@param[in] col The defending-type index.
				@return The @ref TypeEffectiveness value at that cell.
			*/
			ATTR_NODISCARD constexpr Types::TypeEffectiveness getTypeChartCell(const ub row, const ub col)
			{
				assert(row < mTypeChart.size() && ROW_OOB_GET_TYPE_CHART_CELL.data());
				assert(col < mTypeChart.at(row).size() && COL_OOB_GET_TYPE_CHART_CELL.data());

				return mTypeChart.at(row).at(col);
			}

			/*! @brief Returns an entire row from the type chart.
				@pre @p row < @ref MAX_TYPES.
				@param[in] row The row index.
				@return A copy of the full effectiveness row for the given type.
			*/
			ATTR_NODISCARD constexpr std::array<Types::TypeEffectiveness, MAX_TYPES> getTypeChartRow(const ub row)
			{
				assert(row < mTypeChart.size() && ROW_OOB_GET_TYPE_CHART_ROW.data());

				return mTypeChart.at(row);
			}

			/*! @brief Looks up a type ID by its display name.
				@note Time complexity is O(n) where n is @ref getAmountRegistered.
				@param[in] name The display name to search for.
				@return The type ID wrapped in std::optional if found, or std::nullopt if no type with that name is registered.
			*/
			ATTR_NODISCARD ATTR_NOINLINE constexpr std::optional<ub> getTypeId(const std::string_view name) const
			{
				const ub index{findEntryIndexByName(name)};

				if (index == mAmountRegistered)
				{
					return std::nullopt;
				}

				assert(index < mEntries.size() && INDEX_OOB_GET_TYPE_ID.data());

				return mEntries.at(index).typeId; // LCOV_EXCL_BR_LINE — The .at() exception branch is unreachable; the preceding assert
												  // already guarantees index < size.
			}

			/*! @brief Looks up a type's display name by its ID.
				@note Time complexity is O(n) where n is @ref getAmountRegistered.
				@param[in] typeId The type ID to search for.
				@return The name wrapped in std::optional if found, or std::nullopt if no type with that ID is registered.
			*/
			ATTR_NODISCARD constexpr std::optional<std::string_view> getTypeName(const ub typeId) const
			{
				const ub index{findEntryIndexById(typeId)};

				if (index == mAmountRegistered)
				{
					return std::nullopt;
				}

				assert(index < mEntries.size() && INDEX_OOB_GET_TYPE_NAME.data());

				return mEntries.at(index).name;
			}

			/*! @brief Returns the total number of registered types (built-in + custom).
				@return The count of registered types.
			*/
			ATTR_NODISCARD constexpr ub getAmountRegistered() const noexcept
			{
				return mAmountRegistered;
			}

			/*! @brief Returns the next type ID that will be assigned to a newly registered type.
				@return The next available stable type ID.
			*/
			ATTR_NODISCARD constexpr ub getNextTypeId() const noexcept
			{
				return mNextTypeId;
			}

			/*! @brief Returns a read-only span over all currently registered type entries.
				@details The returned span covers the first @ref getAmountRegistered elements of the internal entries array.
				@return A span of @ref TypeEntry covering all valid registered entries.
			*/
			ATTR_NODISCARD constexpr std::span<const TypeEntry> getRegisteredTypes() const noexcept
			{
				return {mEntries.data(), mAmountRegistered};
			}

			// MARK: Setters

			/*! @brief Sets the entry at the given index.
				@pre @p index < @ref MAX_TYPES.
				@param[in] index The index into the entries array.
				@param[in] entry The @ref TypeEntry to store.
			*/
			constexpr void setEntry(const ub index, const TypeEntry &entry)
			{
				assert(index < mEntries.size() && INDEX_OOB_SET_ENTRY.data());

				mEntries.at(index) = entry;
			}

			/*! @brief Sets a single cell in the type chart.
				@pre @p row < @ref MAX_TYPES and @p col < @ref MAX_TYPES.
				@param[in] row The attacking-type index.
				@param[in] col The defending-type index.
				@param[in] value The @ref TypeEffectiveness value to store.
			*/
			constexpr void setTypeChartCell(const ub row, const ub col, const Types::TypeEffectiveness value)
			{
				assert(row < mTypeChart.size() && ROW_OOB_SET_TYPE_CHART_CELL.data());
				assert(col < mTypeChart.at(row).size() && COL_OOB_SET_TYPE_CHART_CELL.data());

				mTypeChart.at(row).at(col) = value;
			}

			/*! @brief Replaces an entire row in the type chart.
				@pre @p row < @ref MAX_TYPES.
				@param[in] row The row index.
				@param[in] chart The full row of @ref TypeEffectiveness values to assign.
			*/
			constexpr void setTypeChartRow(const ub row, const std::array<Types::TypeEffectiveness, MAX_TYPES> &chart)
			{
				assert(row < mTypeChart.size() && ROW_OOB_SET_TYPE_CHART_ROW.data());

				mTypeChart.at(row) = chart;
			}

			/*! @brief Sets the number of registered types.
				@param[in] amount The new count.
			*/
			constexpr void setAmountRegistered(const ub amount) noexcept
			{
				mAmountRegistered = amount;
			}

			/*! @brief Sets the next type ID counter.
				@param[in] nextId The value to assign to the next-type-ID counter.
			*/
			constexpr void setNextTypeId(const ub nextId) noexcept
			{
				mNextTypeId = nextId;
			}

			// MARK: Member Functions

			/*! @brief Finds the internal array index of a type given its stable type ID.
				@note Time complexity is O(n) where n is @ref getAmountRegistered.
				@param[in] typeId The stable type ID to search for.
				@return The array index wrapped in std::optional if found, or std::nullopt if no type with that ID is registered.
			*/
			ATTR_NODISCARD constexpr std::optional<ub> findIndexByTypeId(const ub typeId) const
			{
				// LCOV_EXCL_BR_START — The .at() exception branch inside findEntryIndexById is unreachable; the loop index is bounded by
				// mAmountRegistered which is always < size.
				const ub index{findEntryIndexById(typeId)};
				// LCOV_EXCL_BR_STOP

				if (index == mAmountRegistered)
				{
					return std::nullopt;
				}

				return index;
			}

			/*! @brief Checks whether a type with the given name is registered.
				@note O(n) where n is @ref getAmountRegistered.
				@param[in] name The display name to check.
				@return True if a type with that name exists in the registry, false otherwise.
			*/
			ATTR_NODISCARD constexpr bool hasType(const std::string_view name) const
			{
				return findEntryIndexByName(name) != mAmountRegistered;
			}

			/*! @brief Checks whether a type with the given ID is registered.
				@note O(n) where n is @ref getAmountRegistered.
				@param[in] typeId The type ID to check.
				@return True if a type with that ID exists in the registry, false otherwise.
			*/
			ATTR_NODISCARD constexpr bool hasType(const ub typeId) const
			{
				return findEntryIndexById(typeId) != mAmountRegistered;
			}

			/*! @brief Increments the next type ID counter by one. */
			constexpr void incrementNextTypeId() noexcept
			{
				++mNextTypeId;
			}

			/*! @brief Increments the number of registered types by one. */
			constexpr void incrementAmountRegistered() noexcept
			{
				++mAmountRegistered;
			}

			/*! @brief Decrements the number of registered types by one. */
			constexpr void decrementAmountRegistered() noexcept
			{
				--mAmountRegistered;
			}

		private:
			// MARK: Private Member Functions

			/*! @brief Finds the raw array index of a type entry by display name.
				@details Returns @ref mAmountRegistered as a sentinel when not found, avoiding @ref std::optional construction overhead.
				@note Time complexity is O(n) where n is @ref mAmountRegistered.
				@param[in] name The display name to search for.
				@return The array index if found, or @ref mAmountRegistered if no type with that name is registered.
			*/
			ATTR_NODISCARD constexpr ub findEntryIndexByName(const std::string_view name) const
			{
				assert(mAmountRegistered < mEntries.size() && REGISTERED_EXCEEDS_ENTRIES_FIND_BY_NAME.data());

				for (ub i{0}; i < mAmountRegistered; ++i)
				{
					if (mEntries.at(i).name == name)
					{
						return i;
					}
				}

				return mAmountRegistered;
			}

			/*! @brief Finds the raw array index of a type entry by its stable type ID.
				@details Returns @ref mAmountRegistered as a sentinel when not found, avoiding @ref std::optional construction overhead.
				@note Time complexity is O(n) where n is @ref mAmountRegistered.
				@param[in] typeId The stable type ID to search for.
				@return The array index if found, or @ref mAmountRegistered if no type with tha,t ID is registered.
			*/
			ATTR_NODISCARD constexpr ub findEntryIndexById(const ub typeId) const
			{
				assert(mAmountRegistered < mEntries.size() && REGISTERED_EXCEEDS_ENTRIES_FIND_BY_ID.data());

				for (ub i{0}; i < mAmountRegistered; ++i)
				{
					if (mEntries.at(i).typeId == typeId)
					{
						return i;
					}
				}

				return mAmountRegistered;
			}

			/*! @brief Registers a built-in enum type without a type chart row.
				@details Stores the entry in @ref mEntries at the current @ref mAmountRegistered index and increments the count.
			   Used for types like Stellar that have no offensive matchup data.
				@pre @ref mAmountRegistered < @ref MAX_TYPES.
				@post @ref mAmountRegistered is incremented by one.
				@param[in] type The built-in @ref Types enum value.
				@param[in] name The display name for the type.
			*/
			constexpr void addBuiltin(const Types::Types type, std::string_view name)
			{
				assert(mAmountRegistered < mEntries.size() && REGISTERED_EXCEEDS_ENTRIES_ADD_BUILTIN.data());

				mEntries.at(mAmountRegistered) = TypeEntry{.typeId = static_cast<ub>(type), .name = name};
				++mAmountRegistered;
			}

			/*! @overload void addBuiltin(const Types type, std::string_view name)
				@brief Registers a built-in enum type together with its effectiveness row.
				@details Stores the type chart row in @ref mTypeChart then delegates to the name-only overload.
				@pre @ref mAmountRegistered < @ref MAX_TYPES.
				@post @ref mAmountRegistered is incremented by one and the corresponding @ref mTypeChart row is populated.
				@param[in] offensiveMatchups The full row of effectiveness values for this type against all others.
				@param[in] type The built-in @ref Types enum value.
				@param[in] name The display name for the type.
			*/
			constexpr void addBuiltin(const std::span<const Types::TypeEffectiveness> &offensiveMatchups, const Types::Types type,
									  std::string_view name)
			{
				assert(mAmountRegistered < mEntries.size() && REGISTERED_EXCEEDS_ENTRIES_ADD_BUILTIN.data());
				assert(offensiveMatchups.size() <= mTypeChart.at(mAmountRegistered).size() && MATCHUPS_EXCEED_COLUMNS_ADD_BUILTIN.data());

				for (std::size_t i{0}; i < offensiveMatchups.size(); ++i)
				{
					mTypeChart.at(mAmountRegistered).at(i) = offensiveMatchups.at(i);
				}

				addBuiltin(type, name);
			}

		private:
			/*! @brief Fixed-capacity 2-D array encoding the effectiveness of each type (row) attacking every other type (column).
				@details Indexed as mTypeChart[attacker][defender]. Uninitialized slots contain @ref TypeEffectiveness::NOT_DEFINED.
			*/
			std::array<std::array<Types::TypeEffectiveness, MAX_TYPES>, MAX_TYPES> mTypeChart{};

			/*! @brief Fixed-capacity array of type entries (ID + name pairs).
				@details Only the first @ref mAmountRegistered elements are valid.
			*/
			std::array<TypeEntry, MAX_TYPES> mEntries{};

			/*! @brief The number of currently registered types.
			 */
			ub mAmountRegistered{0};

			/*! @brief Monotonically increasing counter for assigning stable type IDs.
				@details Ensures that each type receives a unique ID that is never reused, even after removals.
			*/
			ub mNextTypeId{0};
	};
} // namespace Pokemon::Registry::Types

#endif
