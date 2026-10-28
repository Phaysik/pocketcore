/*! @file abilityHelpers.h
	@brief Houses free functions that aide in handling abilities.
	@date 10/23/2026
	@since 0.12.26
	@version 0.12.53
	@author Matthew Moore
*/

#ifndef INCLUDE_ABILITY_ABILITY_HELPERS_H
#define INCLUDE_ABILITY_ABILITY_HELPERS_H

#include <algorithm>
#include <functional>
#include <ranges>
#include <span>
#include <utility>
#include <vector>

#include "Core/attributeMacros.h"
#include "Core/concepts.h"
#include "Core/typedefs.h"
#include "Registry/abilityRegistry.h"
#include "Utility/Random/random.h"

#include "abilityID.h"
#include "abilityMeta.h"
#include "builtInAbilityID.h"

namespace PocketCore::Ability
{
	using PocketCore::Core::RandomFn;
	using PocketCore::Core::us;
	using PocketCore::Registry::Ability::AbilityRegistry;
	using PocketCore::Utility::Random::Random;

	/*! @brief Gets a random ability ID from the built-in abilitys.
		@return The random ability ID.
		@since 0.12.26
		@version 0.12.26
	*/
	ATTR_NODISCARD constexpr AbilityID getRandomAbilityIDFromBuiltIns()
	{
		return AbilityID(
			Random::get<us>(std::to_underlying(BuiltinAbilityID::None) + 1, std::to_underlying(BuiltinAbilityID::FinalAbility) - 1));
	}

	/*! @brief Selects a registered ability ID other than @ref NO_ABILITY_ID using an injectable random callable.
		@details Filters @ref AbilityRegistry::getRegisteredAbilities to exclude entries whose ID is @ref NO_ABILITY_ID, then invokes
		   @p func once with inclusive index bounds [0, eligible entry count - 1] and returns the selected entry's stored ID.
		   Entries listed in @p excludedAbilityIDs are also excluded. IDs need not be contiguous.
		   The default @ref Random::get selects eligible entries uniformly.
		@tparam RandomFunc The callable type satisfying @ref RandomFn for @ref us.
		@param[in] config The non-owning registry reference, which must remain valid for the call and is not retained or modified.
		@param[in,out] func The callable accepting two inclusive bounds and returning a value of type @ref us. Defaults to
		   @ref Random::get specialized for @ref us. The callable is forwarded, may update its state, and is not retained.
		@param[in] excludedAbilityIDs Optional non-owning span of IDs to omit; it must remain valid for the call and is not retained or
	   modified.
		@pre At least one registered entry remains after excluding @ref NO_ABILITY_ID and @p excludedAbilityIDs; the caller ensures this.
		@pre @p func is valid for invocation and returns a value within the supplied bounds.
		@post The returned ID is registered, differs from @ref NO_ABILITY_ID, and is absent from @p excludedAbilityIDs.
		@throws std::out_of_range If @p func returns an index outside the eligible-entry range.
		@throws ... Propagates any exception thrown by @p func.
		@note Not thread-safe with the default generator unless the caller synchronizes access to its shared state.
		   The caller must also prevent registry mutation throughout the call, including from @p func, and meet any custom callable's
		   synchronization requirements.
		@return A copy of the selected entry's @ref AbilityID, independent of the registry's lifetime; the result should not be discarded.
		@date 10/23/2026
		@since 0.12.26
		@version 0.12.53
	*/
	template <RandomFn<us> RandomFunc = decltype(&Random::get<us>)>
	ATTR_NODISCARD constexpr AbilityID getRandomAbilityIDFromConfiguration(const AbilityRegistry &config,
																		   RandomFunc &&func = &Random::get<us>,
																		   const std::span<const AbilityID> excludedAbilityIDs = {})
	{
		const std::vector<AbilityMeta> metas{
			std::ranges::to<std::vector<AbilityMeta>>(
				config.getRegisteredAbilities() | std::views::filter([excludedAbilityIDs](const AbilityMeta &meta) {
					return meta.mAbilityID != NO_ABILITY_ID && !std::ranges::contains(excludedAbilityIDs, meta.mAbilityID);
				})),
		};

		const us index{std::invoke(std::forward<RandomFunc>(func), us{0}, static_cast<us>(metas.size() - 1))};

		return metas.at(index).mAbilityID;
	}
} // namespace PocketCore::Ability

#endif
