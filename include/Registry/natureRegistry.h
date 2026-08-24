/*! @file natureRegistry.h
	@brief Provides fixed-capacity storage and lookup for built-in and user-defined natures.
	@date 07/27/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_REGISTRY_NATURE_REGISTRY_H
#define INCLUDE_REGISTRY_NATURE_REGISTRY_H

#include <optional>
#include <span>
#include <string_view>

#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Effect/builtInEffectID.h"
#include "Nature/builtInNatureID.h"
#include "Nature/constants.h"
#include "Nature/natureID.h"
#include "Nature/natureMeta.h"
#include "Registry/fixedMetadataRegistry.h"

namespace PocketCore::Registry::Nature
{
	using PocketCore::Configuration::MAX_NATURES;
	using PocketCore::Configuration::MAX_STAT_STAGES;
	using PocketCore::Configuration::NATURE_STAT_BASE_MULTIPLIER;
	using PocketCore::Configuration::NATURE_STAT_BOOST_MULTIPLIER;
	using PocketCore::Configuration::NATURE_STAT_WEAKNESS_MULTIPLIER;
	using PocketCore::Core::us;
	using PocketCore::Effect::BuiltinEffectID;
	using PocketCore::Nature::BuiltinNatureID;
	using PocketCore::Nature::NatureID;
	using PocketCore::Nature::NatureMeta;
	using PocketCore::Nature::toNatureID;
	using PocketCore::Registry::FixedMetadataRegistry;

	/*! @class NatureRegistry Registry/natureRegistry.h
		@brief Stores built-in and user-defined nature metadata in fixed-capacity storage.
		@details Built-in natures are registered during construction with IDs derived from @ref BuiltinNatureID. Configuration code may
	   append, replace, or remove entries through the low-level mutators while battle-time callers use allocation-free lookup operations.
		@note Lookup operations are O(n), where n is bounded by @ref MAX_NATURES.
		@date 07/27/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	class NatureRegistry : private FixedMetadataRegistry<NatureMeta, NatureID, MAX_NATURES, &NatureMeta::mNatureID>
	{
		private:
			using Base = FixedMetadataRegistry<NatureMeta, NatureID, MAX_NATURES, &NatureMeta::mNatureID>;

			static constexpr std::array<double, MAX_STAT_STAGES> makeMultipliers(const double health, const double attack,
																				 const double defense, const double spAttack,
																				 const double spDefense, const double speed) noexcept
			{
				return {health, attack, defense, spAttack, spDefense, speed};
			}

		public:
			// LCOV_EXCL_START - If the built in additions fail, the program wouldn't work anyway
			/*! @brief Constructs a registry populated with every @ref BuiltinNatureID. */
			ATTR_NOINLINE explicit constexpr NatureRegistry() : Base{toNatureID(BuiltinNatureID::FinalNature).getValue()}
			{
				// --- Neutral natures (no triggers) ---
				auto addNeutral = [this](const BuiltinNatureID natureID, const std::string_view &name) {
					addBuiltin({
						.mStatMultipliers = makeMultipliers(1.0, 1.0, 1.0, 1.0, 1.0, 1.0),
						.mName = name,
						.mNatureID = toNatureID(natureID),
					});
				};

				addBuiltin({.mName = PocketCore::Nature::NATURE_NAME_NONE, .mNatureID = toNatureID(BuiltinNatureID::None)});

				// --- Standard Neutral natures ---
				addNeutral(BuiltinNatureID::Hardy, PocketCore::Nature::NATURE_NAME_HARDY);
				addNeutral(BuiltinNatureID::Docile, PocketCore::Nature::NATURE_NAME_DOCILE);
				addNeutral(BuiltinNatureID::Serious, PocketCore::Nature::NATURE_NAME_SERIOUS);
				addNeutral(BuiltinNatureID::Bashful, PocketCore::Nature::NATURE_NAME_BASHFUL);
				addNeutral(BuiltinNatureID::Quirky, PocketCore::Nature::NATURE_NAME_QUIRKY);

				// --- Standard +Attack natures ---
				addBuiltin({
					.mStatMultipliers
					= makeMultipliers(NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BOOST_MULTIPLIER, NATURE_STAT_WEAKNESS_MULTIPLIER,
									  NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER),
					.mName = PocketCore::Nature::NATURE_NAME_LONELY,
					.mNatureID = toNatureID(BuiltinNatureID::Lonely),
				});

				addBuiltin({
					.mStatMultipliers
					= makeMultipliers(NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BOOST_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER,
									  NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_WEAKNESS_MULTIPLIER),
					.mName = PocketCore::Nature::NATURE_NAME_BRAVE,
					.mNatureID = toNatureID(BuiltinNatureID::Brave),
				});
				addBuiltin({
					.mStatMultipliers
					= makeMultipliers(NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BOOST_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER,
									  NATURE_STAT_WEAKNESS_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER),
					.mName = PocketCore::Nature::NATURE_NAME_ADAMANT,
					.mNatureID = toNatureID(BuiltinNatureID::Adamant),
				});
				addBuiltin({
					.mStatMultipliers
					= makeMultipliers(NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BOOST_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER,
									  NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_WEAKNESS_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER),
					.mName = PocketCore::Nature::NATURE_NAME_NAUGHTY,
					.mNatureID = toNatureID(BuiltinNatureID::Naughty),
				});

				// --- Standard +Defense natures ---
				addBuiltin({
					.mStatMultipliers
					= makeMultipliers(NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_WEAKNESS_MULTIPLIER, NATURE_STAT_BOOST_MULTIPLIER,
									  NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER),
					.mName = PocketCore::Nature::NATURE_NAME_BOLD,
					.mNatureID = toNatureID(BuiltinNatureID::Bold),
				});

				addBuiltin({
					.mStatMultipliers
					= makeMultipliers(NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BOOST_MULTIPLIER,
									  NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_WEAKNESS_MULTIPLIER),
					.mName = PocketCore::Nature::NATURE_NAME_RELAXED,
					.mNatureID = toNatureID(BuiltinNatureID::Relaxed),
				});
				addBuiltin({
					.mStatMultipliers
					= makeMultipliers(NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BOOST_MULTIPLIER,
									  NATURE_STAT_WEAKNESS_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER),
					.mName = PocketCore::Nature::NATURE_NAME_IMPISH,
					.mNatureID = toNatureID(BuiltinNatureID::Impish),
				});
				addBuiltin({
					.mStatMultipliers
					= makeMultipliers(NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BOOST_MULTIPLIER,
									  NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_WEAKNESS_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER),
					.mName = PocketCore::Nature::NATURE_NAME_LAX,
					.mNatureID = toNatureID(BuiltinNatureID::Lax),
				});

				// --- Standard +Speed natures ---
				addBuiltin({
					.mStatMultipliers
					= makeMultipliers(NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_WEAKNESS_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER,
									  NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BOOST_MULTIPLIER),
					.mName = PocketCore::Nature::NATURE_NAME_TIMID,
					.mNatureID = toNatureID(BuiltinNatureID::Timid),
				});
				addBuiltin({
					.mStatMultipliers
					= makeMultipliers(NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_WEAKNESS_MULTIPLIER,
									  NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BOOST_MULTIPLIER),
					.mName = PocketCore::Nature::NATURE_NAME_HASTY,
					.mNatureID = toNatureID(BuiltinNatureID::Hasty),
				});

				addBuiltin({
					.mStatMultipliers
					= makeMultipliers(NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER,
									  NATURE_STAT_WEAKNESS_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BOOST_MULTIPLIER),
					.mName = PocketCore::Nature::NATURE_NAME_JOLLY,
					.mNatureID = toNatureID(BuiltinNatureID::Jolly),
				});
				addBuiltin({
					.mStatMultipliers
					= makeMultipliers(NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER,
									  NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_WEAKNESS_MULTIPLIER, NATURE_STAT_BOOST_MULTIPLIER),
					.mName = PocketCore::Nature::NATURE_NAME_NAIVE,
					.mNatureID = toNatureID(BuiltinNatureID::Naive),
				});

				// --- Standard +SpAttack natures ---
				addBuiltin({
					.mStatMultipliers
					= makeMultipliers(NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_WEAKNESS_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER,
									  NATURE_STAT_BOOST_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER),
					.mName = PocketCore::Nature::NATURE_NAME_MODEST,
					.mNatureID = toNatureID(BuiltinNatureID::Modest),
				});
				addBuiltin({
					.mStatMultipliers
					= makeMultipliers(NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_WEAKNESS_MULTIPLIER,
									  NATURE_STAT_BOOST_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER),
					.mName = PocketCore::Nature::NATURE_NAME_MILD,
					.mNatureID = toNatureID(BuiltinNatureID::Mild),
				});
				addBuiltin({
					.mStatMultipliers
					= makeMultipliers(NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER,
									  NATURE_STAT_BOOST_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_WEAKNESS_MULTIPLIER),
					.mName = PocketCore::Nature::NATURE_NAME_QUIET,
					.mNatureID = toNatureID(BuiltinNatureID::Quiet),
				});
				addBuiltin({
					.mStatMultipliers
					= makeMultipliers(NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER,
									  NATURE_STAT_BOOST_MULTIPLIER, NATURE_STAT_WEAKNESS_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER),
					.mName = PocketCore::Nature::NATURE_NAME_RASH,
					.mNatureID = toNatureID(BuiltinNatureID::Rash),
				});

				// --- Standard +SpDefense natures ---
				addBuiltin({
					.mStatMultipliers
					= makeMultipliers(NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_WEAKNESS_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER,
									  NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BOOST_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER),
					.mName = PocketCore::Nature::NATURE_NAME_CALM,
					.mNatureID = toNatureID(BuiltinNatureID::Calm),
				});
				addBuiltin({
					.mStatMultipliers
					= makeMultipliers(NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_WEAKNESS_MULTIPLIER,
									  NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BOOST_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER),
					.mName = PocketCore::Nature::NATURE_NAME_GENTLE,
					.mNatureID = toNatureID(BuiltinNatureID::Gentle),
				});
				addBuiltin({
					.mStatMultipliers
					= makeMultipliers(NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER,
									  NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BOOST_MULTIPLIER, NATURE_STAT_WEAKNESS_MULTIPLIER),
					.mName = PocketCore::Nature::NATURE_NAME_SASSY,
					.mNatureID = toNatureID(BuiltinNatureID::Sassy),
				});
				addBuiltin({
					.mStatMultipliers
					= makeMultipliers(NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER,
									  NATURE_STAT_WEAKNESS_MULTIPLIER, NATURE_STAT_BOOST_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER),
					.mName = PocketCore::Nature::NATURE_NAME_CAREFUL,
					.mNatureID = toNatureID(BuiltinNatureID::Careful),
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

			/*! @brief Looks up nature metadata by stable ID.
				@param[in] natureID The stable nature identifier.
				@return A non-owning pointer to metadata if registered, or nullptr otherwise. The pointer remains valid until replacement or
			   registry destruction.
			*/
			ATTR_NODISCARD constexpr const NatureMeta *getNatureMetadata(const NatureID natureID) const
			{
				return getMetadata(natureID);
			}

			/*! @brief Looks up an nature ID by display name.
				@param[in] name The case-sensitive display name.
				@return The stable ID if registered, or std::nullopt otherwise.
			*/
			ATTR_NODISCARD constexpr const std::optional<NatureID> getNatureID(const std::string_view &name) const
			{
				return getID(name);
			}

			/*! @brief Looks up an nature display name by stable ID.
				@param[in] natureID The stable nature identifier.
				@return The display name if registered, or std::nullopt otherwise.
			*/
			ATTR_NODISCARD constexpr const std::optional<std::string_view> getNatureName(const NatureID natureID) const
			{
				return getName(natureID);
			}

			/*! @brief Returns all currently registered nature definitions.
				@return A read-only span that remains valid until the registry is mutated or destroyed.
			*/
			ATTR_NODISCARD constexpr const std::span<const NatureMeta> getRegisteredNatures() const noexcept
			{
				return getRegisteredEntries();
			}

			/*! @brief Returns the next stable ID assigned to a custom nature.
				@return The underlying numeric value of the next nature ID.
			*/
			ATTR_NODISCARD constexpr us getNextNatureID() const noexcept
			{
				return getNextID();
			}

			/*! @brief Finds an internal array index by stable nature ID.
				@param[in] natureID The stable nature identifier.
				@return The internal index if registered, or std::nullopt otherwise.
			*/
			ATTR_NODISCARD constexpr const std::optional<us> findIndexByNatureID(const NatureID natureID) const
			{
				return findIndexByID(natureID);
			}

			/*! @brief Checks whether an nature name is registered.
				@param[in] name The case-sensitive display name.
				@return True if the name is registered, otherwise false.
			*/
			ATTR_NODISCARD constexpr bool hasNature(const std::string_view &name) const
			{
				return hasEntry(name);
			}

			/*! @brief Checks whether an nature ID is registered.
				@param[in] natureID The stable nature identifier.
				@return True if the ID is registered, otherwise false.
			*/
			ATTR_NODISCARD constexpr bool hasNature(const NatureID natureID) const
			{
				return hasEntry(natureID);
			}

			/*! @brief Sets the next custom nature ID counter.
				@param[in] nextID The next underlying ID value.
			*/
			constexpr void setNextNatureID(const us nextID) noexcept
			{
				setNextID(nextID);
			}

			/*! @brief Increments the next custom nature ID counter. */
			constexpr void incrementNextNatureID() noexcept
			{
				incrementNextID();
			}
	};
} // namespace PocketCore::Registry::Nature

#endif
