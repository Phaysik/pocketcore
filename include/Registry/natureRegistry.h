/*! @file natureRegistry.h
	@brief Provides fixed-capacity storage and lookup for built-in and user-defined natures.
	@date 09/11/2026
	@since 0.11.6
	@version 0.12.23
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
	using PocketCore::Configuration::NATURE_STAT_BASE_MULTIPLIER;
	using PocketCore::Configuration::NATURE_STAT_BOOST_MULTIPLIER;
	using PocketCore::Configuration::NATURE_STAT_WEAKNESS_MULTIPLIER;
	using PocketCore::Core::us;
	using PocketCore::Effect::BuiltinEffectID;
	using PocketCore::Nature::ADAMANT_STAT_MULTIPLIERS;
	using PocketCore::Nature::BASHFUL_STAT_MULTIPLIERS;
	using PocketCore::Nature::BOLD_STAT_MULTIPLIERS;
	using PocketCore::Nature::BRAVE_STAT_MULTIPLIERS;
	using PocketCore::Nature::BuiltinNatureID;
	using PocketCore::Nature::CALM_STAT_MULTIPLIERS;
	using PocketCore::Nature::CAREFUL_STAT_MULTIPLIERS;
	using PocketCore::Nature::DOCILE_STAT_MULTIPLIERS;
	using PocketCore::Nature::GENTLE_STAT_MULTIPLIERS;
	using PocketCore::Nature::HARDY_STAT_MULTIPLIERS;
	using PocketCore::Nature::HASTY_STAT_MULTIPLIERS;
	using PocketCore::Nature::IMPISH_STAT_MULTIPLIERS;
	using PocketCore::Nature::JOLLY_STAT_MULTIPLIERS;
	using PocketCore::Nature::LAX_STAT_MULTIPLIERS;
	using PocketCore::Nature::LONELY_STAT_MULTIPLIERS;
	using PocketCore::Nature::MILD_STAT_MULTIPLIERS;
	using PocketCore::Nature::MODEST_STAT_MULTIPLIERS;
	using PocketCore::Nature::NAIVE_STAT_MULTIPLIERS;
	using PocketCore::Nature::NATURE_NAME_ADAMANT;
	using PocketCore::Nature::NATURE_NAME_BASHFUL;
	using PocketCore::Nature::NATURE_NAME_BOLD;
	using PocketCore::Nature::NATURE_NAME_BRAVE;
	using PocketCore::Nature::NATURE_NAME_CALM;
	using PocketCore::Nature::NATURE_NAME_CAREFUL;
	using PocketCore::Nature::NATURE_NAME_DOCILE;
	using PocketCore::Nature::NATURE_NAME_GENTLE;
	using PocketCore::Nature::NATURE_NAME_HARDY;
	using PocketCore::Nature::NATURE_NAME_HASTY;
	using PocketCore::Nature::NATURE_NAME_IMPISH;
	using PocketCore::Nature::NATURE_NAME_JOLLY;
	using PocketCore::Nature::NATURE_NAME_LAX;
	using PocketCore::Nature::NATURE_NAME_LONELY;
	using PocketCore::Nature::NATURE_NAME_MILD;
	using PocketCore::Nature::NATURE_NAME_MODEST;
	using PocketCore::Nature::NATURE_NAME_NAIVE;
	using PocketCore::Nature::NATURE_NAME_NAUGHTY;
	using PocketCore::Nature::NATURE_NAME_NONE;
	using PocketCore::Nature::NATURE_NAME_QUIET;
	using PocketCore::Nature::NATURE_NAME_QUIRKY;
	using PocketCore::Nature::NATURE_NAME_RASH;
	using PocketCore::Nature::NATURE_NAME_RELAXED;
	using PocketCore::Nature::NATURE_NAME_SASSY;
	using PocketCore::Nature::NATURE_NAME_SERIOUS;
	using PocketCore::Nature::NATURE_NAME_TIMID;
	using PocketCore::Nature::NatureID;
	using PocketCore::Nature::NatureMeta;
	using PocketCore::Nature::NAUGHTY_STAT_MULTIPLIERS;
	using PocketCore::Nature::QUIET_STAT_MULTIPLIERS;
	using PocketCore::Nature::QUIRKY_STAT_MULTIPLIERS;
	using PocketCore::Nature::RASH_STAT_MULTIPLIERS;
	using PocketCore::Nature::RELAXED_STAT_MULTIPLIERS;
	using PocketCore::Nature::SASSY_STAT_MULTIPLIERS;
	using PocketCore::Nature::SERIOUS_STAT_MULTIPLIERS;
	using PocketCore::Nature::TIMID_STAT_MULTIPLIERS;
	using PocketCore::Nature::toNatureID;
	using PocketCore::Registry::FixedMetadataRegistry;

	/*! @class NatureRegistry Registry/natureRegistry.h
		@brief Stores built-in and user-defined nature metadata in fixed-capacity storage.
		@details Built-in natures are registered during construction with IDs derived from @ref BuiltinNatureID. Configuration code may
	   append, replace, or remove entries through the low-level mutators while battle-time callers use allocation-free lookup operations.
		@note Lookup operations are O(n), where n is bounded by @ref MAX_NATURES.
		@date 09/11/2026
		@since 0.11.6
		@version 0.12.23
		@author Matthew Moore
	*/
	class NatureRegistry : private FixedMetadataRegistry<NatureMeta, NatureID, MAX_NATURES, &NatureMeta::mNatureID>
	{
		private:
			using Base = FixedMetadataRegistry<NatureMeta, NatureID, MAX_NATURES, &NatureMeta::mNatureID>;

		public:
			/*! @brief Compares two NatureRegistry instances for equality.
				@param[in] other The other registry to compare with.
				@return true if the registries are equal, false otherwise.
				@since 0.12.20
				@version 0.12.20
			*/
			ATTR_NODISCARD constexpr bool operator==(const NatureRegistry &other) const noexcept = default;

			// LCOV_EXCL_START - If the built in additions fail, the program wouldn't work anyway

			/*! @brief Constructs a registry populated with every @ref BuiltinNatureID.
				@since 0.11.6
				@version 0.12.23
			 */
			ATTR_NOINLINE explicit constexpr NatureRegistry() : Base{toNatureID(BuiltinNatureID::FinalNature).getValue()}
			{
				addBuiltin({.mName = std::string(NATURE_NAME_NONE), .mNatureID = toNatureID(BuiltinNatureID::None)});

				// --- Standard Neutral natures ---
				addBuiltin({
					.mStatMultipliers = HARDY_STAT_MULTIPLIERS,
					.mName = std::string(NATURE_NAME_HARDY),
					.mNatureID = toNatureID(BuiltinNatureID::Hardy),
				});
				addBuiltin({
					.mStatMultipliers = DOCILE_STAT_MULTIPLIERS,
					.mName = std::string(NATURE_NAME_DOCILE),
					.mNatureID = toNatureID(BuiltinNatureID::Docile),
				});
				addBuiltin({
					.mStatMultipliers = SERIOUS_STAT_MULTIPLIERS,
					.mName = std::string(NATURE_NAME_SERIOUS),
					.mNatureID = toNatureID(BuiltinNatureID::Serious),
				});
				addBuiltin({
					.mStatMultipliers = BASHFUL_STAT_MULTIPLIERS,
					.mName = std::string(NATURE_NAME_BASHFUL),
					.mNatureID = toNatureID(BuiltinNatureID::Bashful),
				});
				addBuiltin({
					.mStatMultipliers = QUIRKY_STAT_MULTIPLIERS,
					.mName = std::string(NATURE_NAME_QUIRKY),
					.mNatureID = toNatureID(BuiltinNatureID::Quirky),
				});

				// --- Standard +Attack natures ---
				addBuiltin({
					.mStatMultipliers = LONELY_STAT_MULTIPLIERS,
					.mName = std::string(NATURE_NAME_LONELY),
					.mNatureID = toNatureID(BuiltinNatureID::Lonely),
				});

				addBuiltin({
					.mStatMultipliers = BRAVE_STAT_MULTIPLIERS,
					.mName = std::string(NATURE_NAME_BRAVE),
					.mNatureID = toNatureID(BuiltinNatureID::Brave),
				});
				addBuiltin({
					.mStatMultipliers = ADAMANT_STAT_MULTIPLIERS,
					.mName = std::string(NATURE_NAME_ADAMANT),
					.mNatureID = toNatureID(BuiltinNatureID::Adamant),
				});
				addBuiltin({
					.mStatMultipliers = NAUGHTY_STAT_MULTIPLIERS,
					.mName = std::string(NATURE_NAME_NAUGHTY),
					.mNatureID = toNatureID(BuiltinNatureID::Naughty),
				});

				// --- Standard +Defense natures ---
				addBuiltin({
					.mStatMultipliers = BOLD_STAT_MULTIPLIERS,
					.mName = std::string(NATURE_NAME_BOLD),
					.mNatureID = toNatureID(BuiltinNatureID::Bold),
				});

				addBuiltin({
					.mStatMultipliers = RELAXED_STAT_MULTIPLIERS,
					.mName = std::string(NATURE_NAME_RELAXED),
					.mNatureID = toNatureID(BuiltinNatureID::Relaxed),
				});
				addBuiltin({
					.mStatMultipliers = IMPISH_STAT_MULTIPLIERS,
					.mName = std::string(NATURE_NAME_IMPISH),
					.mNatureID = toNatureID(BuiltinNatureID::Impish),
				});
				addBuiltin({
					.mStatMultipliers = LAX_STAT_MULTIPLIERS,
					.mName = std::string(NATURE_NAME_LAX),
					.mNatureID = toNatureID(BuiltinNatureID::Lax),
				});

				// --- Standard +Speed natures ---
				addBuiltin({
					.mStatMultipliers = TIMID_STAT_MULTIPLIERS,
					.mName = std::string(NATURE_NAME_TIMID),
					.mNatureID = toNatureID(BuiltinNatureID::Timid),
				});
				addBuiltin({
					.mStatMultipliers = HASTY_STAT_MULTIPLIERS,
					.mName = std::string(NATURE_NAME_HASTY),
					.mNatureID = toNatureID(BuiltinNatureID::Hasty),
				});

				addBuiltin({
					.mStatMultipliers = JOLLY_STAT_MULTIPLIERS,
					.mName = std::string(NATURE_NAME_JOLLY),
					.mNatureID = toNatureID(BuiltinNatureID::Jolly),
				});
				addBuiltin({
					.mStatMultipliers = NAIVE_STAT_MULTIPLIERS,
					.mName = std::string(NATURE_NAME_NAIVE),
					.mNatureID = toNatureID(BuiltinNatureID::Naive),
				});

				// --- Standard +SpAttack natures ---
				addBuiltin({
					.mStatMultipliers = MODEST_STAT_MULTIPLIERS,
					.mName = std::string(NATURE_NAME_MODEST),
					.mNatureID = toNatureID(BuiltinNatureID::Modest),
				});
				addBuiltin({
					.mStatMultipliers = MILD_STAT_MULTIPLIERS,
					.mName = std::string(NATURE_NAME_MILD),
					.mNatureID = toNatureID(BuiltinNatureID::Mild),
				});
				addBuiltin({
					.mStatMultipliers = QUIET_STAT_MULTIPLIERS,
					.mName = std::string(NATURE_NAME_QUIET),
					.mNatureID = toNatureID(BuiltinNatureID::Quiet),
				});
				addBuiltin({
					.mStatMultipliers = RASH_STAT_MULTIPLIERS,
					.mName = std::string(NATURE_NAME_RASH),
					.mNatureID = toNatureID(BuiltinNatureID::Rash),
				});

				// --- Standard +SpDefense natures ---
				addBuiltin({
					.mStatMultipliers = CALM_STAT_MULTIPLIERS,
					.mName = std::string(NATURE_NAME_CALM),
					.mNatureID = toNatureID(BuiltinNatureID::Calm),
				});
				addBuiltin({
					.mStatMultipliers = GENTLE_STAT_MULTIPLIERS,
					.mName = std::string(NATURE_NAME_GENTLE),
					.mNatureID = toNatureID(BuiltinNatureID::Gentle),
				});
				addBuiltin({
					.mStatMultipliers = SASSY_STAT_MULTIPLIERS,
					.mName = std::string(NATURE_NAME_SASSY),
					.mNatureID = toNatureID(BuiltinNatureID::Sassy),
				});
				addBuiltin({
					.mStatMultipliers = CAREFUL_STAT_MULTIPLIERS,
					.mName = std::string(NATURE_NAME_CAREFUL),
					.mNatureID = toNatureID(BuiltinNatureID::Careful),
				});
			}

			// LCOV_EXCL_STOP

		protected:
			using Base::addEntry;
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

			/*! @brief Looks up nature metadata by stable ID.
				@param[in] natureID The stable nature identifier.
				@return A non-owning pointer to metadata if registered, or nullptr otherwise. The pointer remains valid until replacement or
			   registry destruction.
				@since 0.11.6
				@version 0.11.6
			*/
			ATTR_NODISCARD constexpr const NatureMeta *getNatureMetadata(const NatureID natureID) const
			{
				return getMetadata(natureID);
			}

			/*! @brief Looks up an nature ID by display name.
				@param[in] name The case-sensitive display name.
				@return The stable ID if registered, or std::nullopt otherwise.
				@since 0.11.6
				@version 0.11.6
			*/
			ATTR_NODISCARD constexpr const std::optional<NatureID> getNatureID(const std::string_view &name) const
			{
				return getID(name);
			}

			/*! @brief Looks up an nature display name by stable ID.
				@param[in] natureID The stable nature identifier.
				@return The display name if registered, or std::nullopt otherwise.
				@since 0.11.6
				@version 0.11.6
			*/
			ATTR_NODISCARD constexpr const std::optional<std::string_view> getNatureName(const NatureID natureID) const
			{
				return getName(natureID);
			}

			/*! @brief Returns all currently registered nature definitions.
				@return A read-only span that remains valid until the registry is mutated or destroyed.
				@since 0.11.6
				@version 0.11.6
			*/
			ATTR_NODISCARD constexpr const std::span<const NatureMeta> getRegisteredNatures() const noexcept
			{
				return getRegisteredEntries();
			}

			/*! @brief Returns the next stable ID assigned to a custom nature.
				@return The underlying numeric value of the next nature ID.
				@since 0.11.6
				@version 0.11.6
			*/
			ATTR_NODISCARD constexpr us getNextNatureID() const noexcept
			{
				return getNextID();
			}

			/*! @brief Finds an internal array index by stable nature ID.
				@param[in] natureID The stable nature identifier.
				@return The internal index if registered, or std::nullopt otherwise.
				@since 0.11.6
				@version 0.11.6
			*/
			ATTR_NODISCARD constexpr const std::optional<us> findIndexByNatureID(const NatureID natureID) const
			{
				return findIndexByID(natureID);
			}

			/*! @brief Checks whether an nature name is registered.
				@param[in] name The case-sensitive display name.
				@return True if the name is registered, otherwise false.
				@since 0.11.6
				@version 0.11.6
			*/
			ATTR_NODISCARD constexpr bool hasNature(const std::string_view &name) const
			{
				return hasEntry(name);
			}

			/*! @brief Checks whether an nature ID is registered.
				@param[in] natureID The stable nature identifier.
				@return True if the ID is registered, otherwise false.
				@since 0.11.6
				@version 0.11.6
			*/
			ATTR_NODISCARD constexpr bool hasNature(const NatureID natureID) const
			{
				return hasEntry(natureID);
			}
	};
} // namespace PocketCore::Registry::Nature

#endif
