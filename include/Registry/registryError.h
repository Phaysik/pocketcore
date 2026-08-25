/*! @file registryError.h
	@brief Defines registry error types and metadata.
	@date 05/01/2026
	@since x.x.x
	@version x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_REGISTRY_REGISTRY_ERROR_H
#define INCLUDE_REGISTRY_REGISTRY_ERROR_H

#include <string_view>

#include "Core/attributeMacros.h"
#include "Core/typedefs.h"

namespace PocketCore::Registry
{
	using PocketCore::Core::ub;

	inline constexpr std::string_view NO_LOGGING_FAILURE{"There was no logging failure."};

	/*! @enum RegistryError Configuration/constants.h
		@brief Describes the kinds of errors that can occur when mutating configurable registries.
		@details Each value represents a distinct failure mode used by @ref RegistryErrorInfo for programmatic error handling.
		@note All enum values should be handled exhaustively by callers mapping error behavior.
		@since 0.1.0
		@version 0.5.1
	*/
	enum class RegistryError : ub
	{
		MaxCapacity,		 /*!< The registry has reached its maximum number of entries. */
		DuplicateType,		 /*!< A type with the given name already exists. */
		TypeNotFound,		 /*!< No type matching the input was found. */
		MatchupMismatch,	 /*!< The number of provided matchup entries does not match the registered count. */
		DuplicateAbility,	 /*!< An ability with the given name already exists. */
		AbilityNotFound,	 /*!< No ability matching the input was found. */
		DuplicateItem,		 /*!< An item with the given name already exists. */
		ItemNotFound,		 /*!< No item matching the input was found. */
		DuplicateMove,		 /*!< A move with the given name already exists. */
		MoveNotFound,		 /*!< No move matching the input was found. */
		DuplicateStatus,	 /*!< A status with the given name already exists. */
		StatusNotFound,		 /*!< No status matching the input was found. */
		DuplicateWeather,	 /*!< A weather with the given name already exists. */
		WeatherNotFound,	 /*!< No weather matching the input was found. */
		DuplicateTerrain,	 /*!< A terrain with the given name already exists. */
		TerrainNotFound,	 /*!< No terrain matching the input was found. */
		DuplicateMultiplier, /*!< A multiplier with the given name already exists. */
		MultiplierNotFound,	 /*!< No multiplier matching the input was found. */
		DuplicateEffect,	 /*!< An effect with the given name already exists. */
		EffectNotFound,		 /*!< No effect matching the input was found. */
		DuplicateNature,	 /*!< A nature with the given name already exists. */
		NatureNotFound,		 /*!< No nature matching the input was found. */
		DuplicatePokemon,	 /*!< A Pokemon with the given name already exists. */
		PokemonNotFound,	 /*!< No Pokemon matching the input was found. */
	};

	/*! @enum UnspecifiedMatchup Configuration/constants.h
		@brief Controls how the name-keyed @ref Configuration::addType overload treats matchups not explicitly provided.
		@details Selects the default value assigned to omitted matchup entries when expanding a partial matchup definition.
		@since 0.1.0
		@version 0.2.0
	*/
	enum class UnspecifiedMatchup : ub
	{
		Neutral,	/*!< Fill unspecified matchups with @ref PocketCore::Types::TypeEffectiveness::E (neutral / 1x). */
		NotDefined, /*!< Fill unspecified matchups with @ref PocketCore::Types::TypeEffectiveness::NOT_DEFINED. */
	};

	/*! @struct RegistryErrorInfo Configuration/constants.h
		@brief A contextual error carrying both the error kind and the identifier that caused it.
		@details Returned by @ref Configuration methods to provide programmatic access to the offending entry name or the reason for failure
		without requiring the caller to enable logging.
		@note This type is a lightweight data carrier and stores non-owning string views.
		@since 0.1.0
		@version 0.5.1
	*/
	struct RegistryErrorInfo
	{
			// MARK: Constructors

			/*! @brief Initializes error info with an error category and context.
				@param[in] errorKind The registry error category.
				@param[in] errorContext The identifier or context associated with the failure.
				@since 0.1.0
				@version 0.5.1
			*/
			constexpr RegistryErrorInfo(const RegistryError errorKind, const std::string_view &errorContext) noexcept
				: mErrorName{errorKindToString(errorKind)}, mContext{errorContext}, mKind{errorKind}
			{}

			/*! @brief Initializes error info with an error category, context, and logging failure reason.
				@param[in] errorKind The registry error category.
				@param[in] errorContext The identifier or context associated with the failure.
				@param[in] logFailure A message describing why logging failed.
				@since 0.1.0
				@version 0.5.1
			*/
			constexpr RegistryErrorInfo(const RegistryError errorKind, const std::string_view &errorContext,
										const std::string_view &logFailure) noexcept
				: mErrorName{errorKindToString(errorKind)}, mContext{errorContext}, mLoggingFailure{logFailure}, mKind{errorKind}
			{}

			// NOLINTBEGIN(misc-non-private-member-variables-in-classes)

			std::string_view mErrorName{};

			/*! @brief The display name or identifier that triggered the error, or empty if not applicable. */
			std::string_view mContext{};

			/*! @brief The logging failure message if the Logger call itself failed, or a default success message. */
			std::string_view mLoggingFailure{NO_LOGGING_FAILURE};

			/*! @brief The category of the error. */
			RegistryError mKind{};

			// NOLINTEND(misc-non-private-member-variables-in-classes)

		private:
			/*! @brief Converts @ref mKind to a human-readable name.
				@return A string view containing the enum name suitable for diagnostics and error messages.
				@since 0.1.0
				@version 0.5.1
			*/
			ATTR_NODISCARD constexpr std::string_view errorKindToString(const RegistryError errorKind) noexcept
			{
				switch (errorKind) // LCOV_EXCL_BR
				{
					case RegistryError::MaxCapacity:
						mErrorName = "MaxCapacity";
						break;
					case RegistryError::DuplicateType:
						mErrorName = "DuplicateType";
						break;
					case RegistryError::TypeNotFound:
						mErrorName = "TypeNotFound";
						break;
					case RegistryError::MatchupMismatch:
						mErrorName = "MatchupMismatch";
						break;
					case RegistryError::DuplicateAbility:
						mErrorName = "DuplicateAbility";
						break;
					case RegistryError::AbilityNotFound:
						mErrorName = "AbilityNotFound";
						break;
					case RegistryError::DuplicateItem:
						mErrorName = "DuplicateItem";
						break;
					case RegistryError::ItemNotFound:
						mErrorName = "ItemNotFound";
						break;
					case RegistryError::DuplicateMove:
						mErrorName = "DuplicateMove";
						break;
					case RegistryError::MoveNotFound:
						mErrorName = "MoveNotFound";
						break;
					case RegistryError::DuplicateStatus:
						mErrorName = "DuplicateStatus";
						break;
					case RegistryError::StatusNotFound:
						mErrorName = "StatusNotFound";
						break;
					case RegistryError::DuplicateWeather:
						mErrorName = "DuplicateWeather";
						break;
					case RegistryError::WeatherNotFound:
						mErrorName = "WeatherNotFound";
						break;
					case RegistryError::DuplicateTerrain:
						mErrorName = "DuplicateTerrain";
						break;
					case RegistryError::TerrainNotFound:
						mErrorName = "TerrainNotFound";
						break;
					case RegistryError::DuplicateMultiplier:
						mErrorName = "DuplicateMultiplier";
						break;
					case RegistryError::MultiplierNotFound:
						mErrorName = "MultiplierNotFound";
						break;
					case RegistryError::DuplicateEffect:
						mErrorName = "DuplicateEffect";
						break;
					case RegistryError::EffectNotFound:
						mErrorName = "EffectNotFound";
						break;
					case RegistryError::DuplicateNature:
						mErrorName = "DuplicateNature";
						break;
					case RegistryError::NatureNotFound:
						mErrorName = "NatureNotFound";
						break;
					case RegistryError::DuplicatePokemon:
						mErrorName = "DuplicatePokemon";
						break;
					case RegistryError::PokemonNotFound:
						mErrorName = "PokemonNotFound";
						break;
					// LCOV_EXCL_START — Defensive: All enum values are handled, and the default case is unreachable, but this
					// silences compiler warnings about unhandled enum values.
					default:
						mErrorName = "UnknownError";
						break;
						// LCOV_EXCL_STOP
				}

				return mErrorName;
			}
	};
} // namespace PocketCore::Registry

#endif
