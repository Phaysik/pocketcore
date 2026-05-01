/*! @file typeRegistryConfiguration.h
	@brief Contains the function declarations for the user-configurable type registry facade.
	@date 03/10/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_CONFIGURATION_TYPE_REGISTRY_CONFIGURATION_H
#define INCLUDE_CONFIGURATION_TYPE_REGISTRY_CONFIGURATION_H

#include <array>
#include <expected>
#include <optional>
#include <span>
#include <string_view>

#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Registry/typeRegistry.h"
#include "Types/typeEffectiveness.h"

namespace Pokemon::Configuration
{
	using Pokemon::Core::ub;
	using Pokemon::Types::TypeEffectiveness;

	// MARK: Helper Structs

	/*! @struct MatchupPair Configuration/typeRegistryConfiguration.h
		@brief A name-keyed effectiveness entry for specifying type matchups by display name rather than positional index.
		@details Used with the name-keyed overloads of @ref Configuration::addType and @ref Configuration::setMatchupRow to provide
	   order-independent matchup specification.
	*/
	struct MatchupPair
	{
			/*! @brief The display name of the target type. */
			std::string_view typeName{};

			/*! @brief The effectiveness multiplier for this matchup. */
			TypeEffectiveness value;
	};

	/*! @struct TypeDefinition Configuration/typeRegistryConfiguration.h
		@brief A self-contained description of a new type for name-keyed batch registration.
		@details Groups the type's display name with its offensive and defensive matchup pairs so that
		@ref Configuration::addTypes can accept a single span of definitions instead of three parallel spans.
	*/
	struct TypeDefinition
	{
			/*! @brief The display name for the new type. */
			std::string_view name{};

			/*! @brief Name-keyed pairs describing how the new type attacks each existing type and itself. */
			std::span<const MatchupPair> offensiveMatchups{};

			/*! @brief Name-keyed pairs describing how each existing type attacks the new type. */
			std::span<const MatchupPair> defensiveMatchups{};
	};

	// MARK: TypeRegistryConfiguration

	/*! @class TypeRegistryConfiguration Configuration/typeRegistryConfiguration.h
		@brief Facade for managing a dynamic type registry and its associated effectiveness chart.
		@details Provides a high-level API for adding, removing, renaming, and querying types and their matchup relationships. All mutation
	   methods return @ref std::expected to communicate success or structured error information via @ref RegistryErrorInfo. Batch operations
	   provide all-or-nothing (atomic rollback) semantics.
	*/
	class TypeRegistryConfiguration
	{
		public:
			// MARK: Constructor

			/*! @brief Default-constructs a @ref TypeRegistryConfiguration with an empty registry. */
			constexpr TypeRegistryConfiguration() = default;

			// MARK: Getters

			/*! @brief Returns the effectiveness of one type attacking another, looked up by display name.
				@details Resolves both attacker and defender names to their internal array indices and reads the corresponding cell from the
			   type chart.
				@param[in] attackerName The display name of the attacking type.
				@param[in] defenderName The display name of the defending type.
				@return The @ref Pokemon::Types::TypeEffectiveness value on success, or @ref RegistryErrorInfo if either type is not found.
			*/
			ATTR_NODISCARD std::expected<TypeEffectiveness, RegistryErrorInfo> getMatchup(std::string_view attackerName,
																						  std::string_view defenderName);

			/*! @brief Returns the full offensive matchup row for a type identified by display name.
				@details Resolves the attacker name to its internal array index and returns a copy of the entire effectiveness row.
				@param[in] attackerName The display name of the attacking type.
				@return A copy of the full @ref Pokemon::Types::TypeEffectiveness row on success, or @ref RegistryErrorInfo if the type is
			   not found.
			*/
			ATTR_NODISCARD std::expected<std::array<TypeEffectiveness, MAX_TYPES>, RegistryErrorInfo> getMatchupRow(
				std::string_view attackerName);

			/*! @brief Returns the full defensive matchup column for a type identified by display name.
				@details Resolves the defender name to its internal array index and reads every attacker's effectiveness against it.
				@param[in] defenderName The display name of the defending type.
				@return A copy of the full @ref Pokemon::Types::TypeEffectiveness column on success, or @ref RegistryErrorInfo if the type
			   is not found.
			*/
			ATTR_NODISCARD std::expected<std::array<TypeEffectiveness, MAX_TYPES>, RegistryErrorInfo> getDefensiveColumn(
				std::string_view defenderName);

			/*! @brief Looks up a type ID by its display name.
				@param[in] name The display name to search for.
				@return The type ID wrapped in std::optional if found, or std::nullopt if no type with that name is registered.
			*/
			ATTR_NODISCARD constexpr std::optional<ub> getTypeID(const std::string_view name) const
			{
				return registry.getTypeID(name);
			}

			/*! @brief Looks up a type's display name by its ID.
				@param[in] typeID The type ID to search for.
				@return The name wrapped in std::optional if found, or std::nullopt if no type with that ID is registered.
			*/
			ATTR_NODISCARD constexpr std::optional<std::string_view> getTypeName(const ub typeID) const
			{
				return registry.getTypeName(typeID);
			}

			/*! @brief Returns a read-only span over all currently registered type entries.
				@return A span of @ref Registry::Types::TypeEntry covering all valid registered entries.
			*/
			ATTR_NODISCARD constexpr std::span<const Registry::Types::TypeEntry> getRegisteredTypes() const
			{
				return registry.getRegisteredTypes();
			}

			/*! @brief Returns the total number of registered types (built-in + custom).
				@return The count of registered types.
			*/
			ATTR_NODISCARD constexpr ub getAmountRegistered() const noexcept
			{
				return registry.getAmountRegistered();
			}

			// MARK: Setters

			/*! @brief Modifies a single cell in the type chart by attacker and defender name.
				@details Looks up both type names, resolves their array indices, and writes the new effectiveness value into the chart.
				@param[in] attackerName The display name of the attacking type.
				@param[in] defenderName The display name of the defending type.
				@param[in] value The @ref Pokemon::Types::TypeEffectiveness value to assign.
				@return std::expected<void, @ref RegistryErrorInfo> containing the error if either type is not found, or void on success.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> setMatchup(std::string_view attackerName, std::string_view defenderName,
																			 TypeEffectiveness value);

			/*! @brief Replaces the entire offensive matchup row for a type identified by name.
				@details Looks up the attacker by name, resolves its array index, and overwrites every column in that row. The caller only
			   needs to provide entries for the currently registered types. Any indices beyond the span's size are set to @ref
			   Pokemon::Types::TypeEffectiveness::NOT_DEFINED.
				@pre @p newRow.size() <= @ref MAX_TYPES.
				@param[in] attackerName The display name of the attacking type whose row will be replaced.
				@param[in] newRow A span of @ref Pokemon::Types::TypeEffectiveness values for the new offensive matchup row.
				@return std::expected<void, @ref RegistryErrorInfo> containing the error if the type is not found or the span is too large,
			   or void on success.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> setMatchupRow(std::string_view attackerName,
																				std::span<const TypeEffectiveness> newRow);

			/*! @brief Replaces the entire offensive matchup row for a type using name-keyed pairs.
				@details Resolves each @ref MatchupPair by looking up the referenced type name in the registry. Types not mentioned default
			   to @ref Pokemon::Types::TypeEffectiveness::NOT_DEFINED. This allows the caller to specify matchups in any order.
				@param[in] attackerName The display name of the attacking type whose row will be replaced.
				@param[in] newRow A span of @ref MatchupPair values for the new offensive matchup row.
				@return std::expected<void, @ref RegistryErrorInfo> containing the error if the attacker or any referenced type is not
			   found, or void on success.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> setMatchupRow(std::string_view attackerName,
																				std::span<const MatchupPair> newRow);

			/*! @brief Replaces the entire defensive matchup column for a type identified by name.
				@details Looks up the defender by name, resolves its array index, and overwrites every row's cell in that column. The caller
			   only needs to provide entries for the currently registered types. Any indices beyond the span's size are set to @ref
			   Pokemon::Types::TypeEffectiveness::NOT_DEFINED.
				@pre @p newCol.size() <= @ref MAX_TYPES.
				@param[in] defenderName The display name of the defending type whose column will be replaced.
				@param[in] newCol A span of @ref Pokemon::Types::TypeEffectiveness values for the new defensive matchup column.
				@return std::expected<void, @ref RegistryErrorInfo> containing the error if the type is not found or the span is too large,
			   or void on success.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> setDefensiveColumn(std::string_view defenderName,
																					 std::span<const TypeEffectiveness> newCol);

			/*! @brief Replaces the entire defensive matchup column for a type using name-keyed pairs.
				@details Resolves each @ref MatchupPair by looking up the referenced attacker type name in the registry. Types not mentioned
			   default to @ref Pokemon::Types::TypeEffectiveness::NOT_DEFINED. This allows the caller to specify matchups in any order.
				@param[in] defenderName The display name of the defending type whose column will be replaced.
				@param[in] newCol A span of @ref MatchupPair values for the new defensive matchup column.
				@return std::expected<void, @ref RegistryErrorInfo> containing the error if the defender or any referenced type is not
			   found, or void on success.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> setDefensiveColumn(std::string_view defenderName,
																					 std::span<const MatchupPair> newCol);

			// MARK: Member Functions

			/*! @brief Adds a new custom type using name-keyed matchup pairs.
				@details Resolves each @ref MatchupPair by looking up the referenced type name in the registry to determine its array index.
			   This allows the caller to specify matchups in any order without knowing the internal registration sequence. The @p
			   defaultBehavior parameter controls what happens to matchups not explicitly mentioned: @ref UnspecifiedMatchup::NotDefined
			   fails the call,
			   @ref UnspecifiedMatchup::Neutral fills with @ref Pokemon::Types::TypeEffectiveness::E, and
			   @ref UnspecifiedMatchup::NotDefined fills with @ref Pokemon::Types::TypeEffectiveness::NOT_DEFINED.
				@param[in] definition A @ref TypeDefinition struct containing the display name and matchup pair spans for the new type.
				@param[in] defaultBehavior Controls how unspecified matchups are handled (defaults to @ref UnspecifiedMatchup::NotDefined).
				@return The stable type ID assigned to the new type on success, or @ref RegistryErrorInfo on failure.
			*/
			ATTR_NODISCARD std::expected<ub, RegistryErrorInfo> addType(const TypeDefinition &definition, UnspecifiedMatchup defaultBehavior
																										  = UnspecifiedMatchup::NotDefined);

			/*! @brief Adds multiple new custom types using self-contained name-keyed definitions.
				@details Registers each @ref TypeDefinition sequentially by delegating to the name-keyed @ref addType overload.
			   If any registration fails the entire batch is rolled back.
				@param[in] definitions A span of @ref TypeDefinition structs, each containing a name and matchup pair spans.
				@param[in] defaultBehavior Controls how unspecified matchups are handled for every definition in the batch.
				@return std::expected<void, @ref RegistryErrorInfo> containing the error on failure, or void on success.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> addTypes(const std::span<const TypeDefinition> &definitions,
																		   UnspecifiedMatchup defaultBehavior
																		   = UnspecifiedMatchup::NotDefined);

			/*! @brief Removes a type from the registry by name.
				@param[in] typeName The display name of the type to remove.
				@return The stable type ID of the removed type on success, or @ref RegistryErrorInfo if the type is not found.
			*/
			ATTR_NODISCARD std::expected<ub, RegistryErrorInfo> removeType(std::string_view typeName);

			/*! @brief Removes a type from the registry by its enum value.
				@param[in] type The built-in @ref Pokemon::Types::Types enum value to remove.
				@return The stable type ID of the removed type on success, or @ref RegistryErrorInfo if the type is not found.
			*/
			ATTR_NODISCARD std::expected<ub, RegistryErrorInfo> removeType(Pokemon::Types::Types type);

			/*! @brief Removes a type from the registry by its stable type ID.
				@details Useful for removing custom types using the ID returned by @ref addType.
				@param[in] typeID The stable type ID of the type to remove.
				@return The stable type ID of the removed type on success, or @ref RegistryErrorInfo if the type is not found.
			*/
			ATTR_NODISCARD std::expected<ub, RegistryErrorInfo> removeType(ub typeID);
			/*! @brief Removes multiple types from the registry by name with atomic rollback.
				@details Resolves and removes each named type sequentially. If any removal fails, the entire registry is restored
			   to its state before the batch began, guaranteeing all-or-nothing semantics.
				@param[in] typeNames A span of display names identifying the types to remove.
				@return std::expected<void, @ref RegistryErrorInfo> containing the error on failure, or void on success.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> removeTypes(std::span<const std::string_view> typeNames);
			/*! @brief Renames an existing type in the registry.
				@details Looks up @p oldName, checks that @p newName is not already taken, and overwrites the entry's display name.
			   The stable type ID, array position, and all matchup data remain unchanged.
				@param[in] oldName The current display name of the type to rename.
				@param[in] newName The new display name to assign.
				@return std::expected<void, @ref RegistryErrorInfo> containing the error if @p oldName is not found or @p newName is a
			   duplicate, or void on success.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> renameType(std::string_view oldName, std::string_view newName);

			/*! @brief Resets all matchup data for a type identified by display name.
				@details Clears the type's entire offensive row and defensive column to @ref Pokemon::Types::TypeEffectiveness::NOT_DEFINED
			   without removing the type from the registry. The type's entry, stable ID, and array position are preserved.
				@param[in] typeName The display name of the type whose matchups will be cleared.
				@return std::expected<void, @ref RegistryErrorInfo> containing the error if the type is not found, or void on success.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> resetMatchups(std::string_view typeName);

			/*! @brief Resets all matchup data for a type identified by its stable type ID.
				@details Clears the type's entire offensive row and defensive column to @ref Pokemon::Types::TypeEffectiveness::NOT_DEFINED
			   without removing the type from the registry. The type's entry, stable ID, and array position are preserved.
				@param[in] typeID The stable type ID of the type whose matchups will be cleared.
				@return std::expected<void, @ref RegistryErrorInfo> containing the error if the type is not found, or void on success.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> resetMatchups(ub typeID);

			/*! @brief Checks whether a type with the given name is registered.
				@param[in] name The display name to check.
				@return True if a type with that name exists in the registry, false otherwise.
			*/
			ATTR_NODISCARD constexpr bool hasType(const std::string_view name) const
			{
				return registry.hasType(name);
			}

			/*! @brief Checks whether a type with the given ID is registered.
				@param[in] typeID The type ID to check.
				@return True if a type with that ID exists in the registry, false otherwise.
			*/
			ATTR_NODISCARD constexpr bool hasType(const ub typeID) const
			{
				return registry.hasType(typeID);
			}

		private:
			// MARK: Private Member Functions

			/*! @brief Restores the registry to a prior state by discarding entries added after @p previousCount.
				@details Used internally to implement atomic rollback for batch operations. Resets the registry's entry count and
			   next-type-ID counter to the values captured before the batch began.
				@param[in] previousCount The number of registered entries to revert to.
				@param[in] previousNextTypeID The next-type-ID value to restore.
			*/
			void rollbackEntries(ub previousCount, ub previousNextTypeID);

			/*! @brief Removes a single entry from the registry by its internal array index.
				@details Shifts subsequent entries down and clears the corresponding matchup row and column data.
				@param[in] arrayIndex The zero-based position of the entry to remove.
			*/
			void removeEntry(ub arrayIndex);

			/*! @brief Resolves a type name to its internal array index, returning a @ref RegistryErrorInfo on failure.
				@param[in] name The display name of the type.
				@param[in] callerContext A string identifying the calling function for error messages.
				@return The array index on success, or @ref RegistryErrorInfo if the type is not found.
			*/
			ATTR_NODISCARD std::expected<ub, RegistryErrorInfo> resolveIndex(std::string_view name, std::string_view callerContext);

		private:
			/*! @brief The internal type registry storing all type entries and matchup data. */
			Registry::Types::TypeRegistry registry{};
	};
} // namespace Pokemon::Configuration

#endif