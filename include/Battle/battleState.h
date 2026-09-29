/*! @file battleState.h
	@brief Contains the battle state
	@date 09/29/2026
	@since 0.3.0
	@version 0.12.46
	@author Matthew Moore
*/

#ifndef INCLUDE_BATTLE_BATTLESTATE_H
#define INCLUDE_BATTLE_BATTLESTATE_H

#include <algorithm>
#include <array>
#include <expected>
#include <vector>

#include "Configuration/constants.h"
#include "Core/typedefs.h"
#include "Interaction/interactionApplicationError.h"
#include "Interaction/interactionHelpers.h"
#include "Move/moveID.h"
#include "Pokemon/pokemon.h"
#include "Registry/statusRegistry.h"
#include "Ruleset/rulesetPolicy.h"
#include "Status/statusID.h"
#include "Status/statusMeta.h"
#include "Status/volatileStatus.h"
#include "Terrain/terrainID.h"
#include "Weather/weatherID.h"

namespace PocketCore::Battle
{
	using PocketCore::Configuration::MAX_ACTIVE_TERRAINS_ON_FIELD;
	using PocketCore::Configuration::MAX_ACTIVE_WEATHERS_ON_FIELD;
	using PocketCore::Configuration::MAX_VOLATILE_STATUSES_PER_POKEMON;
	using PocketCore::Core::sb;
	using PocketCore::Core::ub;
	using PocketCore::Core::us;
	using PocketCore::Interaction::applyInteractions;
	using PocketCore::Interaction::InteractionApplicationError;
	using PocketCore::Move::MoveID;
	using PocketCore::Pokemon::Pokemon;
	using PocketCore::Registry::Status::StatusRegistry;
	using PocketCore::Ruleset::RulesetPolicy;
	using PocketCore::Status::NO_STATUS_ID;
	using PocketCore::Status::StatusClassification;
	using PocketCore::Status::StatusID;
	using PocketCore::Status::StatusMeta;
	using PocketCore::Status::VolatileStatus;
	using PocketCore::Terrain::TerrainID;
	using PocketCore::Weather::WeatherID;

	/*! @struct StatStages Battle/battleState.h
		@brief Stores a battler's temporary stat stage changes.
		@details Each signed stage applies to the corresponding stat during battle calculations.
		@date 09/28/2026
		@since 0.3.0
		@version 0.12.45
		@author Matthew Moore
	*/
	struct StatStages
	{
		public:
			/*! @brief Writes the StatStages' raw identifier and statistic representation to a stream.
				@param[in,out] outStream The stream receiving the representation.
				@param[in] statStages The StatStages to write.
				@return The supplied stream after writing the representation.
				@since 0.12.45
				@version 0.12.45
			*/
			friend std::ostream &operator<<(std::ostream &outStream, const StatStages &statStages);

		public:
			/*! @brief The temporary Attack stage. */
			sb mAttack{0};
			/*! @brief The temporary Defense stage. */
			sb mDefense{0};
			/*! @brief The temporary Special Attack stage. */
			sb mSpAttack{0};
			/*! @brief The temporary Special Defense stage. */
			sb mSpDefense{0};
			/*! @brief The temporary Speed stage. */
			sb mSpeed{0};
			/*! @brief The temporary accuracy stage. */
			sb mAccuracy{0};
			/*! @brief The temporary evasion stage. */
			sb mEvasion{0};
	};

	/*! @struct DamageFormulaModifiers Battle/battleState.h
		@brief Stores multiplicative modifiers applied to damage-formula statistics.
		@details A default-constructed instance leaves every supported statistic unchanged by initializing each modifier to 1.0.
		@date 09/28/2026
		@since 0.8.5
		@version 0.12.45
		@author Matthew Moore
	*/
	struct DamageFormulaModifiers
	{
		public:
			/*! @brief Writes the DamageFormulaModifiers' raw identifier and statistic representation to a stream.
				@param[in,out] outStream The stream receiving the representation.
				@param[in] damageFormulaModifiers The DamageFormulaModifiers to write.
				@return The supplied stream after writing the representation.
				@since 0.12.45
				@version 0.12.45
			*/
			friend std::ostream &operator<<(std::ostream &outStream, const DamageFormulaModifiers &damageFormulaModifiers);

		public:
			/*! @brief The multiplicative modifier applied to health. */
			double mHealthModifier{1.0};
			/*! @brief The multiplicative modifier applied to Attack. */
			double mAttackModifier{1.0};
			/*! @brief The multiplicative modifier applied to Defense. */
			double mDefenseModifier{1.0};
			/*! @brief The multiplicative modifier applied to Special Attack. */
			double mSpecialAttackModifier{1.0};
			/*! @brief The multiplicative modifier applied to Special Defense. */
			double mSpecialDefenseModifier{1.0};
			/*! @brief The multiplicative modifier applied to Speed. */
			double mSpeedModifier{1.0};
	};

	/*! @struct BattleSlot Battle/battleState.h
		@brief Stores the active battle state associated with one position on a side.
		@details The Pokemon pointer is a non-owning reference to the party member occupying the slot and may be nullptr when the position
	   is empty.
		@warning The owner of the referenced @ref Pokemon is responsible for keeping it alive while mPokemon is in use.
		@date 09/29/2026
		@since 0.3.0
		@version 0.12.46
		@author Matthew Moore
	*/
	struct BattleSlot
	{
		public:
			/*! @brief Applies a registered volatile status according to its interactions with the current statuses.
				@details Blocking interactions leave the array unchanged. Replacement interactions store the incoming status in place, while
			   removal interactions clear matching statuses and compact the remaining active statuses before insertion. A status whose
			   registry metadata classifies it as @ref PocketCore::Status::StatusClassification::NonVolatile is Pokemon-owned and is
			   rejected here, leaving the battle-slot array unchanged.
				@param[in] statusID The registered status identifier to apply. @ref NO_STATUS_ID is ignored.
				@param[in] statusRegistry The registry used to resolve the incoming status metadata.
				@param[in] policy The ruleset policy governing status interactions, including the maximum number of active statuses and
			   replacement behavior when full.
				@return An empty result when the status is applied or is a benign no-op; otherwise the @ref
			   PocketCore::Status::InteractionApplicationError describing the rejection.
				@since 0.12.46
				@version 0.12.46
			*/
			constexpr std::expected<void, InteractionApplicationError> addVolatileStatus(const StatusID statusID,
																					const StatusRegistry &statusRegistry,
																					const RulesetPolicy &policy)
			{
				const StatusMeta *metadata{statusRegistry.getStatusMetadata(statusID)};

				// A non-volatile status belongs to the Pokemon; it must never occupy battle-slot storage.
				if (metadata != nullptr && metadata->mStatusClassification == StatusClassification::NonVolatile)
				{
					return std::unexpected(InteractionApplicationError::WrongClassification);
				}

				std::array<StatusID, MAX_VOLATILE_STATUSES_PER_POKEMON> volatileStatusIDs{};
				std::ranges::transform(mVolatileStatuses, volatileStatusIDs.begin(),
									   [](const VolatileStatus &volatileStatus) { return volatileStatus.mStatusID; });

				const std::expected<void, InteractionApplicationError> result{
					applyInteractions(statusID, NO_STATUS_ID, statusRegistry, volatileStatusIDs, &StatusMeta::mStatusInteractions,
									  policy.mMaxVolatileStatuses, policy.mReplaceVolatileStatusWhenFull),
				};

				std::ranges::transform(volatileStatusIDs, mVolatileStatuses.begin(),
									   [](const StatusID volatileStatusID) { return VolatileStatus{volatileStatusID}; });

				return result;
			}

			/*! @brief Writes the BattleSlot's raw identifier and statistic representation to a stream.
				@param[in,out] outStream The stream receiving the representation.
				@param[in] battleSlot The BattleSlot to write.
				@return The supplied stream after writing the representation.
				@since 0.12.45
				@version 0.12.45
			*/
			friend std::ostream &operator<<(std::ostream &outStream, const BattleSlot &battleSlot);

		public:
			// NOLINTBEGIN(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)

			/*! @brief The temporary modifiers used by damage and battle calculations. */
			DamageFormulaModifiers mDamageFormulaModifiers{};

			/*! @brief The volatile statuses currently affecting this slot's Pokemon. */
			std::array<VolatileStatus, MAX_VOLATILE_STATUSES_PER_POKEMON> mVolatileStatuses{};

			/*! @brief The non-owning Pokemon occupying this slot, or nullptr when unoccupied. */
			Pokemon *mPokemon{nullptr};
			/*! @brief The temporary stat stages for the occupying Pokemon. */
			StatStages mStatStages{};

			/*! @brief The move currently locking this slot's Pokemon into a choice, if any. */
			MoveID mChoiceLockedMove{};

			/*! @brief The side-local position represented by this slot. */
			ub mPosition{0};

			/*! @brief The remaining sleep counter for this slot. */
			ub mSleepCounter{0};
			/*! @brief The current toxic counter for this slot. */
			ub mToxicCounter{0};
			/*! @brief The remaining protection counter for this slot. */
			ub mProtectionCounter{0};

			/*! @brief Indicates whether this slot is protected from applicable effects. */
			bool mIsProtected{false};
			/*! @brief Indicates whether this slot's Pokemon is flinched. */
			bool mIsFlinched{false};
			/*! @brief Indicates whether this slot's Pokemon is grounded. */
			bool mIsGrounded{false};
			/*! @brief Indicates whether faint processing has already occurred for this slot. */
			bool mFaintProcessed{false};

			// NOLINTEND(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)
	};

	/*! @struct BattleState Battle/battleState.h
		@brief Stores the complete mutable state of a battle.
		@details The side vectors own their @ref BattleSlot values. The party vectors contain non-owning pointers to Pokemon objects.
	   Weather, terrain, entry hazards, and battle-start state are stored alongside the active side and party information.
		@warning BattleState does not own the Pokemon objects referenced by mPartyA, mPartyB, or the BattleSlot mPokemon members.
		@date 09/23/2026
		@since 0.3.0
		@version 0.12.43
		@author Matthew Moore
	*/
	struct BattleState
	{
		public:
			/*! @brief Constructs an empty battle state with default-initialized battle data.
				@since 0.10.3
				@version 0.12.7
			*/
			BattleState() = default;

			/*! @brief Constructs a battle state by copying all stored battle data.
				@note Pokemon pointers remain non-owning references in the copied state.
				@since 0.10.3
				@version 0.12.7
			*/
			BattleState(const BattleState &) = default;

			/*! @brief Constructs a battle state by moving stored battle data from another state.
				@note Pokemon pointers remain non-owning references in the moved state.
				@since 0.10.3
				@version 0.12.7
			*/
			BattleState(BattleState &&) noexcept = default;

			/*! @brief Replaces this battle state with a copy of another battle state.
				@note Pokemon pointers remain non-owning references after assignment.
				@since 0.10.3
				@version 0.12.7
			*/
			BattleState &operator=(const BattleState &) = default;

			/*! @brief Replaces this battle state by moving data from another battle state.
				@note Pokemon pointers remain non-owning references after assignment.
				@since 0.10.3
				@version 0.12.7
			*/
			BattleState &operator=(BattleState &&) noexcept = default;

			/*! @brief Destroys the battle state and releases storage owned by its value members.
				@note The Pokemon objects referenced by the state are not owned or destroyed by this operation.
				@since 0.10.3
				@version 0.12.7
			*/
			~BattleState() noexcept;

			// NOLINTBEGIN(misc-non-private-member-variables-in-classes)

			/*! @brief The active slots for side A. */
			std::vector<BattleSlot> mSideA{};
			/*! @brief The active slots for side B. */
			std::vector<BattleSlot> mSideB{};

			/*! @brief Non-owning pointers to side A's party Pokemon. */
			std::vector<Pokemon *> mPartyA{};
			/*! @brief Non-owning pointers to side B's party Pokemon. */
			std::vector<Pokemon *> mPartyB{};

			/*! @brief The battle-wide weather identifiers. */
			std::array<WeatherID, MAX_ACTIVE_WEATHERS_ON_FIELD> mWeatherIDs{};
			/*! @brief The battle-wide terrain identifiers. */
			std::array<TerrainID, MAX_ACTIVE_TERRAINS_ON_FIELD> mTerrainIDs{};

			/*! @brief The ruleset policy for this slot. */
			RulesetPolicy mRuleset{};

			// Spikes can have 0-3 layers

			/*! @brief The number of Spikes layers affecting side A, from 0 to 3. */
			ub mSpikesPartyA{0};
			/*! @brief The number of Spikes layers affecting side B, from 0 to 3. */
			ub mSpikesPartyB{0};

			// Spikes can have 0-2 layers

			/*! @brief The number of Toxic Spikes layers affecting side A, from 0 to 2. */
			ub mToxicSpikesPartyA{0};
			/*! @brief The number of Toxic Spikes layers affecting side B, from 0 to 2. */
			ub mToxicSpikesPartyB{0};

			/*! @brief Indicates whether Stealth Rock affects side A. */
			bool mStealthRockPartyA{false};
			/*! @brief Indicates whether Stealth Rock affects side B. */
			bool mStealthRockPartyB{false};

			/*! @brief Indicates whether battle-start processing has completed. */
			bool mBattleStarted{false};

			// NOLINTEND(misc-non-private-member-variables-in-classes)
	};
} // namespace PocketCore::Battle

#endif
