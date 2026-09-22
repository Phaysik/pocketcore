/*! @file main.cpp
	@brief Contains the function definitions for creating a main
	@date 09/17/2026
	@since 0.1.0
	@version 0.12.37
	@author Matthew Moore
*/

#include <array>
#include <cstdlib>
#include <iostream>
#include <memory>
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
#include "Effect/effectContext.h"
#include "Item/builtInItemID.h"
#include "Item/itemID.h"
#include "Move/builtInMoveID.h"
#include "Nature/builtInNatureID.h"
#include "Pokemon/builtInPokemonID.h"
#include "Pokemon/pokemon.h"
#include "Registry/effectRegistry.h"
#include "Registry/registryProvider.h"
#include "Ruleset/rulesetPolicy.h"
#include "Ruleset/rulesetPresets.h"
#include "Types/builtInTypeID.h"
#include "Utility/Debug/Logging/constants.h"
#include "Utility/Debug/Logging/logger.h"

/*! @brief The entry point for the program
	@since 0.1.0
	@version 0.12.37
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
	using PocketCore::Configuration::AbilityRegistryConfiguration;
	using PocketCore::Configuration::EffectRegistryConfiguration;
	using PocketCore::Configuration::ItemRegistryConfiguration;
	using PocketCore::Configuration::MoveRegistryConfiguration;
	using PocketCore::Configuration::MultiplierRegistryConfiguration;
	using PocketCore::Configuration::NATURE_STAT_BASE_MULTIPLIER;
	using PocketCore::Configuration::NatureRegistryConfiguration;
	using PocketCore::Configuration::PokemonRegistryConfiguration;
	using PocketCore::Configuration::StatusRegistryConfiguration;
	using PocketCore::Configuration::TerrainRegistryConfiguration;
	using PocketCore::Configuration::TypeRegistryConfiguration;
	using PocketCore::Configuration::WeatherRegistryConfiguration;
	using PocketCore::Effect::Side;
	using PocketCore::Item::BuiltinItemID;
	using PocketCore::Item::NO_ITEM_ID;
	using PocketCore::Item::toItemID;
	using PocketCore::Move::BuiltinMoveID;
	using PocketCore::Move::toMoveID;
	using PocketCore::Nature::BuiltinNatureID;
	using PocketCore::Nature::toNatureID;
	using PocketCore::Pokemon::BuiltinPokemonID;
	using PocketCore::Pokemon::Pokemon;
	using PocketCore::Pokemon::toPokemonID;
	using PocketCore::Registry::Effect::EffectRegistry;
	using PocketCore::Registry::RegistryProvider;
	using PocketCore::Ruleset::GEN9_BASE_GAME;
	using PocketCore::Ruleset::RulesetPolicy;
	using PocketCore::Type::BuiltinTypeID;
	using PocketCore::Type::toTypeID;
	namespace Logging = PocketCore::Utility::Debug::Logging;

	const bool initialized{Logging::Logger::initialize(Logging::LOGGING_LOGGER_NAME, Logging::LOGGING_FILE_NAME, true)};

	if (!initialized)
	{
		return EXIT_FAILURE;
	}

	std::unique_ptr<TypeRegistryConfiguration> typeRegistryConfig{std::make_unique<TypeRegistryConfiguration>()};
	std::unique_ptr<AbilityRegistryConfiguration> abilityRegistryConfig{std::make_unique<AbilityRegistryConfiguration>()};
	std::unique_ptr<ItemRegistryConfiguration> itemRegistryConfig{std::make_unique<ItemRegistryConfiguration>()};
	std::unique_ptr<MoveRegistryConfiguration> moveRegistryConfig{std::make_unique<MoveRegistryConfiguration>()};
	std::unique_ptr<StatusRegistryConfiguration> statusRegistryConfig{std::make_unique<StatusRegistryConfiguration>()};
	std::unique_ptr<TerrainRegistryConfiguration> terrainRegistryConfig{std::make_unique<TerrainRegistryConfiguration>()};
	std::unique_ptr<WeatherRegistryConfiguration> weatherRegistryConfig{std::make_unique<WeatherRegistryConfiguration>()};
	std::unique_ptr<MultiplierRegistryConfiguration> multiplierRegistryConfig{std::make_unique<MultiplierRegistryConfiguration>()};
	std::unique_ptr<EffectRegistryConfiguration> effectRegistryConfig{std::make_unique<EffectRegistryConfiguration>()};
	std::unique_ptr<NatureRegistryConfiguration> natureRegistryConfig{std::make_unique<NatureRegistryConfiguration>()};
	std::unique_ptr<PokemonRegistryConfiguration> pokemonRegistryConfig{std::make_unique<PokemonRegistryConfiguration>()};

	const RegistryProvider registryProvider{
		.abilityRegistry = &abilityRegistryConfig->getRuntimeRegistry(),
		.moveRegistry = &moveRegistryConfig->getRuntimeRegistry(),
		.itemRegistry = &itemRegistryConfig->getRuntimeRegistry(),
		.typeRegistry = &typeRegistryConfig->getRuntimeRegistry(),
		.statusRegistry = &statusRegistryConfig->getRuntimeRegistry(),
		.weatherRegistry = &weatherRegistryConfig->getRuntimeRegistry(),
		.terrainRegistry = &terrainRegistryConfig->getRuntimeRegistry(),
		.multiplierRegistry = &multiplierRegistryConfig->getRuntimeRegistry(),
		.natureRegistry = &natureRegistryConfig->getRuntimeRegistry(),
		.pokemonRegistry = &pokemonRegistryConfig->getRuntimeRegistry(),
	};
	const EffectRegistry &effectRegistry{effectRegistryConfig->getRuntimeRegistry()};

	Pokemon pokemonA{
		toPokemonID(BuiltinPokemonID::Blastoise),
		"Blastoise",
		{
			.mMaxHealth = 100U,
			.mAttack = 100U,
			.mDefense = 100U,
			.mSpAttack = 100U,
			.mSpDefense = 100U,
			.mSpeed = 90U,
		},
		50U,
		{toAbilityID(BuiltinAbilityID::Drizzle)},
		{toItemID(BuiltinItemID::CheriBerry)},
		{toTypeID(BuiltinTypeID::Water)},
		{toNatureID(BuiltinNatureID::Hardy)},
		{{{NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER,
		   NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER}}},
		{},
		{},
	};
	pokemonA.setMoveID(0U, toMoveID(BuiltinMoveID::Pound));
	pokemonA.setMaxPP(0U, 35U);
	pokemonA.setCurrentPP(0U, 35U);
	pokemonA.setTypeID(0, toTypeID(BuiltinTypeID::Water));

	Pokemon pokemonB{
		toPokemonID(BuiltinPokemonID::Charizard),
		"Charizard",
		{
			.mMaxHealth = 100U,
			.mAttack = 100U,
			.mDefense = 100U,
			.mSpAttack = 100U,
			.mSpDefense = 100U,
			.mSpeed = 80U,
		},
		50U,
		{toAbilityID(BuiltinAbilityID::Stench)},
		{toItemID(BuiltinItemID::ChestoBerry)},
		{toTypeID(BuiltinTypeID::Fire), toTypeID(BuiltinTypeID::Flying)},
		{toNatureID(BuiltinNatureID::Hardy)},
		{{{NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER,
		   NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER}}},
		{},
		{},
	};
	pokemonB.setMoveID(0U, toMoveID(BuiltinMoveID::Pound));
	pokemonB.setMaxPP(0U, 35U);
	pokemonB.setCurrentPP(0U, 35U);
	pokemonB.setTypeID(0, toTypeID(BuiltinTypeID::Fire));
	pokemonB.setTypeID(1, toTypeID(BuiltinTypeID::Flying));

	const std::array<Pokemon *, 1> partyA{&pokemonA};
	const std::array<Pokemon *, 1> partyB{&pokemonB};
	BattleEngine engine{registryProvider, effectRegistry};
	RulesetPolicy ruleset{GEN9_BASE_GAME};

	if (!engine.startBattle(partyA, partyB, ruleset).has_value())
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
