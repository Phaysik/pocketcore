/*! @file weather.h
	@brief Contains the weather status
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_WEATHER_WEATHER_H
#define INCLUDE_WEATHER_WEATHER_H

#include "Core/typedefs.h"

namespace PocketCore::Status
{
	enum class Weathers : PocketCore::Core::ub
	{
		HarshSunlight,
		Rain,
		Sandstorm,
		Hail,
		Snow,
		Fog,
		ExtremelyHarshSunlight,
		HeavyRain,
		StrongWinds,
		ShadowyAura,
	};
} // namespace PocketCore::Status

#endif
