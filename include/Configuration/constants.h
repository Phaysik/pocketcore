/*! @file constants.h
	@brief Contains the function declarations for creating a Detailed file description
	@date 03/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_CONFIGURATION_CONSTANTS_H
#define INCLUDE_CONFIGURATION_CONSTANTS_H

#include <string_view>

#include "Core/typedefs.h"

namespace Pokemon::Configuration
{
	constexpr Pokemon::Core::ub MAX_TYPES{20};
	constexpr std::string_view NO_LOGGING_FAILURE{"There was no logging failure."};

	/*! @enum RegistryError constants.h
		@showenumvalues
		@brief Describes the kinds of errors that can occur when mutating the type registry.
	*/
	enum class RegistryError : Pokemon::Core::ub
	{
		MaxCapacity,	 /*!< The registry has reached its maximum number of types. */
		DuplicateType,	 /*!< A type with the given name already exists. */
		TypeNotFound,	 /*!< No type matching the input was found. */
		MatchupMismatch, /*!< The number of provided matchup entries does not match the registered count. */
		BatchMismatch,	 /*!< The number of matchup arrays does not match the number of names. */
	};

	/*! @struct RegistryErrorInfo constants.h
		@brief A contextual error carrying both the error kind and the identifier that caused it.
		@details Returned by @ref Configuration methods to provide programmatic access to the offending type name or the reason for failure
	   without requiring the caller to enable logging.
	*/
	struct RegistryErrorInfo
	{
			constexpr RegistryErrorInfo(const RegistryError errorKind, const std::string_view errorContext) noexcept
				: mKind{errorKind}, mErrorName{errorKindToString()}, mContext{errorContext}
			{}

			constexpr RegistryErrorInfo(const RegistryError errorKind, const std::string_view errorContext,
										const std::string_view logFailure) noexcept
				: mKind{errorKind}, mErrorName{errorKindToString()}, mContext{errorContext}, mLoggingFailure{logFailure}
			{}

			// NOLINTBEGIN(misc-non-private-member-variables-in-classes)

			/*! @var kind
				@brief The category of the error.
			*/
			RegistryError mKind{};

			std::string_view mErrorName{};

			/*! @var context
				@brief The display name or identifier that triggered the error, or empty if not applicable.
			*/
			std::string_view mContext{};

			/*! @var mLoggingFailure
				@brief The logging failure message if the Logger call itself failed, or empty if logging succeeded.
			*/
			std::string_view mLoggingFailure{NO_LOGGING_FAILURE};

			// NOLINTEND(misc-non-private-member-variables-in-classes)

		private:
			/*! @brief Convert a @ref RegistryError enum value to a human-readable name.
				@param err The enum value to convert.
				@return A string_view containing the enum name suitable for logging and error messages.
			*/
			[[nodiscard]] constexpr std::string_view errorKindToString() noexcept
			{
				switch (mKind)
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
					case RegistryError::BatchMismatch:
						mErrorName = "BatchMismatch";
						break;
					default:
						mErrorName = "UnknownRegistryError";
						break;
				}

				return mErrorName;
			}
	};

	/*! @enum UnspecifiedMatchup constants.h
		@showenumvalues
		@brief Controls how the name-keyed @ref Configuration::addType overload treats matchups not explicitly provided.
	*/
	enum class UnspecifiedMatchup : Pokemon::Core::ub
	{
		Neutral,	/*!< Fill unspecified matchups with @ref Pokemon::Types::TypeEffectiveness::E (neutral / 1x). */
		NotDefined, /*!< Fill unspecified matchups with @ref Pokemon::Types::TypeEffectiveness::NOT_DEFINED. */
	};
} // namespace Pokemon::Configuration

#endif