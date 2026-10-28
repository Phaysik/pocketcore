/*! @file natureHelpers.h
	@brief Houses free functions that aide in handling natures.
	@date 10/23/2026
	@since 0.12.26
	@version 0.12.53
	@author Matthew Moore
*/

#ifndef INCLUDE_NATURE_NATURE_HELPERS_H
#define INCLUDE_NATURE_NATURE_HELPERS_H

#include <algorithm>
#include <functional>
#include <ranges>
#include <span>
#include <utility>
#include <vector>

#include "Core/attributeMacros.h"
#include "Core/concepts.h"
#include "Core/typedefs.h"
#include "Registry/natureRegistry.h"
#include "Utility/Random/random.h"

#include "builtInNatureID.h"
#include "natureID.h"
#include "natureMeta.h"

namespace PocketCore::Nature
{
	using PocketCore::Core::RandomFn;
	using PocketCore::Core::us;
	using PocketCore::Registry::Nature::NatureRegistry;
	using PocketCore::Utility::Random::Random;

	/*! @brief Selects a built-in nature ID using an injectable random callable.
		@details Invokes @p func once with inclusive bounds from one past @ref BuiltinNatureID::None to one before
		   @ref BuiltinNatureID::FinalNature, excluding both sentinel values. The default @ref Random::get uses a uniform distribution.
		@tparam RandomFunc The callable type satisfying @ref RandomFn for @ref us.
		@param[in,out] func The callable accepting two inclusive bounds and returning a value of type @ref us. Defaults to
		   @ref Random::get specialized for @ref us. The callable is forwarded, may update its state, and is not retained.
		@pre @p func is valid for invocation and returns a value within the supplied bounds.
		@note Not thread-safe with the default generator unless the caller synchronizes access to its shared state.
		   Custom callables determine their own synchronization requirements.
		@return The built-in @ref NatureID wrapping the generated value; the result should not be discarded.
		@since 0.12.26
		@version 0.12.53
	*/
	template <RandomFn<us> RandomFunc = decltype(&Random::get<us>)>
	ATTR_NODISCARD constexpr NatureID getRandomNatureIDFromBuiltIns(RandomFunc &&func = &Random::get<us>)
	{
		return NatureID{
			std::invoke(std::forward<RandomFunc>(func), std::to_underlying(BuiltinNatureID::None) + 1,
						std::to_underlying(BuiltinNatureID::FinalNature) - 1), // FinalNature has no registered metadata so subtract one
		};
	}

	/*! @brief Selects a registered nature ID other than @ref NO_NATURE_ID using an injectable random callable.
		@details Filters @ref NatureRegistry::getRegisteredNatures to exclude entries whose ID is @ref NO_NATURE_ID, then invokes
		   @p func once with inclusive index bounds [0, eligible entry count - 1] and returns the selected entry's stored ID.
		   Entries listed in @p excludedNatureIDs are also excluded. IDs need not be contiguous.
		   The default @ref Random::get selects eligible entries uniformly.
		@tparam RandomFunc The callable type satisfying @ref RandomFn for @ref us.
		@param[in] config The non-owning registry reference, which must remain valid for the call and is not retained or modified.
		@param[in,out] func The callable accepting two inclusive bounds and returning a value of type @ref us. Defaults to
		   @ref Random::get specialized for @ref us. The callable is forwarded, may update its state, and is not retained.
		@param[in] excludedNatureIDs Optional non-owning span of IDs to omit; it must remain valid for the call and is not retained or
	   modified.
		@pre At least one registered entry remains after excluding @ref NO_NATURE_ID and @p excludedNatureIDs; the caller ensures this.
		@pre @p func is valid for invocation and returns a value within the supplied bounds.
		@post The returned ID is registered, differs from @ref NO_NATURE_ID, and is absent from @p excludedNatureIDs.
		@throws std::out_of_range If @p func returns an index outside the eligible-entry range.
		@throws ... Propagates any exception thrown by @p func.
		@note Not thread-safe with the default generator unless the caller synchronizes access to its shared state.
		   The caller must also prevent registry mutation throughout the call, including from @p func, and meet any custom callable's
		   synchronization requirements.
		@return A copy of the selected entry's @ref NatureID, independent of the registry's lifetime; the result should not be discarded.
		@date 10/23/2026
		@since 0.12.26
		@version 0.12.53
	*/
	template <RandomFn<us> RandomFunc = decltype(&Random::get<us>)>
	ATTR_NODISCARD constexpr NatureID getRandomNatureIDFromConfiguration(const NatureRegistry &config, RandomFunc &&func = &Random::get<us>,
																		 const std::span<const NatureID> excludedNatureIDs = {})
	{
		const std::vector<NatureMeta> metas{
			std::ranges::to<std::vector<NatureMeta>>(
				config.getRegisteredNatures() | std::views::filter([excludedNatureIDs](const NatureMeta &meta) {
					return meta.mNatureID != NO_NATURE_ID && !std::ranges::contains(excludedNatureIDs, meta.mNatureID);
				})),
		};

		const us index{std::invoke(std::forward<RandomFunc>(func), us{0}, static_cast<us>(metas.size() - 1))};

		return metas.at(index).mNatureID;
	}
} // namespace PocketCore::Nature

#endif
