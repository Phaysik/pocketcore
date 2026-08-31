/*! @file weatherRegistryConfiguration.h
	@brief Declares the user-facing facade for configuring weather metadata.
	@date 08/27/2026
	@since 0.8.0
	@version 0.12.8
	@author Matthew Moore
*/

#ifndef INCLUDE_CONFIGURATION_WEATHER_REGISTRY_CONFIGURATION_H
#define INCLUDE_CONFIGURATION_WEATHER_REGISTRY_CONFIGURATION_H

#include <expected>
#include <optional>
#include <span>
#include <string_view>

#include "Configuration/constants.h"
#include "Configuration/fixedMetadataRegistryConfiguration.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Registry/weatherRegistry.h"
#include "Weather/weatherID.h"
#include "Weather/weatherMeta.h"

namespace PocketCore::Configuration
{
	using PocketCore::Core::us;
	using PocketCore::Registry::Weather::WeatherRegistry;
	using PocketCore::Weather::WeatherID;
	using PocketCore::Weather::WeatherMeta;

	namespace Detail
	{
		/*! @struct WeatherRegistryConfigurationPolicy Configuration/weatherRegistryConfiguration.h
			@brief Policy class providing error codes and display strings for weather registry configuration.
			@details Encapsulates the weather-specific error categories and display names used by the generic
			 @ref FixedMetadataRegistryConfiguration template to report validation and lookup failures with
			 domain-specific terminology.
			@date 08/26/2026
			@since 0.8.0
			@version 0.12.7
			@author Matthew Moore
		*/
		struct WeatherRegistryConfigurationPolicy
		{
			public:
				/*! @brief The display name of the configuration system. */
				static constexpr std::string_view configurationName{"WeatherRegistryConfiguration"};

				/*! @brief The singular entity type managed by this configuration. */
				static constexpr std::string_view entityName{"weather"};

				/*! @brief The error code returned when a duplicate ability name is registered. */
				static constexpr RegistryError duplicateError{RegistryError::DuplicateWeather};

				/*! @brief The error code returned when an ability lookup fails. */
				static constexpr RegistryError notFoundError{RegistryError::WeatherNotFound};
		};
	} // namespace Detail

	/*! @class WeatherRegistryConfiguration Configuration/weatherRegistryConfiguration.h
		@brief Provides validated user customization over an internal weather registry.
		@details Supports lookup, addition, batch addition, trigger replacement, renaming, and removal. Custom IDs are assigned
	   monotonically and are not reused after removal. Batch additions provide all-or-nothing semantics.
		@date 08/27/2026
		@since 0.8.0
		@version 0.12.8
		@author Matthew Moore
	*/
	class WeatherRegistryConfiguration
		: private FixedMetadataRegistryConfiguration<WeatherRegistry, WeatherMeta, WeatherID, MAX_WEATHERS, &WeatherMeta::mWeatherID,
													 Detail::WeatherRegistryConfigurationPolicy>
	{
		private:
			using Base = FixedMetadataRegistryConfiguration<WeatherRegistry, WeatherMeta, WeatherID, MAX_WEATHERS, &WeatherMeta::mWeatherID,
															Detail::WeatherRegistryConfigurationPolicy>;

		public:
			/*! @brief Constructs a configuration containing all built-in weathers.
				@since 0.8.0
				@version 0.8.0
			 */
			constexpr WeatherRegistryConfiguration() = default;

			using Base::getAmountRegistered;

			/*! @brief Returns read-only access to the configured runtime weather registry.
				@return A reference that remains valid for the lifetime of this configuration.
				@since 0.10.3
				@version 0.12.8
			*/
			ATTR_NODISCARD constexpr const WeatherRegistry &getRuntimeRegistry() const noexcept
			{
				return getRegistry();
			}

			/*! @brief Looks up complete metadata by stable weather ID.
				@param[in] weatherID The built-in or custom stable identifier.
				@return A non-owning pointer to metadata if registered, or nullptr otherwise. The pointer remains valid until replacement or
			   configuration destruction.
				@since 0.8.0
				@version 0.8.0
			*/
			ATTR_NODISCARD constexpr const WeatherMeta *getWeatherMetadata(const WeatherID weatherID) const
			{
				return getMetadata(weatherID);
			}

			/*! @brief Looks up a stable weather ID by display name.
				@param[in] name The case-sensitive display name.
				@return The stable ID if registered, or std::nullopt otherwise.
				@since 0.8.0
				@version 0.8.0
			*/
			ATTR_NODISCARD constexpr const std::optional<WeatherID> getWeatherID(const std::string_view &name) const
			{
				return getID(name);
			}

			/*! @brief Looks up a display name by stable weather ID.
				@param[in] weatherID The built-in or custom stable identifier.
				@return The display name if registered, or std::nullopt otherwise.
				@since 0.8.0
				@version 0.8.0
			*/
			ATTR_NODISCARD constexpr const std::optional<std::string_view> getWeatherName(const WeatherID weatherID) const
			{
				return getName(weatherID);
			}

			/*! @brief Returns all currently registered weather definitions.
				@return A read-only span that remains valid until mutation or destruction.
				@since 0.8.0
				@version 0.8.0
			*/
			ATTR_NODISCARD constexpr const std::span<const WeatherMeta> getRegisteredWeathers() const noexcept
			{
				return getRegisteredEntries();
			}

			/*! @brief Checks whether an weather name is registered.
				@param[in] name The case-sensitive display name.
				@return True if the name is registered, otherwise false.
				@since 0.8.0
				@version 0.8.0
			*/
			ATTR_NODISCARD constexpr bool hasWeather(const std::string_view &name) const
			{
				return hasEntry(name);
			}

			/*! @brief Checks whether an weather ID is registered.
				@param[in] weatherID The built-in or custom stable identifier.
				@return True if the ID is registered, otherwise false.
				@since 0.8.0
				@version 0.8.0
			*/
			ATTR_NODISCARD constexpr bool hasWeather(const WeatherID weatherID) const
			{
				return hasEntry(weatherID);
			}

			/*! @brief Registers one user-defined weather and assigns a stable ID.
				@param[in] weatherMeta The name and trigger metadata to copy into the registry.
				@return The assigned ID on success, or @ref RegistryErrorInfo on duplicate name or exhausted capacity.
				@since 0.8.0
				@version 0.8.0
			*/
			ATTR_NODISCARD std::expected<WeatherID, RegistryErrorInfo> addWeather(const WeatherMeta &weatherMeta);

			/*! @brief Registers multiple weathers atomically.
				@details Restores the complete prior registry state if any definition fails validation.
				@param[in] weatherMetas The weather definitions to register in order.
				@return Void on success, or the first @ref RegistryErrorInfo on failure.
				@since 0.8.0
				@version 0.8.0
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> addWeathers(const std::span<const WeatherMeta> &weatherMetas);

			/*! @brief Renames an weather without changing its other metadata.
				@details @p newName is stored as a non-owning view and its backing storage must remain valid while registered.
				@param[in] oldName The currently registered display name.
				@param[in] newName The unique replacement display name.
				@return Void on success, or @ref RegistryErrorInfo if the source is absent or target name is already registered.
				@since 0.8.0
				@version 0.8.0
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> renameWeather(const std::string_view &oldName,
																				const std::string_view &newName);

			/*! @brief Replaces all weather metadata for an weather selected by stable ID.
				@param[in] weatherName The registered display name.
				@param[in] weatherMeta The metadata to copy into the registry.
				@return Void on success, or @ref RegistryErrorInfo if the weather is not registered.
				@since 0.8.0
				@version 0.8.0
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> updateWeather(const std::string_view &weatherName,
																				const WeatherMeta &weatherMeta);

			/*! @overload std::expected<void, RegistryErrorInfo> updateWeather(WeatherID, const WeatherMeta &weatherMeta)
				@brief Replaces all weather metadata for an weather selected by stable ID.
				@param[in] weatherID The built-in or custom stable identifier.
				@param[in] weatherMeta The metadata to copy into the registry.
				@return Void on success, or @ref RegistryErrorInfo if the weather is not registered.
				@since 0.8.0
				@version 0.8.0
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> updateWeather(const WeatherID weatherID, const WeatherMeta &weatherMeta);

			/*! @brief Removes an weather by display name.
				@param[in] weatherName The registered display name.
				@return The removed stable ID on success, or @ref RegistryErrorInfo if no matching weather exists.
				@since 0.8.0
				@version 0.8.0
			*/
			ATTR_NODISCARD std::expected<WeatherID, RegistryErrorInfo> removeWeather(const std::string_view &weatherName);

			/*! @overload std::expected<WeatherID, RegistryErrorInfo> removeWeather(WeatherID)
				@brief Removes an weather by stable ID.
				@param[in] weatherID The built-in or custom stable identifier.
				@return The removed stable ID on success, or @ref RegistryErrorInfo if no matching weather exists.
				@since 0.8.0
				@version 0.8.0
			*/
			ATTR_NODISCARD std::expected<WeatherID, RegistryErrorInfo> removeWeather(const WeatherID weatherID);
	};
} // namespace PocketCore::Configuration

#endif
