/*! @file typeRegistry.h
	@brief Provides a compile-time registry for Pokemon types with fixed-capacity storage and lookup.
	@date 09/03/2026
	@since 0.1.0
	@version 0.12.19
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
	using PocketCore::Type::BUG_TYPE_MATCHUP;
	using PocketCore::Type::BuiltinTypeID;
	using PocketCore::Type::DARK_TYPE_MATCHUP;
	using PocketCore::Type::DRAGON_TYPE_MATCHUP;
	using PocketCore::Type::ELECTRIC_TYPE_MATCHUP;
	using PocketCore::Type::FAIRY_TYPE_MATCHUP;
	using PocketCore::Type::FIGHTING_TYPE_MATCHUP;
	using PocketCore::Type::FIRE_TYPE_MATCHUP;
	using PocketCore::Type::FLYING_TYPE_MATCHUP;
	using PocketCore::Type::GHOST_TYPE_MATCHUP;
	using PocketCore::Type::GRASS_TYPE_MATCHUP;
	using PocketCore::Type::GROUND_TYPE_MATCHUP;
	using PocketCore::Type::ICE_TYPE_MATCHUP;
	using PocketCore::Type::INVALID_TYPE_CHART_ROW;
	using PocketCore::Type::NORMAL_TYPE_MATCHUP;
	using PocketCore::Type::POISON_TYPE_MATCHUP;
	using PocketCore::Type::PSYCHIC_TYPE_MATCHUP;
	using PocketCore::Type::ROCK_TYPE_MATCHUP;
	using PocketCore::Type::STEEL_TYPE_MATCHUP;
	using PocketCore::Type::toTypeID;
	using PocketCore::Type::TYPE_NAME_BUG;
	using PocketCore::Type::TYPE_NAME_DARK;
	using PocketCore::Type::TYPE_NAME_DRAGON;
	using PocketCore::Type::TYPE_NAME_ELECTRIC;
	using PocketCore::Type::TYPE_NAME_FAIRY;
	using PocketCore::Type::TYPE_NAME_FIGHTING;
	using PocketCore::Type::TYPE_NAME_FIRE;
	using PocketCore::Type::TYPE_NAME_FLYING;
	using PocketCore::Type::TYPE_NAME_GHOST;
	using PocketCore::Type::TYPE_NAME_GRASS;
	using PocketCore::Type::TYPE_NAME_GROUND;
	using PocketCore::Type::TYPE_NAME_ICE;
	using PocketCore::Type::TYPE_NAME_NONE;
	using PocketCore::Type::TYPE_NAME_NORMAL;
	using PocketCore::Type::TYPE_NAME_POISON;
	using PocketCore::Type::TYPE_NAME_PSYCHIC;
	using PocketCore::Type::TYPE_NAME_ROCK;
	using PocketCore::Type::TYPE_NAME_STEEL;
	using PocketCore::Type::TYPE_NAME_STELLAR;
	using PocketCore::Type::TYPE_NAME_WATER;
	using PocketCore::Type::TypeEffectiveness;
	using PocketCore::Type::TypeID;
	using PocketCore::Type::TypeMeta;
	using PocketCore::Type::WATER_TYPE_MATCHUP;

	/*! @class TypeRegistry Registry/typeRegistry.h
		@brief A constexpr registry storing Pokemon types with their effectiveness chart in fixed-capacity arrays.
		@details Stores up to @ref MAX_TYPES entries in fixed-size arrays with no dynamic allocation. Built-in @ref Types enum values are
	   pre-registered by the constructor. Custom type registration and removal are handled externally by
	   @ref PocketCore::Configuration::Configuration, which mutates the registry through its public getters and setters.
		@note All lookup operations are O(n) where n is the number of registered types due to linear search over a fixed-size array. This is
	   acceptable because n is bounded by @ref MAX_TYPES.
		@date 09/03/2026
		@since 0.1.0
		@version 0.12.19
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
				@version 0.12.19
			*/
			ATTR_NOINLINE explicit constexpr TypeRegistry() : Base{toTypeID(BuiltinTypeID::FinalType).getValue()}
			{
				// LCOV_EXCL_BR_START - Built-in types are guaranteed to be registered, so branch coverage for the addBuiltin precondition
				// is not applicable.
				addBuiltin({
					.mName = TYPE_NAME_NONE,
					.mTypeID = toTypeID(BuiltinTypeID::None),
				});
				addBuiltin({
					.mOffensiveMatchups = NORMAL_TYPE_MATCHUP,
					.mName = TYPE_NAME_NORMAL,
					.mTypeID = toTypeID(BuiltinTypeID::Normal),
				});
				addBuiltin({
					.mOffensiveMatchups = FIGHTING_TYPE_MATCHUP,
					.mName = TYPE_NAME_FIGHTING,
					.mTypeID = toTypeID(BuiltinTypeID::Fighting),
				});
				addBuiltin({
					.mOffensiveMatchups = FLYING_TYPE_MATCHUP,
					.mName = TYPE_NAME_FLYING,
					.mTypeID = toTypeID(BuiltinTypeID::Flying),
				});
				addBuiltin({
					.mOffensiveMatchups = POISON_TYPE_MATCHUP,
					.mName = TYPE_NAME_POISON,
					.mTypeID = toTypeID(BuiltinTypeID::Poison),
				});
				addBuiltin({
					.mOffensiveMatchups = GROUND_TYPE_MATCHUP,
					.mName = TYPE_NAME_GROUND,
					.mTypeID = toTypeID(BuiltinTypeID::Ground),
				});
				addBuiltin({
					.mOffensiveMatchups = ROCK_TYPE_MATCHUP,
					.mName = TYPE_NAME_ROCK,
					.mTypeID = toTypeID(BuiltinTypeID::Rock),
				});
				addBuiltin({
					.mOffensiveMatchups = BUG_TYPE_MATCHUP,
					.mName = TYPE_NAME_BUG,
					.mTypeID = toTypeID(BuiltinTypeID::Bug),
				});
				addBuiltin({
					.mOffensiveMatchups = GHOST_TYPE_MATCHUP,
					.mName = TYPE_NAME_GHOST,
					.mTypeID = toTypeID(BuiltinTypeID::Ghost),
				});
				addBuiltin({
					.mOffensiveMatchups = STEEL_TYPE_MATCHUP,
					.mName = TYPE_NAME_STEEL,
					.mTypeID = toTypeID(BuiltinTypeID::Steel),
				});
				addBuiltin({
					.mOffensiveMatchups = FIRE_TYPE_MATCHUP,
					.mName = TYPE_NAME_FIRE,
					.mTypeID = toTypeID(BuiltinTypeID::Fire),
				});
				addBuiltin({
					.mOffensiveMatchups = WATER_TYPE_MATCHUP,
					.mName = TYPE_NAME_WATER,
					.mTypeID = toTypeID(BuiltinTypeID::Water),
				});
				addBuiltin({
					.mOffensiveMatchups = GRASS_TYPE_MATCHUP,
					.mName = TYPE_NAME_GRASS,
					.mTypeID = toTypeID(BuiltinTypeID::Grass),
				});
				addBuiltin({
					.mOffensiveMatchups = ELECTRIC_TYPE_MATCHUP,
					.mName = TYPE_NAME_ELECTRIC,
					.mTypeID = toTypeID(BuiltinTypeID::Electric),
				});
				addBuiltin({
					.mOffensiveMatchups = PSYCHIC_TYPE_MATCHUP,
					.mName = TYPE_NAME_PSYCHIC,
					.mTypeID = toTypeID(BuiltinTypeID::Psychic),
				});
				addBuiltin({
					.mOffensiveMatchups = ICE_TYPE_MATCHUP,
					.mName = TYPE_NAME_ICE,
					.mTypeID = toTypeID(BuiltinTypeID::Ice),
				});
				addBuiltin({
					.mOffensiveMatchups = DRAGON_TYPE_MATCHUP,
					.mName = TYPE_NAME_DRAGON,
					.mTypeID = toTypeID(BuiltinTypeID::Dragon),
				});
				addBuiltin({
					.mOffensiveMatchups = DARK_TYPE_MATCHUP,
					.mName = TYPE_NAME_DARK,
					.mTypeID = toTypeID(BuiltinTypeID::Dark),
				});
				addBuiltin({
					.mOffensiveMatchups = FAIRY_TYPE_MATCHUP,
					.mName = TYPE_NAME_FAIRY,
					.mTypeID = toTypeID(BuiltinTypeID::Fairy),
				});
				addBuiltin({.mName = TYPE_NAME_STELLAR, .mTypeID = toTypeID(BuiltinTypeID::Stellar)});
				// LCOV_EXCL_BR_STOP
			}

		protected:
			using Base::addEntry;
			using Base::Checkpoint;
			using Base::createCheckpoint;
			using Base::decrementAmountRegistered;
			using Base::eraseEntry;
			using Base::getMutableEntry;
			using Base::incrementAmountRegistered;
			using Base::restoreCheckpoint;
			using Base::setAmountRegistered;
			using Base::setEntry;

		protected:
			/*! @brief Sets a single cell in the type chart.
				@pre @p attacker < @ref MAX_TYPES and @p col < @ref MAX_TYPES.
				@param[in] attacker The attacking-type index.
				@param[in] defender The defending-type index.
				@param[in] value The @ref TypeEffectiveness value to store.
				@since 0.1.0
				@version 0.12.19
			*/
			constexpr void setTypeChartCell(const us attacker, const us defender, const TypeEffectiveness value)
			{
				assert(attacker < MAX_TYPES && ROW_OOB_SET_TYPE_CHART_CELL.data());
				assert(defender < MAX_TYPES && COL_OOB_SET_TYPE_CHART_CELL.data());

				getMutableEntry(attacker).mOffensiveMatchups.at(defender) = value;
			}

			/*! @brief Sets a single type-chart cell using stable type IDs.
				@details Resolves both IDs to internal registry indices before updating the corresponding chart cell. The operation is
			   ignored when either type is not registered.
				@param[in] attacker The stable type identifier for the attacking type.
				@param[in] defender The stable type identifier for the defending type.
				@param[in] value The @ref TypeEffectiveness value to store.
				@since 0.12.19
				@version 0.12.19
			*/
			constexpr void setTypeChartCell(const TypeID attacker, const TypeID defender, const TypeEffectiveness value)
			{
				const std::optional<us> attackerIndex{findIndexByID(attacker)};
				const std::optional<us> defenderIndex{findIndexByID(defender)};

				if (!attackerIndex.has_value() || !defenderIndex.has_value())
				{
					return;
				}

				getMutableEntry(attackerIndex.value()).mOffensiveMatchups.at(defenderIndex.value()) = value;
			}

			/*! @brief Replaces an entire row in the type chart.
				@pre @p attacker < @ref MAX_TYPES.
				@param[in] attacker The row index.
				@param[in] chart The full row of @ref TypeEffectiveness values to assign.
				@since 0.1.0
				@version 0.12.19
			*/
			constexpr void setTypeChartRow(const us attacker, const std::array<TypeEffectiveness, MAX_TYPES> &chart)
			{
				assert(attacker < MAX_TYPES && ROW_OOB_SET_TYPE_CHART_ROW.data());

				if (attacker >= MAX_TYPES)
				{
					return;
				}

				getMutableEntry(attacker).mOffensiveMatchups = chart;
			}

			/*! @brief Replaces an entire type-chart row using a stable type ID.
				@details Resolves the attacker ID to its internal registry index before replacing the row. The operation is ignored when the
				attacker is not registered.
				@param[in] attacker The stable type identifier for the attacking type.
				@param[in] chart The complete row of @ref TypeEffectiveness values to assign.
				@since 0.12.19
				@version 0.12.19
			*/
			constexpr void setTypeChartRow(const TypeID attacker, const std::array<TypeEffectiveness, MAX_TYPES> &chart)
			{
				const std::optional<us> attackerIndex{findIndexByID(attacker)};

				if (!attackerIndex.has_value())
				{
					return;
				}

				getMutableEntry(attackerIndex.value()).mOffensiveMatchups = chart;
			}

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

		public:
			/*! @brief Looks up type metadata by stable ID.
				@param[in] typeID The stable type identifier.
				@return A non-owning pointer to metadata if registered, or nullptr otherwise. The pointer remains valid until replacement or
			   registry destruction.
				@since 0.12.19
				@version 0.12.19
			*/
			ATTR_NODISCARD constexpr const TypeMeta *getTypeMetadata(const TypeID typeID) const
			{
				return getMetadata(typeID);
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
				@version 0.12.19
			*/
			ATTR_NODISCARD constexpr us getNextTypeID() const noexcept
			{
				return getNextID();
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

			/*! @brief Returns a single cell from the type chart.
				@pre @p attacker < @ref MAX_TYPES and @p col < @ref MAX_TYPES.
				@param[in] attacker The attacking-type index.
				@param[in] defender The defending-type index.
				@return The @ref TypeEffectiveness value at that cell.
				@since 0.1.0
				@version 0.12.19
			*/
			ATTR_NODISCARD constexpr TypeEffectiveness getTypeChartCell(const us attacker, const us defender) const
			{
				const TypeMeta *metadata{getEntry(attacker)};
				const std::optional<us> defenderIndex{findIndexByID(TypeID{defender})};

				if (metadata == nullptr || !defenderIndex.has_value())
				{
					return TypeEffectiveness::NOT_DEFINED;
				}

				return metadata->mOffensiveMatchups.at(defender);
			}

			/*! @brief Looks up a single type-chart cell by stable type ID.
				@param[in] attacker The stable type identifier for the attacking type.
				@param[in] defender The stable type identifier for the defending type.
				@return A non-owning pointer to the effectiveness value, or nullptr when the attacker is not registered or the defender ID
			   is out of range. The pointer remains valid until the corresponding metadata is replaced or the registry is destroyed.
				@since 0.12.19
				@version 0.12.19
			*/
			ATTR_NODISCARD constexpr const TypeEffectiveness *getTypeChartCell(const TypeID attacker, const TypeID defender) const
			{
				const TypeMeta *metadata{getMetadata(attacker)};
				const std::optional<us> defenderIndex{findIndexByID(defender)};

				if (metadata == nullptr || !defenderIndex.has_value())
				{
					return nullptr;
				}

				return &metadata->mOffensiveMatchups.at(defenderIndex.value());
			}

			/*! @brief Returns an entire row from the type chart.
				@pre @p row < @ref MAX_TYPES.
				@param[in] row The row index.
				@return A read-only reference to the full effectiveness row for the given type, or to an all-@ref NOT_DEFINED fallback row
			   when the index is invalid.
				@since 0.1.0
				@version 0.12.19
			*/
			ATTR_NODISCARD constexpr const std::array<TypeEffectiveness, MAX_TYPES> &getTypeChartRow(const us row) const
			{
				const TypeMeta *metadata{getEntry(row)};

				if (metadata == nullptr)
				{
					return INVALID_TYPE_CHART_ROW;
				}

				return metadata->mOffensiveMatchups;
			}

			/*! @brief Looks up an entire type-chart row by stable type ID.
				@param[in] typeID The stable type identifier for the attacking type.
				@return A non-owning pointer to the effectiveness row, or nullptr when the type ID is not registered. The pointer remains
			   valid until the corresponding metadata is replaced or the registry is destroyed.
				@since 0.12.19
				@version 0.12.19
			*/
			ATTR_NODISCARD constexpr const std::array<TypeEffectiveness, MAX_TYPES> *getTypeChartRow(const TypeID typeID) const
			{
				const TypeMeta *metadata{getMetadata(typeID)};

				if (metadata == nullptr)
				{
					return nullptr;
				}

				return &metadata->mOffensiveMatchups;
			}
	};
} // namespace PocketCore::Registry::Type

#endif
