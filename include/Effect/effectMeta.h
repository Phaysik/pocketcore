/*! @file effectMeta.h
	@brief Defines the metadata stored for built-in and user-defined abilities.
	@date 08/12/2026
	@since 0.10.0
	@version x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_EFFECT_EFFECTMETA_H
#define INCLUDE_EFFECT_EFFECTMETA_H

#include <string_view>

#include "Battle/battleState.h"
#include "Effect/builtInEffectID.h"
#include "Effect/effectContext.h"
#include "Registry/registryProvider.h"

#include "effectID.h"

namespace PocketCore::Effect
{
	using PocketCore::Battle::BattleState;
	using PocketCore::Effect::BuiltinEffectID;
	using PocketCore::Effect::EffectContext;
	using PocketCore::Registry::RegistryProvider;

	using EffectFunction = void (*)(BattleState &, EffectContext &, const RegistryProvider &);

	/*! @struct EffectMeta Effect/effectMeta.h
		@brief Stores one effect's stable ID, display name, and owned trigger definitions.
		@details The trigger vector owns its elements and their effect vectors. The display name is a non-owning view whose backing storage
	   must remain valid while this metadata is registered.
		@date 08/12/2026
		@since 0.10.0
		@version 0.10.0
		@author Matthew Moore
	*/
	struct EffectMeta
	{
		public:
			/*! @brief The case-sensitive display name stored as a non-owning view. */
			std::string_view mName{};

			/*! @brief The function to apply this effect. */
			EffectFunction mApply{};

			/*! @brief The stable built-in or user-assigned identifier. */
			EffectID mEffectID{};

			/*! @brief Whether applying this effect may change the active weather. */
			bool mMayChangeWeather{};

			/*! @brief Whether applying this effect may change the targeted Pokemon's statuses. */
			bool mMayChangeStatus{};
	};
} // namespace PocketCore::Effect

#endif
