/*! @file weatherRegistryConfiguration.cpp
	@brief Defines validated user customization operations for the weather registry.
	@date 07/30/2026
	@since 0.8.1
	@version 0.8.1
	@author Matthew Moore
*/

#include "Configuration/weatherRegistryConfiguration.h"

#include <expected>
#include <span>
#include <string_view>

#include "Core/attributeMacros.h"
#include "Weather/weatherID.h"
#include "Weather/weatherMeta.h"

namespace PocketCore::Configuration
{
	using PocketCore::Weather::WeatherID;
	using PocketCore::Weather::WeatherMeta;

	ATTR_NODISCARD std::expected<WeatherID, RegistryErrorInfo> WeatherRegistryConfiguration::addWeather(const WeatherMeta &weatherMeta)
	{
		return addMetadata(weatherMeta);
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> WeatherRegistryConfiguration::addWeathers(
		const std::span<const WeatherMeta> &weatherMetas)
	{
		return addMetadataBatch(weatherMetas, [](const WeatherMeta &definition) { return WeatherMeta{definition}; });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> WeatherRegistryConfiguration::renameWeather(const std::string_view &oldName,
																									  const std::string_view &newName)
	{
		return renameMetadata(oldName, newName);
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> WeatherRegistryConfiguration::updateWeather(const std::string_view &weatherName,
																									  const WeatherMeta &weatherMeta)
	{
		return mutateMetadata(weatherName, "updateWeather", [&weatherMeta](WeatherMeta &metadata) { metadata = weatherMeta; });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> WeatherRegistryConfiguration::updateWeather(const WeatherID weatherID,
																									  const WeatherMeta &weatherMeta)
	{
		return mutateMetadata(weatherID, "updateWeather", [&weatherMeta](WeatherMeta &metadata) { metadata = weatherMeta; });
	}

	ATTR_NODISCARD std::expected<WeatherID, RegistryErrorInfo> WeatherRegistryConfiguration::removeWeather(
		const std::string_view &weatherName)
	{
		return removeMetadata(weatherName);
	}

	ATTR_NODISCARD std::expected<WeatherID, RegistryErrorInfo> WeatherRegistryConfiguration::removeWeather(const WeatherID weatherID)
	{
		return removeMetadata(weatherID);
	}
} // namespace PocketCore::Configuration
