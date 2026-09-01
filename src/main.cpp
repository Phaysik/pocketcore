/*! @file main.cpp
	@brief Contains the function definitions for creating a main
	@date 09/01/2026
	@since 0.1.0
	@version 0.12.14
	@author Matthew Moore
*/

#include <array>
#include <cstdlib>
#include <iostream>
#include <string_view>

#include "Ability/abilityID.h"
#include "Ability/builtInAbilityID.h"
#include "Battle/battleAction.h"
#include "Battle/battleEngine.h"
#include "Configuration/abilityRegistryConfiguration.h"
#include "Configuration/effectRegistryConfiguration.h"
#include "Configuration/itemRegistryConfiguration.h"
#include "Configuration/moveRegistryConfiguration.h"
#include "Configuration/multiplierRegistryConfiguration.h"
#include "Configuration/natureRegistryConfiguration.h"
#include "Configuration/pokemonRegistryConfiguration.h"
#include "Configuration/statusRegistryConfiguration.h"
#include "Configuration/terrainRegistryConfiguration.h"
#include "Configuration/typeRegistryConfiguration.h"
#include "Configuration/weatherRegistryConfiguration.h"
#include "Item/builtInItemID.h"
#include "Item/itemID.h"
#include "Move/builtInMoveID.h"
#include "Pokemon/pokemon.h"
#include "Registry/effectRegistry.h"
#include "Registry/registryProvider.h"
#include "Types/builtInTypeID.h"
#include "Utility/Debug/Logging/constants.h"
#include "Utility/Debug/Logging/logger.h"

/*! @brief The entry point for the program
	@since 0.1.0
	@version 0.12.14
	@author Matthew Moore
	@return int The status code of the program
*/
int main()
{
	using PocketCore::Ability::BuiltinAbilityID;
	using PocketCore::Ability::NO_ABILITY_ID;
	using PocketCore::Ability::toAbilityID;
	using PocketCore::Battle::BattleAction;
	using PocketCore::Battle::BattleEngine;
	using PocketCore::Battle::BattleTarget;
	using PocketCore::Battle::MoveAction;
	using PocketCore::Effect::Side;
	using PocketCore::Item::BuiltinItemID;
	using PocketCore::Item::NO_ITEM_ID;
	using PocketCore::Item::toItemID;
	using PocketCore::Move::BuiltinMoveID;
	using PocketCore::Move::toMoveID;
	using PocketCore::Pokemon::Pokemon;
	using PocketCore::Registry::Effect::EffectRegistry;
	using PocketCore::Registry::RegistryProvider;
	using PocketCore::Type::BuiltInTypeID;
	using PocketCore::Type::toTypeID;
	namespace Configuration = PocketCore::Configuration;
	namespace Logging = PocketCore::Utility::Debug::Logging;

	const bool initialized{Logging::Logger::initialize(Logging::LOGGING_LOGGER_NAME, Logging::LOGGING_FILE_NAME, true)};

	if (!initialized)
	{
		return EXIT_FAILURE;
	}

	const Configuration::TypeRegistryConfiguration typeRegistryConfig{};
	const Configuration::AbilityRegistryConfiguration abilityRegistryConfig{};
	const Configuration::ItemRegistryConfiguration itemRegistryConfig{};
	const Configuration::MoveRegistryConfiguration moveRegistryConfig{};
	const Configuration::StatusRegistryConfiguration statusRegistryConfig{};
	const Configuration::TerrainRegistryConfiguration terrainRegistryConfig{};
	const Configuration::WeatherRegistryConfiguration weatherRegistryConfig{};
	const Configuration::MultiplierRegistryConfiguration multiplierRegistryConfig{};
	const Configuration::EffectRegistryConfiguration effectRegistryConfig{};
	const Configuration::NatureRegistryConfiguration natureRegistryConfig{};
	const Configuration::PokemonRegistryConfiguration pokemonRegistryConfig{};

	const RegistryProvider registryProvider{
		.abilityRegistry = &abilityRegistryConfig.getRuntimeRegistry(),
		.moveRegistry = &moveRegistryConfig.getRuntimeRegistry(),
		.itemRegistry = &itemRegistryConfig.getRuntimeRegistry(),
		.typeRegistry = &typeRegistryConfig.getRuntimeRegistry(),
		.statusRegistry = &statusRegistryConfig.getRuntimeRegistry(),
		.weatherRegistry = &weatherRegistryConfig.getRuntimeRegistry(),
		.terrainRegistry = &terrainRegistryConfig.getRuntimeRegistry(),
		.multiplierRegistry = &multiplierRegistryConfig.getRuntimeRegistry(),
		.natureRegistry = &natureRegistryConfig.getRuntimeRegistry(),
		.pokemonRegistry = &pokemonRegistryConfig.getRuntimeRegistry(),
	};
	const EffectRegistry &effectRegistry{effectRegistryConfig.getRuntimeRegistry()};

	Pokemon pokemonA{
		"Feraligatr",
		100U,
		100U,
		100U,
		100U,
		100U,
		90U,
		50U,
		{toAbilityID(BuiltinAbilityID::Drizzle)},
		{toItemID(BuiltinItemID::CheriBerry)},
	};
	pokemonA.setMove(0U, toMoveID(BuiltinMoveID::Pound));
	pokemonA.setMaxPP(0U, 35U);
	pokemonA.setCurrentPP(0U, 35U);
	pokemonA.setType(0, toTypeID(BuiltInTypeID::Water));

	Pokemon pokemonB{
		"Charizard",
		100U,
		100U,
		100U,
		100U,
		100U,
		80U,
		50U,
		{toAbilityID(BuiltinAbilityID::Stench)},
		{toItemID(BuiltinItemID::ChestoBerry)},
	};
	pokemonB.setMove(0U, toMoveID(BuiltinMoveID::Pound));
	pokemonB.setMaxPP(0U, 35U);
	pokemonB.setCurrentPP(0U, 35U);
	pokemonB.setType(0, toTypeID(BuiltInTypeID::Fire));
	pokemonB.setType(1, toTypeID(BuiltInTypeID::Flying));

	const std::array<Pokemon *, 1> partyA{&pokemonA};
	const std::array<Pokemon *, 1> partyB{&pokemonB};
	BattleEngine engine{registryProvider, effectRegistry};

	if (!engine.startBattle(partyA, partyB).has_value())
	{
		return EXIT_FAILURE;
	}

	const std::array<BattleAction, 2> actions{
		MoveAction{
			.mSelectedTarget = BattleTarget{.mSide = Side::B, .mSlotIndex = 0U},
			.mSide = Side::A,
			.mUserSlotIndex = 0U,
			.mMoveSlotIndex = 0U,
		},
		MoveAction{
			.mSelectedTarget = BattleTarget{.mSide = Side::A, .mSlotIndex = 0U},
			.mSide = Side::B,
			.mUserSlotIndex = 0U,
			.mMoveSlotIndex = 0U,
		},
	};

	if (!engine.executeTurn(actions).has_value())
	{
		return EXIT_FAILURE;
	}

	std::cout << "PartyA[0]: " << partyA.at(0)->getHealth() << '\n';
	// std::cout << "PartyA[0]:\n";
	// printPokemonWithNames(std::cout, *partyA.at(0), registryProvider) << '\n';

	std::cout << "PartyB[0]: " << partyB.at(0)->getHealth() << '\n';
	// std::cout << "PartyB[0]:\n";
	// printPokemonWithNames(std::cout, *partyB.at(0), registryProvider) << '\n';

	return EXIT_SUCCESS;
}
