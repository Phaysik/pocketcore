/*! @file learnsetRegistry.h
	@brief Provides fixed-capacity storage and lookup for built-in and user-defined learnsets.
	@date 10/08/2026
	@since 0.12.50
	@version 0.12.50
	@author Matthew Moore
*/

#ifndef INCLUDE_REGISTRY_LEARNSET_REGISTRY_H
#define INCLUDE_REGISTRY_LEARNSET_REGISTRY_H

#include <optional>
#include <span>
#include <string_view>

#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Learnset/builtInLearnsetID.h"
#include "Learnset/constants.h"
#include "Learnset/learnsetID.h"
#include "Learnset/learnsetMeta.h"
#include "Registry/fixedMetadataRegistry.h"

namespace PocketCore::Registry::Learnset
{
	using PocketCore::Configuration::MAX_LEARNSETS;
	using PocketCore::Core::us;
	using PocketCore::Learnset::BuiltinLearnsetID;
	using PocketCore::Learnset::LEARNSET_NAME_BLASTOISE;
	using PocketCore::Learnset::LEARNSET_NAME_BULBASAUR;
	using PocketCore::Learnset::LEARNSET_NAME_CHARIZARD;
	using PocketCore::Learnset::LEARNSET_NAME_CHARMANDER;
	using PocketCore::Learnset::LEARNSET_NAME_CHARMELEON;
	using PocketCore::Learnset::LEARNSET_NAME_IVYSAUR;
	using PocketCore::Learnset::LEARNSET_NAME_NONE;
	using PocketCore::Learnset::LEARNSET_NAME_SQUIRTLE;
	using PocketCore::Learnset::LEARNSET_NAME_VENUSAUR;
	using PocketCore::Learnset::LEARNSET_NAME_WARTORTLE;
	using PocketCore::Learnset::LearnsetID;
	using PocketCore::Learnset::LearnsetMeta;
	using PocketCore::Learnset::toLearnsetID;
	using PocketCore::Registry::FixedMetadataRegistry;

	/*! @class LearnsetRegistry Registry/learnsetRegistry.h
		@brief Stores built-in and user-defined learnset metadata in fixed-capacity storage.
		@details Built-in learnsets are registered during construction with IDs derived from @ref BuiltinLearnsetID. Configuration code may
	   append, replace, or remove owned metadata through protected mutators while battle-time callers use allocation-free lookup operations.
		@note Stable-ID lookups are O(log n), while name lookups are O(n), where n is bounded by @ref MAX_LEARNSETS.
		@date 10/08/2026
		@since 0.12.50
		@version 0.12.50
		@author Matthew Moore
	*/
	class LearnsetRegistry : private FixedMetadataRegistry<LearnsetMeta, LearnsetID, MAX_LEARNSETS, &LearnsetMeta::mLearnsetID>
	{
		private:
			using Base = FixedMetadataRegistry<LearnsetMeta, LearnsetID, MAX_LEARNSETS, &LearnsetMeta::mLearnsetID>;

		public:
			/*! @brief Compares two LearnsetRegistry instances for equality.
				@param[in] other The other registry to compare with.
				@return true if the registries are equal, false otherwise.
				@since 0.12.50
				@version 0.12.50
			*/
			ATTR_NODISCARD constexpr bool operator==(const LearnsetRegistry &other) const noexcept = default;

			// LCOV_EXCL_START - If the built in additions fail, the program wouldn't work anyway

			/*! @brief Constructs a registry populated with all built-in learnsets, including None but excluding the FinalLearnset sentinel.
				@since 0.12.50
				@version 0.12.50
			 */
			ATTR_NOINLINE explicit constexpr LearnsetRegistry() : Base{toLearnsetID(BuiltinLearnsetID::FinalLearnset).getValue()}
			{
				addBuiltin({.mName = std::string(LEARNSET_NAME_NONE), .mLearnsetID = toLearnsetID(BuiltinLearnsetID::None)});
				addBuiltin(
					{.mName = std::string(LEARNSET_NAME_BULBASAUR), .mLearnsetID = toLearnsetID(BuiltinLearnsetID::BulbasaurLearnset)});
				addBuiltin({.mName = std::string(LEARNSET_NAME_IVYSAUR), .mLearnsetID = toLearnsetID(BuiltinLearnsetID::IvysaurLearnset)});
				addBuiltin(
					{.mName = std::string(LEARNSET_NAME_VENUSAUR), .mLearnsetID = toLearnsetID(BuiltinLearnsetID::VenusaurLearnset)});
				addBuiltin(
					{.mName = std::string(LEARNSET_NAME_SQUIRTLE), .mLearnsetID = toLearnsetID(BuiltinLearnsetID::SquirtleLearnset)});
				addBuiltin(
					{.mName = std::string(LEARNSET_NAME_WARTORTLE), .mLearnsetID = toLearnsetID(BuiltinLearnsetID::WartortleLearnset)});
				addBuiltin(
					{.mName = std::string(LEARNSET_NAME_BLASTOISE), .mLearnsetID = toLearnsetID(BuiltinLearnsetID::BlastoiseLearnset)});
				addBuiltin(
					{.mName = std::string(LEARNSET_NAME_CHARMANDER), .mLearnsetID = toLearnsetID(BuiltinLearnsetID::CharmanderLearnset)});
				addBuiltin(
					{.mName = std::string(LEARNSET_NAME_CHARMELEON), .mLearnsetID = toLearnsetID(BuiltinLearnsetID::CharmeleonLearnset)});
				addBuiltin(
					{.mName = std::string(LEARNSET_NAME_CHARIZARD), .mLearnsetID = toLearnsetID(BuiltinLearnsetID::CharizardLearnset)});
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

			/*! @brief Looks up learnset metadata by stable ID.
				@param[in] learnsetID The stable learnset identifier.
				@return A non-owning pointer to registry-owned metadata if registered, or nullptr otherwise. The pointer must not be retained
			   across registry mutations or destruction, as mutations can replace metadata or compact storage.
				@since 0.12.50
				@version 0.12.50
			*/
			ATTR_NODISCARD constexpr const LearnsetMeta *getLearnsetMetadata(const LearnsetID learnsetID) const
			{
				return getMetadata(learnsetID);
			}

			/*! @brief Looks up a learnset ID by display name.
				@param[in] name The case-sensitive display name.
				@return The stable ID if registered, or std::nullopt otherwise.
				@since 0.12.50
				@version 0.12.50
			*/
			ATTR_NODISCARD constexpr const std::optional<LearnsetID> getLearnsetID(const std::string_view &name) const
			{
				return getID(name);
			}

			/*! @brief Looks up a learnset display name by stable ID.
				@param[in] learnsetID The stable learnset identifier.
				@return A non-owning view of the registry-owned display name if registered, or std::nullopt otherwise. The view must not be
			   retained across registry mutations or destruction.
				@since 0.12.50
				@version 0.12.50
			*/
			ATTR_NODISCARD constexpr const std::optional<std::string_view> getLearnsetName(const LearnsetID learnsetID) const
			{
				return getName(learnsetID);
			}

			/*! @brief Returns all currently registered learnset definitions.
				@return A read-only span that remains valid until the registry is mutated or destroyed.
				@since 0.12.50
				@version 0.12.50
			*/
			ATTR_NODISCARD constexpr const std::span<const LearnsetMeta> getRegisteredLearnsets() const noexcept
			{
				return getRegisteredEntries();
			}

			/*! @brief Returns the next stable ID assigned to a custom learnset.
				@return The underlying numeric value of the next learnset ID.
				@since 0.12.50
				@version 0.12.50
			*/
			ATTR_NODISCARD constexpr us getNextLearnsetID() const noexcept
			{
				return getNextID();
			}

			/*! @brief Finds an internal array index by stable learnset ID.
				@param[in] learnsetID The stable learnset identifier.
				@return The internal index if registered, or std::nullopt otherwise.
				@since 0.12.50
				@version 0.12.50
			*/
			ATTR_NODISCARD constexpr const std::optional<us> findIndexByLearnsetID(const LearnsetID learnsetID) const
			{
				return findIndexByID(learnsetID);
			}

			/*! @brief Checks whether a learnset name is registered.
				@param[in] name The case-sensitive display name.
				@return True if the name is registered, otherwise false.
				@since 0.12.50
				@version 0.12.50
			*/
			ATTR_NODISCARD constexpr bool hasLearnset(const std::string_view &name) const
			{
				return hasEntry(name);
			}

			/*! @brief Checks whether a learnset ID is registered.
				@param[in] learnsetID The stable learnset identifier.
				@return True if the ID is registered, otherwise false.
				@since 0.12.50
				@version 0.12.50
			*/
			ATTR_NODISCARD constexpr bool hasLearnset(const LearnsetID learnsetID) const
			{
				return hasEntry(learnsetID);
			}
	};
} // namespace PocketCore::Registry::Learnset

#endif
