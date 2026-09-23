/*! @file setPsychicTerrainHandler.h
	@brief Contains the setPsychicTerrain effect handler
	@date 09/23/2026
	@since 0.12.43
	@version 0.12.43
	@author Matthew Moore
*/

#ifndef INCLUDE_EFFECT_SET_PSYCHIC_TERRAIN_HANDLER_H
#define INCLUDE_EFFECT_SET_PSYCHIC_TERRAIN_HANDLER_H

#include "Battle/battleState.h"
#include "Core/attributeMacros.h"
#include "Effect/effectContext.h"
#include "Registry/registryProvider.h"

#include "effectHandlerInterface.h"

namespace PocketCore::Effect
{
	using PocketCore::Battle::BattleSlot;
	using PocketCore::Battle::BattleState;
	using PocketCore::Registry::RegistryProvider;

	/*! @class SetPsychicTerrainHandler EffectHandler/setPsychicTerrainHandler.h
		@brief Sets the battle-wide terrain to psychic terrain.
		@details The handler resolves psychic terrain against the active terrains through the provider's terrain registry and applies any
	   registered terrain interactions. Psychic terrain is inserted into a free slot when one is available; once @ref
	   RulesetPolicy::mMaxTerrains active terrains are present, the oldest terrain is replaced only if @ref
	   RulesetPolicy::mReplaceTerrainWhenFull is true. Blocking interactions leave the active terrains unchanged, and psychic terrain is
	   never inserted twice.
		@note The handler does not inspect the effect context, but it does use the provider's terrain registry.
		@date 09/23/2026
		@since 0.12.43
		@version 0.12.43
		@author Matthew Moore
	*/
	class SetPsychicTerrainHandler : public IEffectHandler
	{
		public:
			/*! @brief Applies psychic terrain to the active terrains according to its registered interactions.
				@param[in,out] state The battle state whose active terrains and ruleset drive insertion and replacement.
				@param[in,out] context The effect context, which is not inspected or modified.
				@param[in] provider The registry provider whose terrain registry resolves psychic terrain's interaction metadata.
				@post @p state reports psychic terrain among its active terrains unless a blocking interaction prevented insertion or the
			   terrain list was full and @ref RulesetPolicy::mReplaceTerrainWhenFull is false.
				@since 0.12.43
				@version 0.12.43
			*/
			void apply(BattleState &state, ATTR_MAYBE_UNUSED EffectContext &context, const RegistryProvider &provider) const override;
	};

	/*! @brief Applies psychic terrain to the active terrains according to its registered interactions.
		@param[in,out] state The battle state whose active terrains and ruleset drive insertion and replacement.
		@param[in,out] context The effect context, which is not inspected or modified.
		@param[in] provider The registry provider whose terrain registry resolves psychic terrain's interaction metadata.
		@post @p state reports psychic terrain among its active terrains unless a blocking interaction prevented insertion or the terrain
	   list was full and @ref RulesetPolicy::mReplaceTerrainWhenFull is false.
		@since 0.12.43
		@version 0.12.43
	*/
	inline void applySetPsychic(BattleState &state, EffectContext &context, const RegistryProvider &provider)
	{
		SetPsychicTerrainHandler{}.apply(state, context, provider);
	}
} // namespace PocketCore::Effect

#endif
