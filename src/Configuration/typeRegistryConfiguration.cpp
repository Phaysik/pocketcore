/*! @file typeRegistryConfiguration.cpp
	@brief Contains the function definitions for creating a type registry configuration
	@date 03/11/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#include "Configuration/typeRegistryConfiguration.h"

#include <array>
#include <cstddef>
#include <expected>
#include <optional>
#include <span>
#include <string_view>
#include <unordered_set>

#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Registry/typeRegistry.h"
#include "Types/typeEffectiveness.h"
#include "Types/types.h"
#include "Utility/Debug/Logging/logger.h"

namespace Pokemon::Configuration
{
	using Pokemon::Configuration::MatchupPair;
	using Pokemon::Configuration::MAX_TYPES;
	using Pokemon::Core::ub;
	using Pokemon::Registry::Types::TypeEntry;
	using Pokemon::Types::TypeEffectiveness;
	using Pokemon::Utility::Debug::Logging::Logger;

	// MARK: Getters

	ATTR_NODISCARD std::expected<TypeEffectiveness, RegistryErrorInfo> TypeRegistryConfiguration::getMatchup(
		const std::string_view attackerName, const std::string_view defenderName)
	{
		const std::expected<ub, RegistryErrorInfo> attackerIndex{resolveIndex(attackerName, "getMatchup")}; // LCOV_EXCL_BR

		if (!attackerIndex.has_value())
		{
			return std::unexpected{attackerIndex.error()};
		}

		const std::expected<ub, RegistryErrorInfo> defenderIndex{resolveIndex(defenderName, "getMatchup")}; // LCOV_EXCL_BR

		if (!defenderIndex.has_value())
		{
			return std::unexpected{defenderIndex.error()};
		}

		return registry.getTypeChartCell(attackerIndex.value(), defenderIndex.value());
	}

	ATTR_NODISCARD std::expected<std::array<TypeEffectiveness, MAX_TYPES>, RegistryErrorInfo> TypeRegistryConfiguration::getMatchupRow(
		const std::string_view attackerName)
	{
		const std::expected<ub, RegistryErrorInfo> attackerIndex{resolveIndex(attackerName, "getMatchupRow")}; // LCOV_EXCL_BR

		if (!attackerIndex.has_value())
		{
			return std::unexpected{attackerIndex.error()};
		}

		return registry.getTypeChartRow(attackerIndex.value());
	}

	ATTR_NODISCARD std::expected<std::array<TypeEffectiveness, MAX_TYPES>, RegistryErrorInfo> TypeRegistryConfiguration::getDefensiveColumn(
		const std::string_view defenderName)
	{
		const std::expected<ub, RegistryErrorInfo> defenderIndex{resolveIndex(defenderName, "getDefensiveColumn")}; // LCOV_EXCL_BR

		if (!defenderIndex.has_value())
		{
			return std::unexpected{defenderIndex.error()};
		}

		std::array<TypeEffectiveness, MAX_TYPES> column{};
		const ub registered{registry.getAmountRegistered()};

		for (ub row{0}; row < registered; ++row)
		{
			column.at(row) = registry.getTypeChartCell(row, defenderIndex.value());
		}

		return column;
	}

	// MARK: Setters

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> TypeRegistryConfiguration::setMatchup(const std::string_view attackerName,
																								const std::string_view defenderName,
																								const TypeEffectiveness value)
	{
		const std::expected<ub, RegistryErrorInfo> attackerIndex{resolveIndex(attackerName, "setMatchup")}; // LCOV_EXCL_BR

		if (!attackerIndex.has_value())
		{
			return std::unexpected{attackerIndex.error()};
		}

		const std::expected<ub, RegistryErrorInfo> defenderIndex{resolveIndex(defenderName, "setMatchup")}; // LCOV_EXCL_BR

		if (!defenderIndex.has_value())
		{
			return std::unexpected{defenderIndex.error()};
		}

		registry.setTypeChartCell(attackerIndex.value(), defenderIndex.value(), value);

		return {};
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> TypeRegistryConfiguration::setMatchupRow(
		const std::string_view attackerName, const std::span<const TypeEffectiveness> newRow)
	{
		if (newRow.size() > MAX_TYPES)
		{
			const std::optional<std::string_view> logResult{Logger::warn(
				"TypeRegistryConfiguration::setMatchupRow row span size ({}) exceeds MAX_TYPES ({}).", newRow.size(), MAX_TYPES)};

			return std::unexpected{RegistryErrorInfo{RegistryError::MatchupMismatch, attackerName, logResult.value_or(std::string_view{})}};
		}

		const std::expected<ub, RegistryErrorInfo> attackerIndex{resolveIndex(attackerName, "setMatchupRow")}; // LCOV_EXCL_BR

		if (!attackerIndex.has_value())
		{
			return std::unexpected{attackerIndex.error()};
		}

		const ub registered{registry.getAmountRegistered()};

		for (ub col{0}; col < registered; ++col)
		{
			registry.setTypeChartCell(attackerIndex.value(), col, col < newRow.size() ? newRow.at(col) : TypeEffectiveness::NOT_DEFINED);
		}

		return {};
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> TypeRegistryConfiguration::setMatchupRow(
		const std::string_view attackerName, const std::span<const MatchupPair> newRow)
	{
		const std::expected<ub, RegistryErrorInfo> attackerIndex{resolveIndex(attackerName, "setMatchupRow")}; // LCOV_EXCL_BR

		if (!attackerIndex.has_value())
		{
			return std::unexpected{attackerIndex.error()};
		}

		const ub registered{registry.getAmountRegistered()};

		// Reset the entire row to NOT_DEFINED, then fill from pairs
		for (ub col{0}; col < registered; ++col)
		{
			registry.setTypeChartCell(attackerIndex.value(), col, TypeEffectiveness::NOT_DEFINED);
		}

		for (const auto &[pairName, pairValue] : newRow)
		{
			const std::expected<ub, RegistryErrorInfo> targetIndex{resolveIndex(pairName, "setMatchupRow")}; // LCOV_EXCL_BR

			if (!targetIndex.has_value())
			{
				return std::unexpected{targetIndex.error()};
			}

			registry.setTypeChartCell(attackerIndex.value(), targetIndex.value(), pairValue);
		}

		return {};
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> TypeRegistryConfiguration::setDefensiveColumn(
		const std::string_view defenderName, const std::span<const TypeEffectiveness> newCol)
	{
		if (newCol.size() > MAX_TYPES)
		{
			const std::optional<std::string_view> logResult{Logger::warn(
				"TypeRegistryConfiguration::setDefensiveColumn column span size ({}) exceeds MAX_TYPES ({}).", newCol.size(), MAX_TYPES)};

			return std::unexpected{RegistryErrorInfo{RegistryError::MatchupMismatch, defenderName, logResult.value_or(std::string_view{})}};
		}

		const std::expected<ub, RegistryErrorInfo> defenderIndex{resolveIndex(defenderName, "setDefensiveColumn")}; // LCOV_EXCL_BR

		if (!defenderIndex.has_value())
		{
			return std::unexpected{defenderIndex.error()};
		}

		const ub registered{registry.getAmountRegistered()};

		for (ub row{0}; row < registered; ++row)
		{
			registry.setTypeChartCell(row, defenderIndex.value(), row < newCol.size() ? newCol.at(row) : TypeEffectiveness::NOT_DEFINED);
		}

		return {};
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> TypeRegistryConfiguration::setDefensiveColumn(
		const std::string_view defenderName, const std::span<const MatchupPair> newCol)
	{
		const std::expected<ub, RegistryErrorInfo> defenderIndex{resolveIndex(defenderName, "setDefensiveColumn")}; // LCOV_EXCL_BR

		if (!defenderIndex.has_value())
		{
			return std::unexpected{defenderIndex.error()};
		}

		const ub registered{registry.getAmountRegistered()};

		// Reset the entire column to NOT_DEFINED, then fill from pairs
		for (ub row{0}; row < registered; ++row)
		{
			registry.setTypeChartCell(row, defenderIndex.value(), TypeEffectiveness::NOT_DEFINED);
		}

		for (const auto &[pairName, pairValue] : newCol)
		{
			const std::expected<ub, RegistryErrorInfo> attackerIndex{resolveIndex(pairName, "setDefensiveColumn")}; // LCOV_EXCL_BR

			if (!attackerIndex.has_value())
			{
				return std::unexpected{attackerIndex.error()};
			}

			registry.setTypeChartCell(attackerIndex.value(), defenderIndex.value(), pairValue);
		}

		return {};
	}

	// MARK: Member Functions

	ATTR_NODISCARD std::expected<ub, RegistryErrorInfo> TypeRegistryConfiguration::addType(const TypeDefinition &definition,
																						   const UnspecifiedMatchup defaultBehavior)
	{
		const ub registered{registry.getAmountRegistered()};
		const std::string_view typeName{definition.name};

		if (registered >= MAX_TYPES)
		{
			const std::optional<std::string_view> logResult{
				Logger::warn("TypeRegistryConfiguration: registry capacity of {} reached. Cannot add type '{}'.", MAX_TYPES, typeName)};

			return std::unexpected{RegistryErrorInfo{RegistryError::MaxCapacity, typeName, logResult.value_or(std::string_view{})}};
		}

		if (registry.hasType(typeName))
		{
			const std::optional<std::string_view> logResult{
				Logger::warn("TypeRegistryConfiguration::addType duplicate type found. Cannot add type '{}'.", typeName)};

			return std::unexpected{RegistryErrorInfo{RegistryError::DuplicateType, typeName, logResult.value_or(std::string_view{})}};
		}

		// Determine the fill value for unspecified matchup slots
		TypeEffectiveness fillValue{};

		if (defaultBehavior == UnspecifiedMatchup::Neutral)
		{
			fillValue = TypeEffectiveness::E;
		}
		else if (defaultBehavior == UnspecifiedMatchup::NotDefined)
		{
			fillValue = TypeEffectiveness::NOT_DEFINED;
		}

		// Build positional arrays from name-keyed pairs
		std::array<TypeEffectiveness, MAX_TYPES> offensiveRow{};
		offensiveRow.fill(fillValue);

		for (const auto &[pairName, pairValue] : definition.offensiveMatchups)
		{
			if (pairName == typeName)
			{
				// Self-matchup: will occupy the slot at index 'registered'
				offensiveRow.at(registered) = pairValue;
				continue;
			}

			const std::optional<ub> targetId{registry.getTypeID(pairName)}; // LCOV_EXCL_BR

			if (!targetId.has_value())
			{
				const std::optional<std::string_view> logResult{
					Logger::warn("TypeRegistryConfiguration::addType offensive matchup references unknown type '{}'. Cannot add type '{}'.",
								 pairName, typeName)};

				return std::unexpected{RegistryErrorInfo{RegistryError::TypeNotFound, pairName, logResult.value_or(std::string_view{})}};
			}

			const std::optional<ub> targetIndex{registry.findIndexByTypeID(targetId.value())}; // LCOV_EXCL_BR

			// LCOV_EXCL_START — Defensive: findIndexByTypeID cannot fail when getTypeID just succeeded on the same registry.
			if (!targetIndex.has_value())
			{
				const std::optional<std::string_view> logResult{
					Logger::warn("TypeRegistryConfiguration::addType internal error: type ID found but array index missing for '{}'. "
								 "Cannot add type '{}'.",
								 pairName, typeName)};

				return std::unexpected{RegistryErrorInfo{RegistryError::TypeNotFound, pairName, logResult.value_or(std::string_view{})}};
			}
			// LCOV_EXCL_STOP

			offensiveRow.at(targetIndex.value()) = pairValue;
		}

		std::array<TypeEffectiveness, MAX_TYPES> defensiveRow{};
		defensiveRow.fill(fillValue);

		for (const auto &[pairName, pairValue] : definition.defensiveMatchups)
		{
			const std::optional<ub> targetId{registry.getTypeID(pairName)}; // LCOV_EXCL_BR

			if (!targetId.has_value())
			{
				const std::optional<std::string_view> logResult{
					Logger::warn("TypeRegistryConfiguration::addType defensive matchup references unknown type '{}'. Cannot add type '{}'.",
								 pairName, typeName)};

				return std::unexpected{RegistryErrorInfo{RegistryError::TypeNotFound, pairName, logResult.value_or(std::string_view{})}};
			}

			const std::optional<ub> targetIndex{registry.findIndexByTypeID(targetId.value())}; // LCOV_EXCL_BR

			// LCOV_EXCL_START — Defensive: findIndexByTypeID cannot fail when getTypeID just succeeded on the same registry.
			if (!targetIndex.has_value())
			{
				const std::optional<std::string_view> logResult{
					Logger::warn("TypeRegistryConfiguration::addType internal error: type ID found but array index missing for '{}'. "
								 "Cannot add type '{}'.",
								 pairName, typeName)};

				return std::unexpected{RegistryErrorInfo{RegistryError::TypeNotFound, pairName, logResult.value_or(std::string_view{})}};
			}
			// LCOV_EXCL_STOP

			defensiveRow.at(targetIndex.value()) = pairValue;
		}

		// Write directly to the registry — bypasses the positional overload's strict defined-count validation,
		// which is not applicable when the fill policy has already been resolved by name-keyed logic.
		registry.setEntry(registered, TypeEntry{.typeID = registry.getNextTypeID(), .name = typeName});

		for (ub i{0}; i < registered; ++i)
		{
			registry.setTypeChartCell(registered, i, offensiveRow.at(i));
			registry.setTypeChartCell(i, registered, defensiveRow.at(i));
		}

		registry.setTypeChartCell(registered, registered, offensiveRow.at(registered));

		const ub assignedTypeID{registry.getNextTypeID()};

		registry.incrementAmountRegistered();
		registry.incrementNextTypeID();

		return assignedTypeID;
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> TypeRegistryConfiguration::addTypes(
		const std::span<const TypeDefinition> &definitions, const UnspecifiedMatchup defaultBehavior)
	{
		const ub currentCount{registry.getAmountRegistered()};
		const ub currentNextTypeID{registry.getNextTypeID()};

		const ub batchSize{static_cast<ub>(definitions.size())};

		if (batchSize == 0)
		{
			return {};
		}

		if (currentCount + batchSize > MAX_TYPES)
		{
			const std::optional<std::string_view> logResult{Logger::warn(
				"TypeRegistryConfiguration::addTypes registry capacity ({}) exceeded when adding {} types.", MAX_TYPES, batchSize)};

			return std::unexpected{RegistryErrorInfo{RegistryError::MaxCapacity, {}, logResult.value_or(std::string_view{})}};
		}

		// Validate duplicates (against existing registry and within the batch)
		std::unordered_set<std::string_view> seenNames;
		seenNames.reserve(static_cast<std::size_t>(batchSize * 2));

		for (const auto &def : definitions)
		{
			if (seenNames.contains(def.name))
			{
				const std::optional<std::string_view> logResult{
					Logger::warn("TypeRegistryConfiguration::addTypes duplicate type '{}' found within batch. Rolling back entries to "
								 "previous safe state.",
								 def.name)};

				rollbackEntries(currentCount, currentNextTypeID);

				return std::unexpected{RegistryErrorInfo{RegistryError::DuplicateType, def.name, logResult.value_or(std::string_view{})}};
			}

			seenNames.insert(def.name);

			const std::expected<ub, RegistryErrorInfo> result{addType(def, defaultBehavior)}; // LCOV_EXCL_BR

			if (!result.has_value())
			{
				static_cast<void>(Logger::warn(
					"TypeRegistryConfiguration::addTypes Error adding type '{}'. Rolling back entries to previous safe state.", def.name));

				rollbackEntries(currentCount, currentNextTypeID);

				return std::unexpected{result.error()};
			}
		}

		return {};
	}

	ATTR_NODISCARD std::expected<ub, RegistryErrorInfo> TypeRegistryConfiguration::removeType(const std::string_view typeName)
	{
		const std::optional<ub> typeID{registry.getTypeID(typeName)}; // LCOV_EXCL_BR

		if (!typeID.has_value())
		{
			const std::optional<std::string_view> logResult{
				Logger::info("TypeRegistryConfiguration::removeType type not found. Cannot remove type, by name, '{}'.", typeName)};

			return std::unexpected{RegistryErrorInfo{RegistryError::TypeNotFound, typeName, logResult.value_or(std::string_view{})}};
		}

		const std::optional<ub> arrayIndex{registry.findIndexByTypeID(typeID.value())}; // LCOV_EXCL_BR

		// LCOV_EXCL_START — Defensive: findIndexByTypeID cannot fail when getTypeID just succeeded on the same registry.
		if (!arrayIndex.has_value())
		{
			const std::optional<std::string_view> logResult{Logger::info(
				"TypeRegistryConfiguration::removeType internal error: type ID found but array index missing for '{}'.", typeName)};

			return std::unexpected{RegistryErrorInfo{RegistryError::TypeNotFound, typeName, logResult.value_or(std::string_view{})}};
		}
		// LCOV_EXCL_STOP

		const ub removedId{typeID.value()};
		removeEntry(arrayIndex.value());

		return removedId;
	}

	ATTR_NODISCARD std::expected<ub, RegistryErrorInfo> TypeRegistryConfiguration::removeType(const Pokemon::Types::Types type)
	{
		const ub typeID{static_cast<ub>(type)};

		const std::optional<ub> arrayIndex{registry.findIndexByTypeID(typeID)}; // LCOV_EXCL_BR

		if (!arrayIndex.has_value())
		{
			const std::optional<std::string_view> logResult{
				Logger::info("TypeRegistryConfiguration::removeType type not found. Cannot remove type, by ID, '{}'.", typeID)};

			return std::unexpected{RegistryErrorInfo{RegistryError::TypeNotFound, {}, logResult.value_or(std::string_view{})}};
		}

		removeEntry(arrayIndex.value());

		return typeID;
	}

	ATTR_NODISCARD std::expected<ub, RegistryErrorInfo> TypeRegistryConfiguration::removeType(const ub typeID)
	{
		const std::optional<ub> arrayIndex{registry.findIndexByTypeID(typeID)}; // LCOV_EXCL_BR

		if (!arrayIndex.has_value())
		{
			const std::optional<std::string_view> logResult{
				Logger::info("TypeRegistryConfiguration::removeType type not found. Cannot remove type, by stable ID, '{}'.", typeID)};

			return std::unexpected{RegistryErrorInfo{RegistryError::TypeNotFound, {}, logResult.value_or(std::string_view{})}};
		}

		removeEntry(arrayIndex.value());

		return typeID;
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> TypeRegistryConfiguration::removeTypes(
		const std::span<const std::string_view> typeNames)
	{
		// Snapshot the entire registry for all-or-nothing rollback
		const Registry::Types::TypeRegistry snapshot{registry};

		for (const auto &name : typeNames)
		{
			const std::expected<ub, RegistryErrorInfo> result{removeType(name)}; // LCOV_EXCL_BR

			if (!result.has_value())
			{
				static_cast<void>(
					Logger::info("TypeRegistryConfiguration::removeTypes rolling back to previous state due to error on type '{}'.", name));

				registry = snapshot;

				return std::unexpected{result.error()};
			}
		}

		return {};
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> TypeRegistryConfiguration::renameType(const std::string_view oldName,
																								const std::string_view newName)
	{
		const std::optional<ub> typeID{registry.getTypeID(oldName)}; // LCOV_EXCL_BR

		if (!typeID.has_value())
		{
			const std::optional<std::string_view> logResult{
				Logger::info("TypeRegistryConfiguration::renameType type '{}' not found.", oldName)};

			return std::unexpected{RegistryErrorInfo{RegistryError::TypeNotFound, oldName, logResult.value_or(std::string_view{})}};
		}

		if (registry.hasType(newName))
		{
			const std::optional<std::string_view> logResult{
				Logger::warn("TypeRegistryConfiguration::renameType target name '{}' already exists.", newName)};

			return std::unexpected{RegistryErrorInfo{RegistryError::DuplicateType, newName, logResult.value_or(std::string_view{})}};
		}

		const std::optional<ub> arrayIndex{registry.findIndexByTypeID(typeID.value())}; // LCOV_EXCL_BR

		// LCOV_EXCL_START — Defensive: findIndexByTypeID cannot fail when getTypeID just succeeded on the same registry.
		if (!arrayIndex.has_value())
		{
			const std::optional<std::string_view> logResult{Logger::info(
				"TypeRegistryConfiguration::renameType internal error: type ID found but array index missing for '{}'.", oldName)};

			return std::unexpected{RegistryErrorInfo{RegistryError::TypeNotFound, oldName, logResult.value_or(std::string_view{})}};
		}
		// LCOV_EXCL_STOP

		registry.setEntry(arrayIndex.value(), TypeEntry{.typeID = typeID.value(), .name = newName});

		return {};
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> TypeRegistryConfiguration::resetMatchups(const std::string_view typeName)
	{
		const std::expected<ub, RegistryErrorInfo> typeIndex{resolveIndex(typeName, "resetMatchups")}; // LCOV_EXCL_BR

		if (!typeIndex.has_value())
		{
			const std::optional<std::string_view> logResult{
				Logger::info("TypeRegistryConfiguration::resetMatchups type not found for type name '{}'.", typeName)}; // LCOV_EXCL_BR

			return std::unexpected{RegistryErrorInfo{RegistryError::TypeNotFound, typeName, logResult.value_or(std::string_view{})}};
		}

		const ub registered{registry.getAmountRegistered()};
		const ub index{typeIndex.value()}; // LCOV_EXCL_BR

		// Clear offensive row
		for (ub col{0}; col < registered; ++col)
		{
			registry.setTypeChartCell(index, col, TypeEffectiveness::NOT_DEFINED); // LCOV_EXCL_BR
		}

		// Clear defensive column
		for (ub row{0}; row < registered; ++row)
		{
			registry.setTypeChartCell(row, index, TypeEffectiveness::NOT_DEFINED); // LCOV_EXCL_BR
		}

		return {};
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> TypeRegistryConfiguration::resetMatchups(const ub typeID)
	{
		const std::optional<ub> arrayIndex{registry.findIndexByTypeID(typeID)}; // LCOV_EXCL_BR

		if (!arrayIndex.has_value())
		{
			const std::optional<std::string_view> logResult{
				Logger::info("TypeRegistryConfiguration::resetMatchups type not found for stable ID '{}'.", typeID)}; // LCOV_EXCL_BR

			return std::unexpected{RegistryErrorInfo{RegistryError::TypeNotFound, std::to_string(static_cast<Pokemon::Core::sb>(typeID)),
													 logResult.value_or(std::string_view{})}};
		}

		const ub registered{registry.getAmountRegistered()};
		const ub index{arrayIndex.value()}; // LCOV_EXCL_BR

		for (ub col{0}; col < registered; ++col)
		{
			registry.setTypeChartCell(index, col, TypeEffectiveness::NOT_DEFINED); // LCOV_EXCL_BR
		}

		for (ub row{0}; row < registered; ++row)
		{
			registry.setTypeChartCell(row, index, TypeEffectiveness::NOT_DEFINED); // LCOV_EXCL_BR
		}

		return {};
	}

	// MARK: Private Member Function

	void TypeRegistryConfiguration::rollbackEntries(const ub previousCount, const ub previousNextTypeID)
	{
		ub registered{registry.getAmountRegistered()};

		for (ub row{registered}; row > previousCount; --row)
		{
			// LCOV_EXCL_BR_START - Has an error branch due to the functions calling .at(), but the asserts in the functions will prevent
			// those
			// branches from ever being hit

			registry.setEntry(row, TypeEntry{});
			registry.setTypeChartRow(row, {});

			// LCOV_EXCL_BR_STOP

			for (ub col{0}; col < registered; ++col)
			{
				// Clear the defensive matchup cell for this row and column

				// NOLINTNEXTLINE(readability-suspicious-call-argument)
				registry.setTypeChartCell(col, row, TypeEffectiveness::NOT_DEFINED); // LCOV_EXCL_BR
			}
		}

		registry.setAmountRegistered(previousCount);
		registry.setNextTypeID(previousNextTypeID); // LCOV_EXCL_BR
	}

	void TypeRegistryConfiguration::removeEntry(const ub arrayIndex)
	{
		const ub registered{registry.getAmountRegistered()};

		// LCOV_EXCL_BR_START - Has an error branch due to the functions calling .at(), but the asserts in the functions will prevent those
		// branches from ever being hit

		// Set data to default
		registry.setEntry(arrayIndex, TypeEntry{});
		registry.setTypeChartRow(arrayIndex, {});

		// LCOV_EXCL_BR_STOP

		for (ub i{arrayIndex}; i < registered - 1; ++i)
		{
			// LCOV_EXCL_BR_START - Has an error branch due to the functions calling .at(), but the asserts in the functions will prevent
			// those
			// branches from ever being hit

			// Shift elements
			registry.setEntry(i, registry.getEntry(i + 1));
			registry.setTypeChartRow(i, registry.getTypeChartRow(i + 1));

			// LCOV_EXCL_BR_STOP
		}

		registry.decrementAmountRegistered();

		const ub newRegistered{registry.getAmountRegistered()};

		// Clear the vacated last row
		registry.setTypeChartRow(newRegistered, {}); // LCOV_EXCL_BR

		for (ub row{0}; row < newRegistered; ++row)
		{
			for (ub col{arrayIndex}; col < newRegistered; ++col)
			{
				registry.setTypeChartCell(row, col, registry.getTypeChartCell(row, col + 1));
			}

			// Clear the vacated last column cell for this row
			registry.setTypeChartCell(row, newRegistered, TypeEffectiveness::NOT_DEFINED);
		}
	}

	ATTR_NODISCARD std::expected<ub, RegistryErrorInfo> TypeRegistryConfiguration::resolveIndex(const std::string_view name,
																								const std::string_view callerContext)
	{
		const std::optional<ub> typeID{registry.getTypeID(name)}; // LCOV_EXCL_BR

		if (!typeID.has_value())
		{
			const std::optional<std::string_view> logResult{
				Logger::info("TypeRegistryConfiguration::{} type '{}' not found.", callerContext, name)}; // LCOV_EXCL_BR

			return std::unexpected{RegistryErrorInfo{RegistryError::TypeNotFound, name, logResult.value_or(std::string_view{})}};
		}

		// Don't need to check for if it has a value because if getTypeID works, then this is guaranteed to work as they call
		// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
		return registry.findIndexByTypeID(typeID.value()).value(); // LCOV_EXCL_BR
	}
} // namespace Pokemon::Configuration