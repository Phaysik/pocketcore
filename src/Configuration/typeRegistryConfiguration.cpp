/*! @file typeRegistryConfiguration.cpp
	@brief Contains the function definitions for creating a type registry configuration
	@date 09/10/2026
	@since 0.2.0
	@version 0.12.20
	@author Matthew Moore
*/

#include "Configuration/typeRegistryConfiguration.h"

#include <array>
#include <cstddef>
#include <expected>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <unordered_set>
#include <utility>
#include <vector>

#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Registry/registryError.h"
#include "Registry/typeRegistry.h"
#include "Types/typeEffectiveness.h"
#include "Types/typeID.h"
#include "Types/typeMeta.h"
#include "Utility/Debug/Logging/logger.h"

namespace PocketCore::Configuration
{
	using PocketCore::Configuration::MatchupPair;
	using PocketCore::Configuration::MAX_TYPES;
	using PocketCore::Core::us;
	using PocketCore::Registry::RegistryError;
	using PocketCore::Registry::RegistryErrorInfo;
	using PocketCore::Registry::Type::TypeMeta;
	using PocketCore::Type::TypeEffectiveness;
	using PocketCore::Type::TypeID;
	using PocketCore::Utility::Debug::Logging::Logger;

	// MARK: Getters

	ATTR_NODISCARD std::expected<TypeEffectiveness, RegistryErrorInfo> TypeRegistryConfiguration::getMatchup(
		const std::string_view &attackerName, const std::string_view &defenderName)
	{
		const std::expected<us, RegistryErrorInfo> attackerIndex{resolveIndex(attackerName, "getMatchup")}; // LCOV_EXCL_BR

		if (!attackerIndex.has_value())
		{
			return std::unexpected{attackerIndex.error()};
		}

		const std::expected<us, RegistryErrorInfo> defenderIndex{resolveIndex(defenderName, "getMatchup")}; // LCOV_EXCL_BR

		if (!defenderIndex.has_value())
		{
			return std::unexpected{defenderIndex.error()};
		}

		return getRegistry().getTypeChartCell(attackerIndex.value(), defenderIndex.value());
	}

	ATTR_NODISCARD std::expected<std::array<TypeEffectiveness, MAX_TYPES>, RegistryErrorInfo> TypeRegistryConfiguration::getMatchupRow(
		const std::string_view &attackerName)
	{
		const std::expected<us, RegistryErrorInfo> attackerIndex{resolveIndex(attackerName, "getMatchupRow")}; // LCOV_EXCL_BR

		if (!attackerIndex.has_value())
		{
			return std::unexpected{attackerIndex.error()};
		}

		return getRegistry().getTypeChartRow(attackerIndex.value());
	}

	ATTR_NODISCARD std::expected<std::array<TypeEffectiveness, MAX_TYPES>, RegistryErrorInfo> TypeRegistryConfiguration::getDefensiveColumn(
		const std::string_view &defenderName)
	{
		const std::expected<us, RegistryErrorInfo> defenderIndex{resolveIndex(defenderName, "getDefensiveColumn")}; // LCOV_EXCL_BR

		if (!defenderIndex.has_value())
		{
			return std::unexpected{defenderIndex.error()};
		}

		std::array<TypeEffectiveness, MAX_TYPES> column{};
		const us registered{getRegistry().getAmountRegistered()};

		for (us row{0}; row < registered; ++row)
		{
			column.at(row) = getRegistry().getTypeChartCell(row, defenderIndex.value());
		}

		return column;
	}

	// MARK: Setters

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> TypeRegistryConfiguration::setMatchup(const std::string_view &attackerName,
																								const std::string_view &defenderName,
																								const TypeEffectiveness value)
	{
		const std::expected<us, RegistryErrorInfo> attackerIndex{resolveIndex(attackerName, "setMatchup")}; // LCOV_EXCL_BR

		if (!attackerIndex.has_value())
		{
			return std::unexpected{attackerIndex.error()};
		}

		const std::expected<us, RegistryErrorInfo> defenderIndex{resolveIndex(defenderName, "setMatchup")}; // LCOV_EXCL_BR

		if (!defenderIndex.has_value())
		{
			return std::unexpected{defenderIndex.error()};
		}

		setTypeChartCell(attackerIndex.value(), defenderIndex.value(), value);

		return {};
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> TypeRegistryConfiguration::setMatchupRow(
		const std::string_view &attackerName, const std::span<const TypeEffectiveness> &newRow)
	{
		if (newRow.size() > MAX_TYPES)
		{
			const std::optional<std::string_view> logResult{
				Logger::warn("TypeRegistryConfiguration::setMatchupRow row span size ({}) exceeds MAX_TYPES ({}).", newRow.size(),
							 MAX_TYPES),
			};

			return std::unexpected{RegistryErrorInfo{RegistryError::MatchupMismatch, attackerName, logResult.value_or(std::string_view{})}};
		}

		const std::expected<us, RegistryErrorInfo> attackerIndex{resolveIndex(attackerName, "setMatchupRow")}; // LCOV_EXCL_BR

		if (!attackerIndex.has_value())
		{
			return std::unexpected{attackerIndex.error()};
		}

		const us registered{getRegistry().getAmountRegistered()};

		for (us col{0}; col < registered; ++col)
		{
			setTypeChartCell(attackerIndex.value(), col, col < newRow.size() ? newRow.at(col) : TypeEffectiveness::NOT_DEFINED);
		}

		return {};
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> TypeRegistryConfiguration::setMatchupRow(
		const std::string_view &attackerName, const std::span<const MatchupPair> &newRow)
	{
		if (newRow.size() > MAX_TYPES)
		{
			const std::optional<std::string_view> logResult{
				Logger::warn("TypeRegistryConfiguration::setMatchupRow row span size ({}) exceeds MAX_TYPES ({}).", newRow.size(),
							 MAX_TYPES),
			};

			return std::unexpected{RegistryErrorInfo{RegistryError::MatchupMismatch, attackerName, logResult.value_or(std::string_view{})}};
		}

		const std::expected<us, RegistryErrorInfo> attackerIndex{resolveIndex(attackerName, "setMatchupRow")}; // LCOV_EXCL_BR

		if (!attackerIndex.has_value())
		{
			return std::unexpected{attackerIndex.error()};
		}

		std::vector<std::pair<us, TypeEffectiveness>> updates;
		updates.reserve(newRow.size());

		for (const auto &[pairName, pairValue] : newRow)
		{
			const std::expected<us, RegistryErrorInfo> targetIndex{resolveIndex(pairName, "setMatchupRow")};

			if (!targetIndex)
			{
				return std::unexpected{targetIndex.error()};
			}

			updates.emplace_back(targetIndex.value(), pairValue);
		}

		for (const auto &[targetIndex, value] : updates)
		{
			setTypeChartCell(attackerIndex.value(), targetIndex, value);
		}

		return {};
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> TypeRegistryConfiguration::setDefensiveColumn(
		const std::string_view &defenderName, const std::span<const TypeEffectiveness> &newCol)
	{
		if (newCol.size() > MAX_TYPES)
		{
			const std::optional<std::string_view> logResult{
				Logger::warn("TypeRegistryConfiguration::setDefensiveColumn column span size ({}) exceeds MAX_TYPES ({}).", newCol.size(),
							 MAX_TYPES),
			};

			return std::unexpected{RegistryErrorInfo{RegistryError::MatchupMismatch, defenderName, logResult.value_or(std::string_view{})}};
		}

		const std::expected<us, RegistryErrorInfo> defenderIndex{resolveIndex(defenderName, "setDefensiveColumn")}; // LCOV_EXCL_BR

		if (!defenderIndex.has_value())
		{
			return std::unexpected{defenderIndex.error()};
		}

		const us registered{getRegistry().getAmountRegistered()};

		for (us row{0}; row < registered; ++row)
		{
			setTypeChartCell(row, defenderIndex.value(), row < newCol.size() ? newCol.at(row) : TypeEffectiveness::NOT_DEFINED);
		}

		return {};
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> TypeRegistryConfiguration::setDefensiveColumn(
		const std::string_view &defenderName, const std::span<const MatchupPair> &newCol)
	{
		if (newCol.size() > MAX_TYPES)
		{
			const std::optional<std::string_view> logResult{
				Logger::warn("TypeRegistryConfiguration::setDefensiveColumn column span size ({}) exceeds MAX_TYPES ({}).", newCol.size(),
							 MAX_TYPES),
			};

			return std::unexpected{RegistryErrorInfo{RegistryError::MatchupMismatch, defenderName, logResult.value_or(std::string_view{})}};
		}

		const std::expected<us, RegistryErrorInfo> defenderIndex{resolveIndex(defenderName, "setDefensiveColumn")}; // LCOV_EXCL_BR

		if (!defenderIndex.has_value())
		{
			return std::unexpected{defenderIndex.error()};
		}

		std::vector<std::pair<us, TypeEffectiveness>> updates;
		updates.reserve(newCol.size());

		for (const auto &[pairName, pairValue] : newCol)
		{
			const std::expected<us, RegistryErrorInfo> attackerIndex{resolveIndex(pairName, "setDefensiveColumn")};

			if (!attackerIndex)
			{
				return std::unexpected{attackerIndex.error()};
			}

			updates.emplace_back(attackerIndex.value(), pairValue);
		}

		for (const auto &[attackerIndex, value] : updates)
		{
			setTypeChartCell(attackerIndex, defenderIndex.value(), value);
		}

		return {};
	}

	// MARK: Member Functions

	ATTR_NODISCARD std::expected<TypeID, RegistryErrorInfo> TypeRegistryConfiguration::addType(const TypeDefinition &definition,
																							   const UnspecifiedMatchup defaultBehavior)
	{
		const us registered{getRegistry().getAmountRegistered()};
		const std::string typeName{definition.mName};

		if (registered >= MAX_TYPES || TypeID{getRegistry().getNextTypeID()} == PocketCore::Type::NO_TYPE_ID)
		{
			const std::optional<std::string_view> logResult{
				Logger::warn("TypeRegistryConfiguration: registry capacity or type ID space exhausted. Cannot add type '{}'.", typeName),
			};

			return std::unexpected{RegistryErrorInfo{RegistryError::MaxCapacity, typeName, logResult.value_or(std::string_view{})}};
		}

		if (getRegistry().hasType(typeName))
		{
			const std::optional<std::string_view> logResult{
				Logger::warn("TypeRegistryConfiguration::addType duplicate type found. Cannot add type '{}'.", typeName),
			};

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

		for (const auto &[pairName, pairValue] : definition.mOffensiveMatchups)
		{
			if (pairName == typeName)
			{
				// Self-matchup: will occupy the slot at index 'registered'
				offensiveRow.at(registered) = pairValue;
				continue;
			}

			const std::optional<TypeID> targetId{getRegistry().getTypeID(pairName)}; // LCOV_EXCL_BR

			if (!targetId.has_value())
			{
				const std::optional<std::string_view> logResult{
					Logger::warn("TypeRegistryConfiguration::addType offensive matchup references unknown type '{}'. Cannot add type '{}'.",
								 pairName, typeName),
				};

				return std::unexpected{RegistryErrorInfo{RegistryError::TypeNotFound, pairName, logResult.value_or(std::string_view{})}};
			}

			const std::optional<us> targetIndex{getRegistry().findIndexByTypeID(targetId.value())}; // LCOV_EXCL_BR

			// LCOV_EXCL_START — Defensive: findIndexByTypeID cannot fail when getTypeID just succeeded on the same getRegistry().
			if (!targetIndex.has_value())
			{
				const std::optional<std::string_view> logResult{
					Logger::warn("TypeRegistryConfiguration::addType internal error: type ID found but array index missing for '{}'. "
								 "Cannot add type '{}'.",
								 pairName, typeName),
				};

				return std::unexpected{RegistryErrorInfo{RegistryError::TypeNotFound, pairName, logResult.value_or(std::string_view{})}};
			}
			// LCOV_EXCL_STOP

			offensiveRow.at(targetIndex.value()) = pairValue;
		}

		std::array<TypeEffectiveness, MAX_TYPES> defensiveRow{};
		defensiveRow.fill(fillValue);

		for (const auto &[pairName, pairValue] : definition.mDefensiveMatchups)
		{
			const std::optional<TypeID> targetId{getRegistry().getTypeID(pairName)}; // LCOV_EXCL_BR

			if (!targetId.has_value())
			{
				const std::optional<std::string_view> logResult{
					Logger::warn("TypeRegistryConfiguration::addType defensive matchup references unknown type '{}'. Cannot add type '{}'.",
								 pairName, typeName),
				};

				return std::unexpected{RegistryErrorInfo{RegistryError::TypeNotFound, pairName, logResult.value_or(std::string_view{})}};
			}

			const std::optional<us> targetIndex{getRegistry().findIndexByTypeID(targetId.value())}; // LCOV_EXCL_BR

			// LCOV_EXCL_START — Defensive: findIndexByTypeID cannot fail when getTypeID just succeeded on the same getRegistry().
			if (!targetIndex.has_value())
			{
				const std::optional<std::string_view> logResult{
					Logger::warn("TypeRegistryConfiguration::addType internal error: type ID found but array index missing for '{}'. "
								 "Cannot add type '{}'.",
								 pairName, typeName),
				};

				return std::unexpected{RegistryErrorInfo{RegistryError::TypeNotFound, pairName, logResult.value_or(std::string_view{})}};
			}
			// LCOV_EXCL_STOP

			defensiveRow.at(targetIndex.value()) = pairValue;
		}

		const TypeID assignedTypeID{addEntry(TypeMeta{.mOffensiveMatchups = offensiveRow, .mName = typeName})};

		for (us i{0}; i < registered; ++i)
		{
			setTypeChartCell(i, registered, defensiveRow.at(i));
		}

		return assignedTypeID;
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> TypeRegistryConfiguration::addTypes(
		const std::span<const TypeDefinition> &definitions, const UnspecifiedMatchup defaultBehavior)
	{
		const us currentCount{getRegistry().getAmountRegistered()};
		const TypeRegistry::Checkpoint checkpoint{createCheckpoint()};

		const us batchSize{static_cast<us>(definitions.size())};

		if (batchSize == 0)
		{
			return {};
		}

		if (currentCount + batchSize > MAX_TYPES)
		{
			const std::optional<std::string_view> logResult{
				Logger::warn("TypeRegistryConfiguration::addTypes registry capacity ({}) exceeded when adding {} types.", MAX_TYPES,
							 batchSize),
			};

			return std::unexpected{RegistryErrorInfo{RegistryError::MaxCapacity, {}, logResult.value_or(std::string_view{})}};
		}

		// Validate duplicates (against existing registry and within the batch)
		std::unordered_set<std::string_view> seenNames;
		seenNames.reserve(static_cast<std::size_t>(batchSize * 2));

		for (const auto &def : definitions)
		{
			if (seenNames.contains(def.mName))
			{
				const std::optional<std::string_view> logResult{
					Logger::warn("TypeRegistryConfiguration::addTypes duplicate type '{}' found within batch. Rolling back entries to "
								 "previous safe state.",
								 def.mName),
				};

				rollbackEntries(currentCount, checkpoint);

				return std::unexpected{RegistryErrorInfo{RegistryError::DuplicateType, def.mName, logResult.value_or(std::string_view{})}};
			}

			seenNames.insert(def.mName);

			const std::expected<TypeID, RegistryErrorInfo> result{addType(def, defaultBehavior)}; // LCOV_EXCL_BR

			if (!result.has_value())
			{
				static_cast<void>(Logger::warn(
					"TypeRegistryConfiguration::addTypes Error adding type '{}'. Rolling back entries to previous safe state.", def.mName));

				rollbackEntries(currentCount, checkpoint);

				return std::unexpected{result.error()};
			}
		}

		return {};
	}

	ATTR_NODISCARD std::expected<TypeID, RegistryErrorInfo> TypeRegistryConfiguration::removeType(const std::string_view &typeName)
	{
		return removeMetadata(typeName);
	}

	ATTR_NODISCARD std::expected<TypeID, RegistryErrorInfo> TypeRegistryConfiguration::removeType(const TypeID typeID)
	{
		return removeMetadata(typeID);
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> TypeRegistryConfiguration::renameType(const std::string_view &oldName,
																								const std::string_view &newName)
	{
		return renameMetadata(oldName, newName);
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> TypeRegistryConfiguration::resetMatchups(const std::string_view &typeName)
	{
		const std::expected<us, RegistryErrorInfo> typeIndex{resolveIndex(typeName, "resetMatchups")}; // LCOV_EXCL_BR

		if (!typeIndex.has_value())
		{
			const std::optional<std::string_view> logResult{
				Logger::info("TypeRegistryConfiguration::resetMatchups type not found for type name '{}'.", typeName),
			}; // LCOV_EXCL_BR

			return std::unexpected{RegistryErrorInfo{RegistryError::TypeNotFound, typeName, logResult.value_or(std::string_view{})}};
		}

		clearRows(typeIndex.value()); // LCOV_EXCL_BR

		return {};
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> TypeRegistryConfiguration::resetMatchups(const TypeID typeID)
	{
		const std::optional<us> arrayIndex{getRegistry().findIndexByTypeID(typeID)}; // LCOV_EXCL_BR

		if (!arrayIndex.has_value())
		{
			const std::optional<std::string_view> logResult{
				Logger::info("TypeRegistryConfiguration::resetMatchups type not found for stable ID '{}'.", typeID.getValue()),
			}; // LCOV_EXCL_BR

			return std::unexpected{
				RegistryErrorInfo{RegistryError::TypeNotFound, {}, logResult.value_or(std::string_view{})},
			};
		}

		clearRows(arrayIndex.value()); // LCOV_EXCL_BR

		return {};
	}

	// MARK: Private Member Function

	void TypeRegistryConfiguration::rollbackEntries(const us previousCount, const TypeRegistry::Checkpoint checkpoint)
	{
		const us registered{getRegistry().getAmountRegistered()};

		for (us row{previousCount}; row < registered; ++row)
		{
			setTypeChartRow(row, {});

			for (us col{0}; col < registered; ++col)
			{
				// NOLINTNEXTLINE(readability-suspicious-call-argument)
				setTypeChartCell(col, row, TypeEffectiveness::NOT_DEFINED); // LCOV_EXCL_BR
			}
		}

		restoreCheckpoint(checkpoint);
	}

	ATTR_NODISCARD std::expected<us, RegistryErrorInfo> TypeRegistryConfiguration::resolveIndex(const std::string_view &name,
																								const std::string_view &callerContext)
	{
		const std::optional<TypeID> typeID{getRegistry().getTypeID(name)}; // LCOV_EXCL_BR

		if (!typeID.has_value())
		{
			const std::optional<std::string_view> logResult{
				Logger::info("TypeRegistryConfiguration::{} type '{}' not found.", callerContext, name),
			}; // LCOV_EXCL_BR

			return std::unexpected{RegistryErrorInfo{RegistryError::TypeNotFound, name, logResult.value_or(std::string_view{})}};
		}

		// Don't need to check for if it has a value because if getTypeID works, then this is guaranteed to work as they call
		// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
		return getRegistry().findIndexByTypeID(typeID.value()).value(); // LCOV_EXCL_BR
	}
} // namespace PocketCore::Configuration
