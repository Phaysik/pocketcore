/*! @file weatherHandler.h
	@brief Contains the weather effect handler
	@date 10/28/2026
	@since 0.9.8
	@version 0.12.53
	@author Matthew Moore
*/

#ifndef INCLUDE_EFFECT_WEATHERHANDLER_H
#define INCLUDE_EFFECT_WEATHERHANDLER_H

#include "Battle/battleRandomSources.h"
#include "Battle/battleState.h"
#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Effect/effectContext.h"
#include "Move/builtInMoveID.h"
#include "Move/moveID.h"
#include "Registry/registryProvider.h"
#include "Type/builtInTypeID.h"
#include "Weather/builtInWeatherID.h"
#include "Weather/weatherID.h"

#include "effectHandlerInterface.h"

namespace PocketCore::Effect
{
	using PocketCore::Battle::BattleRandomSources;
	using PocketCore::Battle::BattleSlot;
	using PocketCore::Battle::BattleState;
	using PocketCore::Configuration::FIRE_MOVE_IN_EXTREMELY_HARSH_SUNLIGHT_MULTIPLIER_VALUE;
	using PocketCore::Configuration::FIRE_MOVE_IN_HARSH_SUNLIGHT_MULTIPLIER_VALUE;
	using PocketCore::Configuration::FIRE_MOVE_IN_HEAVY_RAIN_MULTIPLIER_VALUE;
	using PocketCore::Configuration::FIRE_MOVE_IN_RAIN_MULTIPLIER_VALUE;
	using PocketCore::Configuration::HYDRO_STEAM_MOVE_IN_HARSH_SUNLIGHT_MULTIPLIER_VALUE;
	using PocketCore::Configuration::WATER_MOVE_IN_EXTREMELY_HARSH_SUNLIGHT_MULTIPLIER_VALUE;
	using PocketCore::Configuration::WATER_MOVE_IN_HARSH_SUNLIGHT_MULTIPLIER_VALUE;
	using PocketCore::Configuration::WATER_MOVE_IN_HEAVY_RAIN_MULTIPLIER_VALUE;
	using PocketCore::Configuration::WATER_MOVE_IN_RAIN_MULTIPLIER_VALUE;
	using PocketCore::Configuration::WEATHER_NULLIFICATION_MULTIPLIER_VALUE;
	using PocketCore::Move::BuiltinMoveID;
	using PocketCore::Move::MoveID;
	using PocketCore::Registry::RegistryProvider;
	using PocketCore::Type::BuiltinTypeID;
	using PocketCore::Type::toTypeID;
	using PocketCore::Weather::BuiltinWeatherID;
	using PocketCore::Weather::WeatherID;

	/*! @class WeatherHandler EffectHandler/weatherHandler.h
		@brief Applies the weather damage multiplier for a move.
		@details The handler first scans both parties for a weather-nullifying ability and stores the neutral multiplier when one is
			present. Otherwise it selects the rain or harsh sunlight multiplier based on the move's type, treating Hydro Steam as a
			sunlight-boosted move.
		@note The handler inspects both parties rather than only the active slots, and does not use the registry provider.
		@date 10/28/2026
		@since 0.9.8
		@version 0.12.53
		@author Matthew Moore
	*/
	class WeatherHandler : public IEffectHandler
	{
		public:
			/*! @brief Stores the weather multiplier that applies to the current move.
				@param[in,out] state The battle state containing the active weather and both parties.
				@param[in,out] context The effect context containing the move data and receiving the weather multiplier.
				@param[in] provider The registry provider, which is not used by this handler.
				@param[in] randomSources The random sources used for battle-related numeric types.
				@post @p context contains the nullification multiplier when a party member has Air Lock or Cloud Nine, or the weather- and
				 type-specific multiplier otherwise.
				@since 0.9.8
				@version 0.12.53
			*/
			void apply(BattleState &state, EffectContext &context, ATTR_MAYBE_UNUSED const RegistryProvider &provider,
					   ATTR_MAYBE_UNUSED const BattleRandomSources &randomSources) const override;

		private:
			/*! @brief Applies the weather multipliers that apply to the current move and weather condition.
				@param[in,out] state The battle state containing the active weather and both parties.
				@param[in,out] context The effect context containing the move data and receiving the weather multiplier.
				@post @p context contains the nullification multiplier when a party member has Air Lock or Cloud Nine, or the weather- and
				 type-specific multiplier otherwise.
				@since 0.12.53
				@version 0.12.53
			*/
			static void applyMultipliers(BattleState &state, EffectContext &context)
			{
				const bool isRainy{std::ranges::contains(state.mWeatherIDs, toWeatherID(BuiltinWeatherID::Rain))};
				const bool isHeavyRain{std::ranges::contains(state.mWeatherIDs, toWeatherID(BuiltinWeatherID::HeavyRain))};
				const bool isHarshSunlight{std::ranges::contains(state.mWeatherIDs, toWeatherID(BuiltinWeatherID::HarshSunlight))};
				const bool isExtremelyHarshSunlight{
					std::ranges::contains(state.mWeatherIDs, toWeatherID(BuiltinWeatherID::ExtremelyHarshSunlight)),
				};

				const double fireMoveInRainMultiplier{
					isRainy ? FIRE_MOVE_IN_RAIN_MULTIPLIER_VALUE : FIRE_MOVE_IN_HEAVY_RAIN_MULTIPLIER_VALUE,
				};
				const double fireMoveInSunMultiplier{
					isHarshSunlight ? FIRE_MOVE_IN_HARSH_SUNLIGHT_MULTIPLIER_VALUE : FIRE_MOVE_IN_EXTREMELY_HARSH_SUNLIGHT_MULTIPLIER_VALUE,
				};
				const double waterMoveInSunMultiplier{
					isHarshSunlight ? WATER_MOVE_IN_HARSH_SUNLIGHT_MULTIPLIER_VALUE
									: WATER_MOVE_IN_EXTREMELY_HARSH_SUNLIGHT_MULTIPLIER_VALUE,
				};
				const double waterMoveInRainMultiplier{
					isRainy ? WATER_MOVE_IN_RAIN_MULTIPLIER_VALUE : WATER_MOVE_IN_HEAVY_RAIN_MULTIPLIER_VALUE,
				};
				const bool isWaterTypeMove{context.mMoveTypeID == toTypeID(BuiltinTypeID::Water)};
				const bool isFireTypeMove{context.mMoveTypeID == toTypeID(BuiltinTypeID::Fire)};

				const bool isHydroSteamMove{context.mMoveID == toMoveID(BuiltinMoveID::HydroSteam)};

				const bool doesHydroSteamApply{isHydroSteamMove && !isExtremelyHarshSunlight};

				if (isRainy || isHeavyRain)
				{
					if (isWaterTypeMove)
					{
						context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Weather), waterMoveInRainMultiplier);
						return;
					}

					if (isFireTypeMove)
					{
						context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Weather), fireMoveInRainMultiplier);
						return;
					}

					context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Weather), WEATHER_NULLIFICATION_MULTIPLIER_VALUE);
					return;
				}

				if (isHarshSunlight || isExtremelyHarshSunlight)
				{
					if (isFireTypeMove)
					{
						context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Weather), fireMoveInSunMultiplier);
						return;
					}

					if (doesHydroSteamApply)
					{
						context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Weather),
											  HYDRO_STEAM_MOVE_IN_HARSH_SUNLIGHT_MULTIPLIER_VALUE);
						return;
					}

					if (isWaterTypeMove)
					{
						context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Weather), waterMoveInSunMultiplier);
						return;
					}

					context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Weather), WEATHER_NULLIFICATION_MULTIPLIER_VALUE);
					return;
				}

				context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Weather), WEATHER_NULLIFICATION_MULTIPLIER_VALUE);
			}
	};

	/*! @brief Applies the weather damage multiplier to an effect context.
		@param[in,out] state The battle state containing the active weather and both parties.
		@param[in,out] context The effect context containing the move data and receiving the weather multiplier.
		@param[in] provider The registry provider, which is not used by this handler.
		@param[in] randomSources The random sources used for battle-related numeric types.
		@post @p context contains the nullification multiplier when a party member nullifies weather, or the weather- and type-specific
		 multiplier otherwise.
		@note The active multipliers remain unchanged when the weather does not affect the move.
		@since 0.10.0
		@version 0.12.9
	*/
	inline void applyWeather(BattleState &state, EffectContext &context, const RegistryProvider &provider,
							 const BattleRandomSources &randomSources = {})
	{
		WeatherHandler{}.apply(state, context, provider, randomSources);
	}
} // namespace PocketCore::Effect

#endif
