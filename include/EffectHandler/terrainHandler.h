/*! @file terrainHandler.h
	@brief Contains the terrain effect handler
	@date 09/02/2026
	@since 0.9.10
	@version 0.12.17
	@author Matthew Moore
*/

#ifndef INCLUDE_EFFECT_TERRAINHANDLER_H
#define INCLUDE_EFFECT_TERRAINHANDLER_H

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

	/*! @class TerrainHandler EffectHandler/terrainHandler.h
		@brief Applies the active terrain's damage adjustment to the effect user.
		@details The handler scales the user's attack damage-formula modifier when the move's type matches the active terrain, boosting
			electric, grass, and psychic moves in their respective terrains and weakening dragon moves in misty terrain.
		@warning The context must identify valid user and target slots, and the user's modifier is mutated in place rather than stored as
			a context multiplier.
		@note Terrain effects are skipped entirely when the target is not grounded.
		@date 09/02/2026
		@since 0.9.10
		@version 0.12.17
		@author Matthew Moore
	*/
	class TerrainHandler : public IEffectHandler
	{
		public:
			/*! @brief Scales the user's attack modifier according to the active terrain.
				@param[in,out] state The battle state containing the active terrain and both battle slots.
				@param[in,out] context The effect context identifying the slots and the move's type.
				@param[in] provider The registry provider, which is not used by this handler.
				@pre The user and target indexes in @p context must refer to existing slots in their selected teams.
				@post The user's attack damage-formula modifier is multiplied by the terrain value matching the move's type.
				@note The state remains unchanged when the target is ungrounded or the move's type does not match the terrain.
				@since 0.9.10
				@version 0.12.17
			*/
			void apply(BattleState &state, EffectContext &context, ATTR_MAYBE_UNUSED const RegistryProvider &provider) const override;
	};

	/*! @brief Applies the active terrain's damage adjustment for an effect context.
		@param[in,out] state The battle state containing the active terrain and both battle slots.
		@param[in,out] context The effect context identifying the slots and the move's type.
		@param[in] provider The registry provider, which is not used by this handler.
		@pre The user and target indexes in @p context must refer to existing slots in their selected teams.
		@post The user's attack damage-formula modifier is multiplied by the terrain value matching the move's type.
		@note The state remains unchanged when the target is ungrounded or the move's type does not match the terrain.
		@since 0.10.0
		@version 0.12.9
	*/
	inline void applyTerrain(BattleState &state, EffectContext &context, const RegistryProvider &provider)
	{
		TerrainHandler{}.apply(state, context, provider);
	}
} // namespace PocketCore::Effect

#endif
