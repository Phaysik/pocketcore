/*! @file constants.h
	@brief Defines configuration constants and error metadata used by the type registry.
	@date --/--/----
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
	using Pokemon::Core::ub;

	// MARK: Constexpr statements

	constexpr ub MAX_TYPES{20};
	constexpr std::string_view NO_LOGGING_FAILURE{"There was no logging failure."};

	// MARK: Helper Enums

	/*! @enum RegistryError Configuration/constants.h
		@brief Describes the kinds of errors that can occur when mutating the type registry.
		@details Each value represents a distinct failure mode used by @ref RegistryErrorInfo for programmatic error handling.
		@note All enum values should be handled exhaustively by callers mapping error behavior.
	*/
	enum class RegistryError : ub
	{
		MaxCapacity,	 /*!< The registry has reached its maximum number of types. */
		DuplicateType,	 /*!< A type with the given name already exists. */
		TypeNotFound,	 /*!< No type matching the input was found. */
		MatchupMismatch, /*!< The number of provided matchup entries does not match the registered count. */
		BatchMismatch,	 /*!< The number of matchup arrays does not match the number of names. */
	};

	/*! @enum UnspecifiedMatchup Configuration/constants.h
		@brief Controls how the name-keyed @ref Configuration::addType overload treats matchups not explicitly provided.
		@details Selects the default value assigned to omitted matchup entries when expanding a partial matchup definition.
	*/
	enum class UnspecifiedMatchup : ub
	{
		Neutral,	/*!< Fill unspecified matchups with @ref Pokemon::Types::TypeEffectiveness::E (neutral / 1x). */
		NotDefined, /*!< Fill unspecified matchups with @ref Pokemon::Types::TypeEffectiveness::NOT_DEFINED. */
	};

	// MARK: RegistryErrorInfo

	/*! @struct RegistryErrorInfo Configuration/constants.h
		@brief A contextual error carrying both the error kind and the identifier that caused it.
		@details Returned by @ref Configuration methods to provide programmatic access to the offending type name or the reason for failure
		without requiring the caller to enable logging.
		@note This type is a lightweight data carrier and stores non-owning string views.
	*/
	struct RegistryErrorInfo
	{
			// MARK: Constructors

			/*! @brief Initializes error info with an error category and context.
				@param[in] errorKind The registry error category.
				@param[in] errorContext The identifier or context associated with the failure.
			*/
			constexpr RegistryErrorInfo(const RegistryError errorKind, const std::string_view errorContext) noexcept
				: mKind{errorKind}, mErrorName{errorKindToString()}, mContext{errorContext}
			{}

			/*! @brief Initializes error info with an error category, context, and logging failure reason.
				@param[in] errorKind The registry error category.
				@param[in] errorContext The identifier or context associated with the failure.
				@param[in] logFailure A message describing why logging failed.
			*/
			constexpr RegistryErrorInfo(const RegistryError errorKind, const std::string_view errorContext,
										const std::string_view logFailure) noexcept
				: mKind{errorKind}, mErrorName{errorKindToString()}, mContext{errorContext}, mLoggingFailure{logFailure}
			{}

			// NOLINTBEGIN(misc-non-private-member-variables-in-classes)

			/*! @brief The category of the error. */
			RegistryError mKind{};

			std::string_view mErrorName{};

			/*! @brief The display name or identifier that triggered the error, or empty if not applicable. */
			std::string_view mContext{};

			/*! @var mLoggingFailure
				@brief The logging failure message if the Logger call itself failed, or a default success message.
			*/
			std::string_view mLoggingFailure{NO_LOGGING_FAILURE};

			// NOLINTEND(misc-non-private-member-variables-in-classes)

		private:
			// MARK: Private Member Function

			/*! @brief Converts @ref mKind to a human-readable name.
				@return A string view containing the enum name suitable for diagnostics and error messages.
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
} // namespace Pokemon::Configuration

#endif