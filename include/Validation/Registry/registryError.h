/*! @file registryError.h
	@brief Defines registry error types and metadata.
	@date 10/13/2026
	@since 0.12.1
	@version 0.12.51
	@author Matthew Moore
*/

#ifndef INCLUDE_VALIDATION_REGISTRY_REGISTRY_ERROR_H
#define INCLUDE_VALIDATION_REGISTRY_REGISTRY_ERROR_H

#include "Core/typedefs.h"

namespace PocketCore::Validation::Registry
{
	using PocketCore::Core::ub;

	/*! @enum RegistryError
		@brief Describes the kinds of errors that can occur when mutating configurable registries.
		@note All enum values should be handled exhaustively by callers mapping error behavior.
		@since 0.1.0
		@version 0.12.51
	*/
	enum class RegistryError : ub
	{
		MaxCapacity,		 /*!< The registry has reached its maximum number of entries. */
		MissingRegistry,	 /*!< A registry was not found. */
		DuplicateMetadata,	 /*!< A metadata with the given name already exists. */
		MetadataNotFound,	 /*!< No metadata matching the input was found. */
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
		DuplicateLearnset,	 /*!< A Learnset with the given name already exists. */
		LearnsetNotFound,	 /*!< No Learnset matching the input was found. */
		DuplicateLocation,	 /*!< A location with the given name already exists. */
		LocationNotFound,	 /*!< No location matching the input was found. */
	};

	/*! @enum UnspecifiedMatchup
		@brief Controls how the name-keyed @ref Configuration::addType overload treats matchups not explicitly provided.
		@details Selects the default value assigned to omitted matchup entries when expanding a partial matchup definition.
		@since 0.1.0
		@version 0.12.51
	*/
	enum class UnspecifiedMatchup : ub
	{
		Neutral,	/*!< Fill unspecified matchups with @ref PocketCore::Type::TypeEffectiveness::E (neutral / 1x). */
		NotDefined, /*!< Fill unspecified matchups with @ref PocketCore::Type::TypeEffectiveness::NOT_DEFINED. */
	};
} // namespace PocketCore::Validation::Registry

#endif
