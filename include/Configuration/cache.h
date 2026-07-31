/*! @file cache.h
	@brief Defines configuration constants and error metadata used by the type registry.
	@date --/--/----
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_CONFIGURATION_CACHE_H
#define INCLUDE_CONFIGURATION_CACHE_H

#include <array>

#include "Configuration/constants.h"
#include "Core/attributeMacros.h"

namespace PocketCore::Configuration
{
	ATTR_NODISCARD constexpr std::array<float, MAX_STAGE_CACHE> STAT_STAGE_MULTIPLIERS() noexcept
	{
		std::array<float, MAX_STAGE_CACHE> table{};

		constexpr float NUM{BASE_STAGE_MULTIPLIER_NUMERATOR};
		constexpr float DEN{BASE_STAGE_MULTIPLIER_DENOMINATOR};

		for (sb statStage{-MAX_STAT_STAGES}; statStage <= MAX_STAT_STAGES; ++statStage)
		{
			const auto index{static_cast<std::size_t>(statStage + MAX_STAT_STAGES)};

			if (statStage >= 0)
			{
				table.at(index) = (NUM + static_cast<float>(statStage)) / DEN;
			}
			else
			{
				table.at(index) = NUM / (DEN - static_cast<float>(statStage));
			}
		}

		return table;
	}

	ATTR_NODISCARD constexpr std::array<float, MAX_STAGE_CACHE> ACCURACY_STAGE_MULTIPLIERS() noexcept
	{
		std::array<float, MAX_STAGE_CACHE> table{};

		constexpr float NUM{BASE_ACCURACY_MULTIPLIER_NUMERATOR};
		constexpr float DEN{BASE_ACCURACY_MULTIPLIER_DENOMINATOR};

		for (sb statStage{-MAX_STAT_STAGES}; statStage <= MAX_STAT_STAGES; ++statStage)
		{
			const auto index{static_cast<std::size_t>(statStage + MAX_STAT_STAGES)};

			if (statStage >= 0)
			{
				table.at(index) = (NUM + static_cast<float>(statStage)) / DEN;
			}
			else
			{
				table.at(index) = NUM / (DEN - static_cast<float>(statStage));
			}
		}

		return table;
	}

	ATTR_NODISCARD constexpr std::array<float, MAX_STAGE_CACHE> EVASION_STAGE_MULTIPLIERS() noexcept
	{
		std::array<float, MAX_STAGE_CACHE> table{};

		constexpr float NUM{BASE_EVASION_MULTIPLIER_NUMERATOR};
		constexpr float DEN{BASE_EVASION_MULTIPLIER_DENOMINATOR};

		for (sb statStage{-MAX_STAT_STAGES}; statStage <= MAX_STAT_STAGES; ++statStage)
		{
			const auto index{static_cast<std::size_t>(statStage + MAX_STAT_STAGES)};

			if (statStage >= 0)
			{
				table.at(index) = NUM / (DEN + static_cast<float>(statStage));
			}
			else
			{
				table.at(index) = (NUM - static_cast<float>(statStage)) / DEN;
			}
		}

		return table;
	}

	constexpr std::array<float, MAX_STAGE_CACHE> CACHE_STAT_STAGE_MULTIPLIERS{STAT_STAGE_MULTIPLIERS()};
	constexpr std::array<float, MAX_STAGE_CACHE> CACHE_ACCURACY_STAGE_MULTIPLIERS{ACCURACY_STAGE_MULTIPLIERS()};
	constexpr std::array<float, MAX_STAGE_CACHE> CACHE_EVASION_STAGE_MULTIPLIERS{EVASION_STAGE_MULTIPLIERS()};
} // namespace PocketCore::Configuration
#endif
