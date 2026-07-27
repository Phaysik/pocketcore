/*! @file abilityRegistryConfiguration.cpp
	@brief Defines validated user customization operations for the ability registry.
	@date 07/27/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#include "Configuration/abilityRegistryConfiguration.h"

#include <cassert>
#include <cstddef>
#include <expected>
#include <limits>
#include <optional>
#include <span>
#include <string_view>
#include <utility>
#include <vector>

#include "Ability/abilityID.h"
#include "Ability/abilityMeta.h"
#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Registry/abilityRegistry.h"
#include "Utility/Debug/Logging/logger.h"

namespace PocketCore::Configuration
{
	using PocketCore::Ability::AbilityID;
	using PocketCore::Ability::AbilityMeta;
	using PocketCore::Core::us;
	using PocketCore::Utility::Debug::Logging::Logger;

	ATTR_NODISCARD std::expected<AbilityID, RegistryErrorInfo> AbilityRegistryConfiguration::addAbility(const AbilityDefinition &definition)
	{
		if (registry.getAmountRegistered() >= MAX_ABILITIES || registry.getNextAbilityID() == std::numeric_limits<ub>::max())
		{
			const std::optional<std::string_view> logResult{
				Logger::warn(
					"AbilityRegistryConfiguration::addAbility cannot add '{}': registry capacity or ability ID space is exhausted.",
					definition.name),
			};

			return std::unexpected{RegistryErrorInfo{RegistryError::MaxCapacity, definition.name, logResult.value_or(std::string_view{})}};
		}

		if (registry.hasAbility(definition.name))
		{
			const std::optional<std::string_view> logResult{
				Logger::warn("AbilityRegistryConfiguration::addAbility duplicate ability '{}'.", definition.name),
			};

			return std::unexpected{
				RegistryErrorInfo{RegistryError::DuplicateAbility, definition.name, logResult.value_or(std::string_view{})}};
		}

		const us entryIndex{registry.getAmountRegistered()};
		const AbilityID assignedID{registry.getNextAbilityID()};
		std::vector<AbilityEffectTrigger> ownedTriggers{definition.triggers.begin(), definition.triggers.end()};

		registry.setEntry(entryIndex,
						  AbilityMeta{.mAbilityID = assignedID, .mName = definition.name, .mTriggers = std::move(ownedTriggers)});
		registry.incrementAmountRegistered();
		registry.incrementNextAbilityID();

		return assignedID;
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> AbilityRegistryConfiguration::addAbilities(
		const std::span<const AbilityDefinition> definitions)
	{
		if (definitions.size() > static_cast<std::size_t>(MAX_ABILITIES - registry.getAmountRegistered()))
		{
			const std::optional<std::string_view> logResult{
				Logger::warn("AbilityRegistryConfiguration::addAbilities cannot add {} entries: registry capacity is {}.",
							 definitions.size(), MAX_ABILITIES),
			};

			return std::unexpected{RegistryErrorInfo{RegistryError::MaxCapacity, {}, logResult.value_or(std::string_view{})}};
		}

		const Registry::Ability::AbilityRegistry snapshot{registry};

		for (const AbilityDefinition &definition : definitions)
		{
			const std::expected<AbilityID, RegistryErrorInfo> result{addAbility(definition)};

			if (!result.has_value())
			{
				registry = snapshot;
				return std::unexpected{result.error()};
			}
		}

		return {};
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> AbilityRegistryConfiguration::setAbilityTriggers(
		const std::string_view abilityName, const std::span<const AbilityEffectTrigger> triggers)
	{
		const std::expected<ub, RegistryErrorInfo> index{resolveIndex(abilityName, "setAbilityTriggers")};

		if (!index.has_value())
		{
			return std::unexpected{index.error()};
		}

		AbilityMeta metadata{registry.getEntry(index.value())};
		metadata.mTriggers.assign(triggers.begin(), triggers.end());
		registry.setEntry(index.value(), metadata);

		return {};
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> AbilityRegistryConfiguration::setAbilityTriggers(
		const AbilityID abilityID, const std::span<const AbilityEffectTrigger> triggers)
	{
		const std::expected<ub, RegistryErrorInfo> index{resolveIndex(abilityID, "setAbilityTriggers")};

		if (!index.has_value())
		{
			return std::unexpected{index.error()};
		}

		AbilityMeta metadata{registry.getEntry(index.value())};
		metadata.mTriggers.assign(triggers.begin(), triggers.end());
		registry.setEntry(index.value(), metadata);

		return {};
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> AbilityRegistryConfiguration::renameAbility(const std::string_view oldName,
																									  const std::string_view newName)
	{
		const std::expected<ub, RegistryErrorInfo> index{resolveIndex(oldName, "renameAbility")};

		if (!index.has_value())
		{
			return std::unexpected{index.error()};
		}

		if (registry.hasAbility(newName))
		{
			const std::optional<std::string_view> logResult{
				Logger::warn("AbilityRegistryConfiguration::renameAbility target name '{}' already exists.", newName),
			};

			return std::unexpected{RegistryErrorInfo{RegistryError::DuplicateAbility, newName, logResult.value_or(std::string_view{})}};
		}

		AbilityMeta metadata{registry.getEntry(index.value())};
		metadata.mName = newName;
		registry.setEntry(index.value(), metadata);

		return {};
	}

	ATTR_NODISCARD std::expected<AbilityID, RegistryErrorInfo> AbilityRegistryConfiguration::removeAbility(
		const std::string_view abilityName)
	{
		const std::optional<AbilityID> abilityID{registry.getAbilityID(abilityName)};

		if (!abilityID.has_value())
		{
			const std::optional<std::string_view> logResult{
				Logger::info("AbilityRegistryConfiguration::removeAbility ability '{}' not found.", abilityName),
			};

			return std::unexpected{RegistryErrorInfo{RegistryError::AbilityNotFound, abilityName, logResult.value_or(std::string_view{})}};
		}

		const std::optional<ub> index{registry.findIndexByAbilityID(abilityID.value())};
		assert(index.has_value());
		removeEntry(index.value());

		return abilityID.value();
	}

	ATTR_NODISCARD std::expected<AbilityID, RegistryErrorInfo> AbilityRegistryConfiguration::removeAbility(const AbilityID abilityID)
	{
		const std::expected<ub, RegistryErrorInfo> index{resolveIndex(abilityID, "removeAbility")};

		if (!index.has_value())
		{
			return std::unexpected{index.error()};
		}

		removeEntry(index.value());
		return abilityID;
	}

	ATTR_NODISCARD std::expected<us, RegistryErrorInfo> AbilityRegistryConfiguration::resolveIndex(const std::string_view name,
																								   const std::string_view callerContext)
	{
		const std::optional<AbilityID> abilityID{registry.getAbilityID(name)};

		if (!abilityID.has_value())
		{
			const std::optional<std::string_view> logResult{
				Logger::info("AbilityRegistryConfiguration::{} ability '{}' not found.", callerContext, name),
			};

			return std::unexpected{RegistryErrorInfo{RegistryError::AbilityNotFound, name, logResult.value_or(std::string_view{})}};
		}

		// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
		return registry.findIndexByAbilityID(abilityID.value())
			.value(); // LCOV_EXCL_BR - Cannot fail when getAbilityID just succeeded on the same registry
	}

	ATTR_NODISCARD std::expected<us, RegistryErrorInfo> AbilityRegistryConfiguration::resolveIndex(const AbilityID abilityID,
																								   const std::string_view callerContext)
	{
		const std::optional<us> index{registry.findIndexByAbilityID(abilityID)};

		if (!index.has_value())
		{
			const std::optional<std::string_view> logResult{
				Logger::info("AbilityRegistryConfiguration::{} ability ID '{}' not found.", callerContext, abilityID.getValue()),
			};

			return std::unexpected{RegistryErrorInfo{RegistryError::AbilityNotFound, {}, logResult.value_or(std::string_view{})}};
		}

		return index.value();
	}

	void AbilityRegistryConfiguration::removeEntry(const us index)
	{
		const us registered{registry.getAmountRegistered()};

		for (us current{index}; current + 1U < registered; ++current)
		{
			registry.setEntry(current, registry.getEntry(static_cast<us>(current + 1U)));
		}

		registry.setEntry(static_cast<us>(registered - 1U), AbilityMeta{});
		registry.decrementAmountRegistered();
	}
} // namespace PocketCore::Configuration
