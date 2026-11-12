/*! @file pokemonRegistry.h
	@brief Provides fixed-capacity storage and lookup for built-in and user-defined pokemons.
	@date 11/12/2026
	@since 0.11.6
	@version 0.12.54
	@author Matthew Moore
*/

#ifndef INCLUDE_REGISTRY_POKEMON_REGISTRY_H
#define INCLUDE_REGISTRY_POKEMON_REGISTRY_H

#include <expected>
#include <optional>
#include <span>
#include <string_view>
#include <variant>

#include "Ability/abilityID.h"
#include "Ability/abilityMeta.h"
#include "Ability/builtInAbilityID.h"
#include "Battle/battleRandomSources.h"
#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Effect/builtInEffectID.h"
#include "EggGroup/builtInEggGroupID.h"
#include "Item/builtInItemID.h"
#include "Item/itemID.h"
#include "Item/itemMeta.h"
#include "Learnset/builtInLearnsetID.h"
#include "Learnset/learnsetID.h"
#include "Move/moveID.h"
#include "Move/moveMeta.h"
#include "Nature/natureID.h"
#include "Nature/natureMeta.h"
#include "Pokemon/builtInPokemonID.h"
#include "Pokemon/constants.h"
#include "Pokemon/pokemon.h"
#include "Pokemon/pokemonEncounter.h"
#include "Pokemon/pokemonID.h"
#include "Pokemon/pokemonInstantiationContext.h"
#include "Pokemon/pokemonMeta.h"
#include "Pokemon/pokemonStats.h"
#include "Registry/abilityRegistry.h"
#include "Registry/fixedMetadataRegistry.h"
#include "Registry/itemRegistry.h"
#include "Registry/learnsetRegistry.h"
#include "Registry/locationRegistry.h"
#include "Registry/moveRegistry.h"
#include "Registry/natureRegistry.h"
#include "Type/builtInTypeID.h"
#include "Validation/Ability/abilityError.h"
#include "Validation/Item/itemError.h"
#include "Validation/Move/moveError.h"
#include "Validation/Nature/natureError.h"
#include "Validation/Pokemon/pokemonError.h"
#include "Validation/Registry/registryError.h"

namespace PocketCore::Registry::Pokemon
{

	using PocketCore::Ability::AbilityClassification;
	using PocketCore::Ability::AbilityID;
	using PocketCore::Ability::AbilityMeta;
	using PocketCore::Ability::BuiltinAbilityID;
	using PocketCore::Ability::toAbilityID;
	using PocketCore::Battle::BattleRandomSources;
	using PocketCore::Configuration::MAX_ABILITIES_PER_POKEMON;
	using PocketCore::Configuration::MAX_ITEMS_PER_POKEMON;
	using PocketCore::Configuration::MAX_MOVES_PER_POKEMON;
	using PocketCore::Configuration::MAX_NATURES_PER_POKEMON;
	using PocketCore::Configuration::MAX_POKEMON;
	using PocketCore::Configuration::MIN_EV_STAT_VALUE;
	using PocketCore::Core::us;
	using PocketCore::Effect::BuiltinEffectID;
	using PocketCore::EggGroup::BuiltinEggGroupID;
	using PocketCore::EggGroup::toEggGroupID;
	using PocketCore::Item::BuiltinItemID;
	using PocketCore::Item::ItemID;
	using PocketCore::Item::ItemMeta;
	using PocketCore::Item::toItemID;
	using PocketCore::Learnset::BuiltinLearnsetID;
	using PocketCore::Learnset::LearnsetID;
	using PocketCore::Learnset::toLearnsetID;
	using PocketCore::Move::MoveID;
	using PocketCore::Move::MoveMeta;
	using PocketCore::Nature::NatureID;
	using PocketCore::Nature::NatureMeta;
	using PocketCore::Pokemon::BLASTOISE_BASE_STATS;
	using PocketCore::Pokemon::BLASTOISE_ENCOUNTERS;
	using PocketCore::Pokemon::BuiltinPokemonID;
	using PocketCore::Pokemon::BULBASAUR_BASE_STATS;
	using PocketCore::Pokemon::BULBASAUR_ENCOUNTERS;
	using PocketCore::Pokemon::CHARIZARD_BASE_STATS;
	using PocketCore::Pokemon::CHARIZARD_ENCOUNTERS;
	using PocketCore::Pokemon::CHARMANDER_BASE_STATS;
	using PocketCore::Pokemon::CHARMANDER_ENCOUNTERS;
	using PocketCore::Pokemon::CHARMELEON_BASE_STATS;
	using PocketCore::Pokemon::CHARMELEON_ENCOUNTERS;
	using PocketCore::Pokemon::DITTO_BASE_STATS;
	using PocketCore::Pokemon::DITTO_ENCOUNTERS;
	using PocketCore::Pokemon::IVYSAUR_BASE_STATS;
	using PocketCore::Pokemon::IVYSAUR_ENCOUNTERS;
	using PocketCore::Pokemon::Pokemon;
	using PocketCore::Pokemon::POKEMON_NAME_BLASTOISE;
	using PocketCore::Pokemon::POKEMON_NAME_BULBASAUR;
	using PocketCore::Pokemon::POKEMON_NAME_CHARIZARD;
	using PocketCore::Pokemon::POKEMON_NAME_CHARMANDER;
	using PocketCore::Pokemon::POKEMON_NAME_CHARMELEON;
	using PocketCore::Pokemon::POKEMON_NAME_DITTO;
	using PocketCore::Pokemon::POKEMON_NAME_IVYSAUR;
	using PocketCore::Pokemon::POKEMON_NAME_NONE;
	using PocketCore::Pokemon::POKEMON_NAME_SQUIRTLE;
	using PocketCore::Pokemon::POKEMON_NAME_VENUSAUR;
	using PocketCore::Pokemon::POKEMON_NAME_WARTORTLE;
	using PocketCore::Pokemon::POKEMON_STAT_COUNT;
	using PocketCore::Pokemon::PokemonGender;
	using PocketCore::Pokemon::PokemonID;
	using PocketCore::Pokemon::PokemonMeta;
	using PocketCore::Pokemon::PokemonOrigin;
	using PocketCore::Pokemon::PokemonSpec;
	using PocketCore::Pokemon::SetLevelEncounter;
	using PocketCore::Pokemon::SQUIRTLE_BASE_STATS;
	using PocketCore::Pokemon::SQUIRTLE_ENCOUNTERS;
	using PocketCore::Pokemon::toPokemonID;
	using PocketCore::Pokemon::VENUSAUR_BASE_STATS;
	using PocketCore::Pokemon::VENUSAUR_ENCOUNTERS;
	using PocketCore::Pokemon::WARTORTLE_BASE_STATS;
	using PocketCore::Pokemon::WARTORTLE_ENCOUNTERS;
	using PocketCore::Registry::Ability::AbilityRegistry;
	using PocketCore::Registry::FixedMetadataRegistry;
	using PocketCore::Registry::Item::ItemRegistry;
	using PocketCore::Registry::Learnset::LearnsetRegistry;
	using PocketCore::Registry::Location::LocationRegistry;
	using PocketCore::Registry::Move::MoveRegistry;
	using PocketCore::Registry::Nature::NatureRegistry;
	using PocketCore::Type::BuiltinTypeID;
	using PocketCore::Type::toTypeID;
	using PocketCore::Validation::Ability::AbilityError;
	using PocketCore::Validation::Item::ItemError;
	using PocketCore::Validation::Move::MoveError;
	using PocketCore::Validation::Nature::NatureError;
	using PocketCore::Validation::Pokemon::PokemonError;
	using PocketCore::Validation::Registry::RegistryError;

	using PokemonInstantiationError = std::variant<RegistryError, PokemonError, NatureError, AbilityError, ItemError, MoveError>;

	/*! @struct PokemonInstantiationDependencies Registry/pokemonRegistry.h
		@brief Aggregates non-owning pointers to runtime metadata registries needed for Pokemon instantiation.
		@details Provides a lightweight dependency bundle passed into the Pokemon instantiation system.
		All pointers are non-owning and must refer to registry instances whose lifetime exceeds the provider usage.
		@warning Dereferencing any null member pointer is undefined behavior.
		@date 10/09/2026
		@since 0.12.24
		@version 0.12.51
		@author Matthew Moore
	*/
	struct PokemonInstantiationDependencies
	{
		public:
			/*! @brief Non-owning pointer to the ability metadata registry.
				@details Must point to a valid @ref AbilityRegistry instance for ability metadata queries.
			*/
			const AbilityRegistry *mAbilityRegistry{nullptr};

			/*! @brief Non-owning pointer to the move metadata registry.
				@details Must point to a valid @ref MoveRegistry instance for move metadata queries.
			*/
			const MoveRegistry *mMoveRegistry{nullptr};

			/*! @brief Non-owning pointer to the item metadata registry.
				@details Must point to a valid @ref ItemRegistry instance for item metadata queries.
			*/
			const ItemRegistry *mItemRegistry{nullptr};

			/*! @brief Non-owning pointer to the nature metadata registry.
				@details Must point to a valid @ref NatureRegistry instance for nature metadata queries.
			*/
			const NatureRegistry *mNatureRegistry{nullptr};

			/*! @brief Non-owning pointer to the learnset metadata registry.
				@details Must point to a valid @ref LearnsetRegistry instance for learnset metadata queries.
			*/
			const LearnsetRegistry *mLearnsetRegistry{nullptr};

			/*! @brief Non-owning pointer to the learnset metadata registry.
				@details Must point to a valid @ref LearnsetRegistry instance for learnset metadata queries.
			*/
			const LocationRegistry *mLocationRegistry{nullptr};
	};

	/*! @class PokemonRegistry Registry/pokemonRegistry.h
		@brief Stores built-in and user-defined pokemon metadata in fixed-capacity storage.
		@details Built-in pokemons are registered during construction with IDs derived from @ref BuiltinPokemonID. Configuration code may
	   append, replace, or remove entries through the low-level mutators while battle-time callers use allocation-free lookup operations.
		@note Lookup operations are O(n), where n is bounded by @ref MAX_POKEMON.
		@date 11/12/2026
		@since 0.11.6
		@version 0.12.54
		@author Matthew Moore
	*/
	class PokemonRegistry : private FixedMetadataRegistry<PokemonMeta, PokemonID, MAX_POKEMON, &PokemonMeta::mPokemonID>
	{
		private:
			using Base = FixedMetadataRegistry<PokemonMeta, PokemonID, MAX_POKEMON, &PokemonMeta::mPokemonID>;

		public:
			/*! @brief Compares two PokemonRegistry instances for equality.
				@param[in] other The other registry to compare with.
				@return true if the registries are equal, false otherwise.
				@since 0.12.20
				@version 0.12.20
			*/
			ATTR_NODISCARD constexpr bool operator==(const PokemonRegistry &other) const noexcept = default;

			// LCOV_EXCL_START - If the built in additions fail, the program wouldn't work anyway

			/*! @brief Constructs a registry populated with every @ref BuiltinPokemonID.
				@since 0.11.6
				@version 0.12.53
			 */
			ATTR_NOINLINE explicit constexpr PokemonRegistry()
			{
				addBuiltin({.mName = std::string(POKEMON_NAME_NONE), .mPokemonID = toPokemonID(BuiltinPokemonID::None)});
				addBuiltin({
				    .mEncounters = BULBASAUR_ENCOUNTERS,
					.mName = std::string(POKEMON_NAME_BULBASAUR),
					.mItemPool = {},
					.mAbilityPool = {{
						{.mAbilityID = toAbilityID(BuiltinAbilityID::Overgrow), .mClassification = AbilityClassification::Standard},
						{.mAbilityID = toAbilityID(BuiltinAbilityID::Chlorophyll), .mClassification = AbilityClassification::Hidden},
					},},
					.mBaseStats = BULBASAUR_BASE_STATS,
					.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Grass), toEggGroupID(BuiltinEggGroupID::Monster)},
					.mTypeIDs = {toTypeID(BuiltinTypeID::Grass), toTypeID(BuiltinTypeID::Poison)},
					.mGenderPool = {PokemonGender::Male, PokemonGender::Female},
					.mPokemonID = toPokemonID(BuiltinPokemonID::Bulbasaur),
					.mLearnsetID = toLearnsetID(BuiltinLearnsetID::BulbasaurLearnset),
				});
				addBuiltin({
                    .mEncounters = IVYSAUR_ENCOUNTERS,
					.mName = std::string(POKEMON_NAME_IVYSAUR),
					.mItemPool = {},
					.mAbilityPool = {{
						{.mAbilityID = toAbilityID(BuiltinAbilityID::Overgrow), .mClassification = AbilityClassification::Standard},
						{.mAbilityID = toAbilityID(BuiltinAbilityID::Chlorophyll), .mClassification = AbilityClassification::Hidden},
					},},
					.mBaseStats = IVYSAUR_BASE_STATS,
					.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Grass), toEggGroupID(BuiltinEggGroupID::Monster)},
					.mTypeIDs = {toTypeID(BuiltinTypeID::Grass), toTypeID(BuiltinTypeID::Poison)},
					.mGenderPool = {PokemonGender::Male, PokemonGender::Female},
					.mPokemonID = toPokemonID(BuiltinPokemonID::Ivysaur),
					.mLearnsetID = toLearnsetID(BuiltinLearnsetID::IvysaurLearnset),
				});
				addBuiltin({
				    .mEncounters = VENUSAUR_ENCOUNTERS,
					.mName = std::string(POKEMON_NAME_VENUSAUR),
					.mItemPool = {},
					.mAbilityPool = {{
						{.mAbilityID = toAbilityID(BuiltinAbilityID::Overgrow), .mClassification = AbilityClassification::Standard},
						{.mAbilityID = toAbilityID(BuiltinAbilityID::Chlorophyll), .mClassification = AbilityClassification::Hidden},
					},},
					.mBaseStats = VENUSAUR_BASE_STATS,
					.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Grass), toEggGroupID(BuiltinEggGroupID::Monster)},
					.mTypeIDs = {toTypeID(BuiltinTypeID::Grass), toTypeID(BuiltinTypeID::Poison)},
					.mGenderPool = {PokemonGender::Male, PokemonGender::Female},
					.mPokemonID = toPokemonID(BuiltinPokemonID::Venusaur),
					.mLearnsetID = toLearnsetID(BuiltinLearnsetID::VenusaurLearnset),
				});
				addBuiltin({
                    .mEncounters = CHARMANDER_ENCOUNTERS,
					.mName = std::string(POKEMON_NAME_CHARMANDER),
					.mItemPool = {},
					.mAbilityPool = {{
						{.mAbilityID = toAbilityID(BuiltinAbilityID::Blaze), .mClassification = AbilityClassification::Standard},
						{.mAbilityID = toAbilityID(BuiltinAbilityID::SolarPower), .mClassification = AbilityClassification::Hidden},
					},},
					.mBaseStats = CHARMANDER_BASE_STATS,
					.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Dragon), toEggGroupID(BuiltinEggGroupID::Monster)},
					.mTypeIDs = {toTypeID(BuiltinTypeID::Fire)},
					.mGenderPool = {PokemonGender::Male, PokemonGender::Female},
					.mPokemonID = toPokemonID(BuiltinPokemonID::Charmander),
					.mLearnsetID = toLearnsetID(BuiltinLearnsetID::CharmanderLearnset),
				});
				addBuiltin({
				.mEncounters = CHARMELEON_ENCOUNTERS,
					.mName = std::string(POKEMON_NAME_CHARMELEON),
					.mItemPool = {},
					.mAbilityPool = {{
						{.mAbilityID = toAbilityID(BuiltinAbilityID::Blaze), .mClassification = AbilityClassification::Standard},
						{.mAbilityID = toAbilityID(BuiltinAbilityID::SolarPower), .mClassification = AbilityClassification::Hidden},
					},},
					.mBaseStats = CHARMELEON_BASE_STATS,
					.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Dragon), toEggGroupID(BuiltinEggGroupID::Monster)},
					.mTypeIDs = {toTypeID(BuiltinTypeID::Fire)},
					.mGenderPool = {PokemonGender::Male, PokemonGender::Female},
					.mPokemonID = toPokemonID(BuiltinPokemonID::Charmeleon),
					.mLearnsetID = toLearnsetID(BuiltinLearnsetID::CharmeleonLearnset),
				});
				addBuiltin({
				    .mEncounters = CHARIZARD_ENCOUNTERS,
					.mName = std::string(POKEMON_NAME_CHARIZARD),
					.mItemPool = {},
					.mAbilityPool = {{
						{.mAbilityID = toAbilityID(BuiltinAbilityID::Blaze), .mClassification = AbilityClassification::Standard},
						{.mAbilityID = toAbilityID(BuiltinAbilityID::SolarPower), .mClassification = AbilityClassification::Hidden},
					},},
					.mBaseStats = CHARIZARD_BASE_STATS,
					.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Dragon), toEggGroupID(BuiltinEggGroupID::Monster)},
					.mTypeIDs = {toTypeID(BuiltinTypeID::Fire), toTypeID(BuiltinTypeID::Flying)},
					.mGenderPool = {PokemonGender::Male, PokemonGender::Female},
					.mPokemonID = toPokemonID(BuiltinPokemonID::Charizard),
					.mLearnsetID = toLearnsetID(BuiltinLearnsetID::CharizardLearnset),
				});
				addBuiltin({
                    .mEncounters = SQUIRTLE_ENCOUNTERS,
					.mName = std::string(POKEMON_NAME_SQUIRTLE),
					.mItemPool = {},
					.mAbilityPool = {{
						{.mAbilityID = toAbilityID(BuiltinAbilityID::Torrent), .mClassification = AbilityClassification::Standard},
						{.mAbilityID = toAbilityID(BuiltinAbilityID::RainDish), .mClassification = AbilityClassification::Hidden},
					},},
					.mBaseStats = SQUIRTLE_BASE_STATS,
					.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Monster), toEggGroupID(BuiltinEggGroupID::Water1)},
					.mTypeIDs = {toTypeID(BuiltinTypeID::Water)},
					.mGenderPool = {PokemonGender::Male, PokemonGender::Female},
					.mPokemonID = toPokemonID(BuiltinPokemonID::Squirtle),
					.mLearnsetID = toLearnsetID(BuiltinLearnsetID::SquirtleLearnset),
				});
				addBuiltin({
				    .mEncounters = WARTORTLE_ENCOUNTERS,
					.mName = std::string(POKEMON_NAME_WARTORTLE),
					.mItemPool = {},
					.mAbilityPool = {{
						{.mAbilityID = toAbilityID(BuiltinAbilityID::Torrent), .mClassification = AbilityClassification::Standard},
						{.mAbilityID = toAbilityID(BuiltinAbilityID::RainDish), .mClassification = AbilityClassification::Hidden},
					},},
					.mBaseStats = WARTORTLE_BASE_STATS,
					.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Monster), toEggGroupID(BuiltinEggGroupID::Water1)},
					.mTypeIDs = {toTypeID(BuiltinTypeID::Water)},
					.mGenderPool = {PokemonGender::Male, PokemonGender::Female},
					.mPokemonID = toPokemonID(BuiltinPokemonID::Wartortle),
					.mLearnsetID = toLearnsetID(BuiltinLearnsetID::WartortleLearnset),
				});
				addBuiltin({
					.mEncounters = BLASTOISE_ENCOUNTERS,
					.mName = std::string(POKEMON_NAME_BLASTOISE),
					.mItemPool = {},
					.mAbilityPool = {{
						{.mAbilityID = toAbilityID(BuiltinAbilityID::Torrent), .mClassification = AbilityClassification::Standard},
						{.mAbilityID = toAbilityID(BuiltinAbilityID::RainDish), .mClassification = AbilityClassification::Hidden},
					},},
					.mBaseStats = BLASTOISE_BASE_STATS,
					.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Monster), toEggGroupID(BuiltinEggGroupID::Water1)},
					.mTypeIDs = {toTypeID(BuiltinTypeID::Water)},
					.mGenderPool = {PokemonGender::Male, PokemonGender::Female},
					.mPokemonID = toPokemonID(BuiltinPokemonID::Blastoise),
					.mLearnsetID = toLearnsetID(BuiltinLearnsetID::BlastoiseLearnset),
				});

				addBuiltin({
				    .mEncounters = DITTO_ENCOUNTERS,
					.mName = std::string(POKEMON_NAME_DITTO),
					.mAbilityPool = {{
						{.mAbilityID = toAbilityID(BuiltinAbilityID::Limber), .mClassification = AbilityClassification::Standard},
						{.mAbilityID = toAbilityID(BuiltinAbilityID::Imposter), .mClassification = AbilityClassification::Hidden},
					},},
					.mBaseStats = DITTO_BASE_STATS,
					.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Ditto)},
					.mTypeIDs = {toTypeID(BuiltinTypeID::Normal)},
					.mGenderPool = {PokemonGender::Genderless},
					.mPokemonID = toPokemonID(BuiltinPokemonID::Ditto),
					.mLearnsetID = toLearnsetID(BuiltinLearnsetID::DittoLearnset),
				});
			}

			// LCOV_EXCL_STOP

		protected:
			using Base::addEntry;
			using Base::clearEntries;
			using Base::createCheckpoint;
			using Base::decrementAmountRegistered;
			using Base::eraseEntry;
			using Base::incrementAmountRegistered;
			using Base::restoreCheckpoint;
			using Base::setAmountRegistered;
			using Base::setEntry;

		public:
			using Base::findIndexByID;
			using Base::getAmountRegistered;
			using Base::getEntry;
			using Base::getID;
			using Base::getMetadata;
			using Base::getName;
			using Base::getNextID;
			using Base::getRegisteredEntries;
			using Base::hasEntry;

			/*! @brief Looks up pokemon metadata by stable ID.
				@param[in] pokemonID The stable pokemon identifier.
				@return A non-owning pointer to metadata if registered, or nullptr otherwise. The pointer remains valid until replacement or
			   registry destruction.
				@since 0.11.6
				@version 0.11.6
			*/
			ATTR_NODISCARD constexpr const PokemonMeta *getPokemonMetadata(const PokemonID pokemonID) const
			{
				return getMetadata(pokemonID);
			}

			/*! @brief Looks up an pokemon ID by display name.
				@param[in] name The case-sensitive display name.
				@return The stable ID if registered, or std::nullopt otherwise.
				@since 0.11.6
				@version 0.12.51
			*/
			ATTR_NODISCARD constexpr const std::optional<PokemonID> getPokemonID(const std::string_view name) const
			{
				return getID(name);
			}

			/*! @brief Looks up an pokemon display name by stable ID.
				@param[in] pokemonID The stable pokemon identifier.
				@return The display name if registered, or std::nullopt otherwise.
				@since 0.11.6
				@version 0.11.6
			*/
			ATTR_NODISCARD constexpr const std::optional<std::string_view> getPokemonName(const PokemonID pokemonID) const
			{
				return getName(pokemonID);
			}

			/*! @brief Returns all currently registered pokemon definitions.
				@return A read-only span that remains valid until the registry is mutated or destroyed.
				@since 0.11.6
				@version 0.11.6
			*/
			ATTR_NODISCARD constexpr const std::span<const PokemonMeta> getRegisteredPokemons() const noexcept
			{
				return getRegisteredEntries();
			}

			/*! @brief Returns the next stable ID assigned to a custom pokemon.
				@return The underlying numeric value of the next pokemon ID.
				@since 0.11.6
				@version 0.11.6
			*/
			ATTR_NODISCARD constexpr us getNextPokemonID() const noexcept
			{
				return getNextID();
			}

			/*! @brief Finds an internal array index by stable pokemon ID.
				@param[in] pokemonID The stable pokemon identifier.
				@return The internal index if registered, or std::nullopt otherwise.
				@since 0.11.6
				@version 0.11.6
			*/
			ATTR_NODISCARD constexpr const std::optional<us> findIndexByPokemonID(const PokemonID pokemonID) const
			{
				return findIndexByID(pokemonID);
			}

			/*! @brief Checks whether an pokemon name is registered.
				@param[in] name The case-sensitive display name.
				@return True if the name is registered, otherwise false.
				@since 0.11.6
				@version 0.12.51
			*/
			ATTR_NODISCARD constexpr bool hasPokemon(const std::string_view name) const
			{
				return hasEntry(name);
			}

			/*! @brief Checks whether an pokemon ID is registered.
				@param[in] pokemonID The stable pokemon identifier.
				@return True if the ID is registered, otherwise false.
				@since 0.11.6
				@version 0.11.6
			*/
			ATTR_NODISCARD constexpr bool hasPokemon(const PokemonID pokemonID) const
			{
				return hasEntry(pokemonID);
			}

			/*! @brief Instantiates a Pokemon from the registry.
				@details Uses caller-supplied species ID and encounter index from @ref PokemonSpec.
					Non-empty IDContext and IVEVContext spans supply nature IDs, ability entries, item IDs, move IDs, IVs, and EVs.
					Supplied identifiers and stat arrays pass through their validation helpers and existing duplicate normalization.
					Non-empty sentinel-only identifier spans supply unused slots rather than requesting generation.
					For empty nature, ability, item, move, or IV spans, the selected encounter's corresponding values are used when present.
					Otherwise, natures are selected from registered natures, abilities from the species pool (with breeding inheritance for
					egg encounters), items from the species pool, moves from the learnset and encounter, and IVs randomly. Empty EV spans
					produce zero EVs. Supplied moves require registered metadata, not encounter eligibility.
					A supplied level or gender is used without sampling that field. An omitted level uses the fixed encounter level or
					samples its inclusive range; an omitted gender is sampled from the species gender pool.
					Species metadata resolves name, base stats, types, egg groups, and learnset ID. Selected move metadata resolves
					maximum/default PP; selected nature metadata resolves stat multipliers. Construction derives calculated stats and
					the level damage factor, initializes full health and default current PP, and leaves statuses empty.
					A fully supplied spec generates no fields and consumes no randomness; mixed specs generate only omitted fields.
				@pre Non-null dependency pointers refer to live registries. Inputs and metadata satisfy the applicable construction/helper
					constraints.
				@param[in] pokemonSpec The borrowed specification; span storage and any accessed breeding parents remain valid throughout
					the call. Its species ID is not generated.
				@param[in] dependencyRegistries The borrowed dependency bundle used for metadata resolution; it has no default argument.
				@param[in] randomSources The borrowed random sources used only by automatic policies requiring random draws.
					Referenced sources remain valid throughout the call and may advance their state.
				@return A Pokemon on success, or @ref PokemonInstantiationError containing a missing registry, registry lookup, or nature,
					ability, item, move, or Pokemon validation/generation error. A missing encounter index requires a supplied level; a supplied
					index must identify an active encounter. The returned display name borrows species metadata storage, which must remain valid
					for the Pokemon's lifetime; instance arrays and resolved PP/stat values are copied.
				@since 0.12.24
				@version 0.12.54
			*/
			ATTR_NODISCARD std::expected<Pokemon, PokemonInstantiationError> instantiate(
				const PokemonSpec &pokemonSpec, const PokemonInstantiationDependencies &dependencyRegistries,
				const BattleRandomSources &randomSources) const;
	};
} // namespace PocketCore::Registry::Pokemon

#endif
