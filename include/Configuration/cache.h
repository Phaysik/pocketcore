/*! @file cache.h
	@brief Defines constexpr lookup tables for battle stat-stage multipliers.
	@date 08/03/2026
	@since 0.8.2
	@version 0.9.0
	@author Matthew Moore
*/

#ifndef INCLUDE_CONFIGURATION_CACHE_H
#define INCLUDE_CONFIGURATION_CACHE_H

#include <algorithm>
#include <array>
#include <cstddef>

#include "Configuration/constants.h"
#include "Core/attributeMacros.h"

namespace PocketCore::Configuration
{
	/*! @brief Converts a stat stage into a valid multiplier-table index.
		@param[in] statStage The signed stat stage to convert.
		@return The zero-based index corresponding to the clamped stat stage.
		@note Values outside the range from @p -MAX_STAT_STAGES to @p MAX_STAT_STAGES are clamped before conversion.
		@note The function is constexpr and noexcept so it can be used while constructing compile-time lookup tables.
		@since 0.8.7
		@version 0.9.0
	*/
	ATTR_NODISCARD ATTR_PURE constexpr std::size_t statStageCacheIndex(const sb statStage) noexcept
	{
		const sb clampedStage{std::clamp(statStage, static_cast<sb>(-MAX_STAT_STAGES), MAX_STAT_STAGES)};
		return static_cast<std::size_t>(clampedStage + MAX_STAT_STAGES);
	}

	/*! @brief Builds the standard stat-stage multiplier lookup table.
		@return An array containing the standard multiplier for every supported clamped stat stage.
		@note Negative stages use the reciprocal-style denominator formula, while non-negative stages use the numerator-increment formula.
		@note The returned table is constructed at compile time when used in a constant expression.
		@since 0.8.2
		@version 0.8.7
	*/
	ATTR_NODISCARD constexpr std::array<double, MAX_STAGE_CACHE> STAT_STAGE_MULTIPLIERS() noexcept
	{
		std::array<double, MAX_STAGE_CACHE> table{};

		constexpr double NUM{BASE_STAGE_MULTIPLIER_NUMERATOR};
		constexpr double DEN{BASE_STAGE_MULTIPLIER_DENOMINATOR};

		for (sb statStage{-MAX_STAT_STAGES}; statStage <= MAX_STAT_STAGES; ++statStage)
		{
			const std::size_t index{statStageCacheIndex(statStage)};

			if (statStage >= 0)
			{
				table.at(index) = (NUM + static_cast<double>(statStage)) / DEN;
			}
			else
			{
				table.at(index) = NUM / (DEN - static_cast<double>(statStage));
			}
		}

		return table;
	}

	/*! @brief Builds the accuracy-stage multiplier lookup table.
		@return An array containing the accuracy multiplier for every supported clamped stat stage.
		@note Negative stages use the reciprocal-style denominator formula, while non-negative stages use the numerator-increment formula.
		@note The returned table is constructed at compile time when used in a constant expression.
		@since 0.8.2
		@version 0.8.7
	*/
	ATTR_NODISCARD constexpr std::array<double, MAX_STAGE_CACHE> ACCURACY_STAGE_MULTIPLIERS() noexcept
	{
		std::array<double, MAX_STAGE_CACHE> table{};

		constexpr double NUM{BASE_ACCURACY_MULTIPLIER_NUMERATOR};
		constexpr double DEN{BASE_ACCURACY_MULTIPLIER_DENOMINATOR};

		for (sb statStage{-MAX_STAT_STAGES}; statStage <= MAX_STAT_STAGES; ++statStage)
		{
			const std::size_t index{statStageCacheIndex(statStage)};

			if (statStage >= 0)
			{
				table.at(index) = (NUM + static_cast<double>(statStage)) / DEN;
			}
			else
			{
				table.at(index) = NUM / (DEN - static_cast<double>(statStage));
			}
		}

		return table;
	}

	/*! @brief Builds the evasion-stage multiplier lookup table.
		@return An array containing the evasion multiplier for every supported clamped stat stage.
		@note Positive stages reduce the multiplier using the denominator-increment formula, while negative stages use the
	   numerator-increment formula.
		@note The returned table is constructed at compile time when used in a constant expression.
		@since 0.8.2
		@version 0.8.7
	*/
	ATTR_NODISCARD constexpr std::array<double, MAX_STAGE_CACHE> EVASION_STAGE_MULTIPLIERS() noexcept
	{
		std::array<double, MAX_STAGE_CACHE> table{};

		constexpr double NUM{BASE_EVASION_MULTIPLIER_NUMERATOR};
		constexpr double DEN{BASE_EVASION_MULTIPLIER_DENOMINATOR};

		for (sb statStage{-MAX_STAT_STAGES}; statStage <= MAX_STAT_STAGES; ++statStage)
		{
			const std::size_t index{statStageCacheIndex(statStage)};

			if (statStage >= 0)
			{
				table.at(index) = NUM / (DEN + static_cast<double>(statStage));
			}
			else
			{
				table.at(index) = (NUM - static_cast<double>(statStage)) / DEN;
			}
		}

		return table;
	}

	/*! @brief The precomputed standard stat-stage multipliers indexed by @ref statStageCacheIndex. */
	constexpr std::array<double, MAX_STAGE_CACHE> CACHE_STAT_STAGE_MULTIPLIERS{STAT_STAGE_MULTIPLIERS()};
	/*! @brief The precomputed accuracy-stage multipliers indexed by @ref statStageCacheIndex. */
	constexpr std::array<double, MAX_STAGE_CACHE> CACHE_ACCURACY_STAGE_MULTIPLIERS{ACCURACY_STAGE_MULTIPLIERS()};
	/*! @brief The precomputed evasion-stage multipliers indexed by @ref statStageCacheIndex. */
	constexpr std::array<double, MAX_STAGE_CACHE> CACHE_EVASION_STAGE_MULTIPLIERS{EVASION_STAGE_MULTIPLIERS()};
} // namespace PocketCore::Configuration
#endif
