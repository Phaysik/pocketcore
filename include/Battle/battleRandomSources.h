/*! @file battleRandomSources.h
	@brief Provides access to default random sources for battle-related numeric types.
	@date 10/19/2026
	@since 0.12.51
	@version 0.12.51
	@author Matthew Moore
*/

#ifndef INCLUDE_BATTLE_BATTLE_RANDOM_SOURCES_H
#define INCLUDE_BATTLE_BATTLE_RANDOM_SOURCES_H

#include <cstddef>
#include <functional>

#include "Core/typedefs.h"
#include "Utility/Random/defaultRandomSource.h"
#include "Utility/Random/randomSourceInterface.h"

namespace PocketCore::Battle
{
	using PocketCore::Core::sb;
	using PocketCore::Core::ub;
	using PocketCore::Utility::Random::getDefaultRandomSource;
	using PocketCore::Utility::Random::IRandomSource;

	/*! @struct BattleRandomSources battleRandomSources.h "Battle/battleRandomSources.h"
		@brief Groups non-owning random source references for battle configuration.
		@details Each slot can be rebound to a compatible @ref PocketCore::Utility::Random::IRandomSource.
			Custom sources must outlive every use through this configuration; copying the configuration copies references, not sources.
		@note Each slot initially references @ref PocketCore::Utility::Random::defaultRandomSource for its numeric type.
			These type-specific sources share the underlying generator.
		@warning The default sources are not thread-safe. Callers must synchronize concurrent access to their shared generator.
			Custom sources define their own synchronization requirements.
		@date 10/19/2026
		@version 0.12.51
		@since 0.12.51
	*/
	struct BattleRandomSources
	{
		public:
			/*! @brief Holds the non-owning random source reference for unsigned bytes of type @ref PocketCore::Core::ub.
			 */
			std::reference_wrapper<IRandomSource<ub>> mUnsigned{getDefaultRandomSource<ub>()};

			/*! @brief Holds the non-owning random source reference for signed bytes of type @ref PocketCore::Core::sb.
			 */
			std::reference_wrapper<IRandomSource<sb>> mSigned{getDefaultRandomSource<sb>()};

			/*! @brief Holds the non-owning random source reference for double-precision floating-point values.
			 */
			std::reference_wrapper<IRandomSource<double>> mReal{getDefaultRandomSource<double>()};

			/*! @brief Holds the non-owning random source reference for indices of type std::size_t.
			 */
			std::reference_wrapper<IRandomSource<std::size_t>> mIndex{getDefaultRandomSource<std::size_t>()};
	};
} // namespace PocketCore::Battle

#endif
