/*! @file weatherRegistry.h
	@brief Provides fixed-capacity storage and lookup for built-in and user-defined weathers.
	@date 07/27/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_REGISTRY_WEATHER_REGISTRY_H
#define INCLUDE_REGISTRY_WEATHER_REGISTRY_H

#include <optional>
#include <span>
#include <string_view>

#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Effect/builtInEffectID.h"
#include "Registry/fixedMetadataRegistry.h"
#include "Weather/builtInWeatherID.h"
#include "Weather/constants.h"
#include "Weather/weatherID.h"
#include "Weather/weatherMeta.h"

namespace PocketCore::Registry::Weather
{
	using PocketCore::Configuration::MAX_WEATHERS;
	using PocketCore::Core::us;
	using PocketCore::Effect::BuiltinEffectID;
	using PocketCore::Registry::FixedMetadataRegistry;
	using PocketCore::Weather::BuiltinWeatherID;
	using PocketCore::Weather::toWeatherID;
	using PocketCore::Weather::WeatherID;
	using PocketCore::Weather::WeatherMeta;

	/*! @class WeatherRegistry Registry/weatherRegistry.h
		@brief Stores built-in and user-defined weather metadata in fixed-capacity storage.
		@details Built-in weathers are registered during construction with IDs derived from @ref BuiltinWeatherID. Configuration code may
	   append, replace, or remove entries through the low-level mutators while battle-time callers use allocation-free lookup operations.
		@note Lookup operations are O(n), where n is bounded by @ref MAX_WEATHERS.
		@date 07/27/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	class WeatherRegistry : private FixedMetadataRegistry<WeatherMeta, WeatherID, MAX_WEATHERS, &WeatherMeta::mWeatherID>
	{
		private:
			using Base = FixedMetadataRegistry<WeatherMeta, WeatherID, MAX_WEATHERS, &WeatherMeta::mWeatherID>;

		public:
			// LCOV_EXCL_START - If the built in additions fail, the program wouldn't work anyway
			/*! @brief Constructs a registry populated with every @ref BuiltinWeatherID. */
			ATTR_NOINLINE explicit constexpr WeatherRegistry()
				: Base{static_cast<us>(toWeatherID(BuiltinWeatherID::ShadowyAura).getValue() + 1U)}
			{
				addBuiltin({.mName = PocketCore::Weather::WEATHER_NAME_NONE, .mWeatherID = toWeatherID(BuiltinWeatherID::None)});
				addBuiltin({
					.mName = PocketCore::Weather::WEATHER_NAME_HARSH_SUNLIGHT,
					.mWeatherID = toWeatherID(BuiltinWeatherID::HarshSunlight),
				});
				addBuiltin({
					.mName = PocketCore::Weather::WEATHER_NAME_RAIN,
					.mWeatherID = toWeatherID(BuiltinWeatherID::Rain),
				});
				addBuiltin({
					.mName = PocketCore::Weather::WEATHER_NAME_SANDSTORM,
					.mWeatherID = toWeatherID(BuiltinWeatherID::Sandstorm),
				});
				addBuiltin({
					.mName = PocketCore::Weather::WEATHER_NAME_HAIL,
					.mWeatherID = toWeatherID(BuiltinWeatherID::Hail),
				});
				addBuiltin({
					.mName = PocketCore::Weather::WEATHER_NAME_SNOW,
					.mWeatherID = toWeatherID(BuiltinWeatherID::Snow),
				});
				addBuiltin({
					.mName = PocketCore::Weather::WEATHER_NAME_FOG,
					.mWeatherID = toWeatherID(BuiltinWeatherID::Fog),
				});
				addBuiltin({
					.mName = PocketCore::Weather::WEATHER_NAME_EXTREMELY_HARSH_SUNLIGHT,
					.mWeatherID = toWeatherID(BuiltinWeatherID::ExtremelyHarshSunlight),
				});
				addBuiltin({
					.mName = PocketCore::Weather::WEATHER_NAME_HEAVY_RAIN,
					.mWeatherID = toWeatherID(BuiltinWeatherID::HeavyRain),
				});
				addBuiltin({
					.mName = PocketCore::Weather::WEATHER_NAME_STRONG_WINDS,
					.mWeatherID = toWeatherID(BuiltinWeatherID::StrongWinds),
				});
				addBuiltin({
					.mName = PocketCore::Weather::WEATHER_NAME_SHADOWY_AURA,
					.mWeatherID = toWeatherID(BuiltinWeatherID::ShadowyAura),
				});
			}

			// LCOV_EXCL_STOP

			using Base::decrementAmountRegistered;
			using Base::eraseEntry;
			using Base::findIndexByID;
			using Base::getAmountRegistered;
			using Base::getEntry;
			using Base::getID;
			using Base::getMetadata;
			using Base::getName;
			using Base::getNextID;
			using Base::getRegisteredEntries;
			using Base::hasEntry;
			using Base::incrementAmountRegistered;
			using Base::incrementNextID;
			using Base::setAmountRegistered;
			using Base::setEntry;
			using Base::setNextID;

			/*! @brief Looks up weather metadata by stable ID.
				@param[in] weatherID The stable weather identifier.
				@return A non-owning pointer to metadata if registered, or nullptr otherwise. The pointer remains valid until replacement or
			   registry destruction.
			*/
			ATTR_NODISCARD constexpr const WeatherMeta *getWeatherMetadata(const WeatherID weatherID) const
			{
				return getMetadata(weatherID);
			}

			/*! @brief Looks up an weather ID by display name.
				@param[in] name The case-sensitive display name.
				@return The stable ID if registered, or std::nullopt otherwise.
			*/
			ATTR_NODISCARD constexpr const std::optional<WeatherID> getWeatherID(const std::string_view &name) const
			{
				return getID(name);
			}

			/*! @brief Looks up an weather display name by stable ID.
				@param[in] weatherID The stable weather identifier.
				@return The display name if registered, or std::nullopt otherwise.
			*/
			ATTR_NODISCARD constexpr const std::optional<std::string_view> getWeatherName(const WeatherID weatherID) const
			{
				return getName(weatherID);
			}

			/*! @brief Returns all currently registered weather definitions.
				@return A read-only span that remains valid until the registry is mutated or destroyed.
			*/
			ATTR_NODISCARD constexpr const std::span<const WeatherMeta> getRegisteredWeathers() const noexcept
			{
				return getRegisteredEntries();
			}

			/*! @brief Returns the next stable ID assigned to a custom weather.
				@return The underlying numeric value of the next weather ID.
			*/
			ATTR_NODISCARD constexpr us getNextWeatherID() const noexcept
			{
				return getNextID();
			}

			/*! @brief Finds an internal array index by stable weather ID.
				@param[in] weatherID The stable weather identifier.
				@return The internal index if registered, or std::nullopt otherwise.
			*/
			ATTR_NODISCARD constexpr const std::optional<us> findIndexByWeatherID(const WeatherID weatherID) const
			{
				return findIndexByID(weatherID);
			}

			/*! @brief Checks whether an weather name is registered.
				@param[in] name The case-sensitive display name.
				@return True if the name is registered, otherwise false.
			*/
			ATTR_NODISCARD constexpr bool hasWeather(const std::string_view &name) const
			{
				return hasEntry(name);
			}

			/*! @brief Checks whether an weather ID is registered.
				@param[in] weatherID The stable weather identifier.
				@return True if the ID is registered, otherwise false.
			*/
			ATTR_NODISCARD constexpr bool hasWeather(const WeatherID weatherID) const
			{
				return hasEntry(weatherID);
			}

			/*! @brief Sets the next custom weather ID counter.
				@param[in] nextID The next underlying ID value.
			*/
			constexpr void setNextWeatherID(const us nextID) noexcept
			{
				setNextID(nextID);
			}

			/*! @brief Increments the next custom weather ID counter. */
			constexpr void incrementNextWeatherID() noexcept
			{
				incrementNextID();
			}
	};
} // namespace PocketCore::Registry::Weather

#endif
