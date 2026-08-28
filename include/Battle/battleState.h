/*! @file battleState.h
	@brief Contains the battle state
	@date 08/14/2026
	@since 0.3.0
	@version 0.10.3
	@author Matthew Moore
*/

#ifndef INCLUDE_BATTLE_BATTLESTATE_H
#define INCLUDE_BATTLE_BATTLESTATE_H

#include <vector>

#include "Core/typedefs.h"
#include "Move/moveID.h"
#include "Pokemon/pokemon.h"
#include "Terrain/terrainID.h"
#include "Weather/weatherID.h"

namespace PocketCore::Battle
{
	using PocketCore::Core::sb;
	using PocketCore::Core::ub;
	using PocketCore::Core::us;
	using PocketCore::Move::MoveID;
	using PocketCore::Pokemon::Pokemon;
	using PocketCore::Terrain::TerrainID;
	using PocketCore::Weather::WeatherID;

	/*! @struct StatStages Battle/battleState.h
		@brief Stores a battler's temporary stat stage changes.
		@details Each signed stage applies to the corresponding stat during battle calculations.
		@date 07/26/2026
		@since 0.3.0
		@version 0.7.2
		@author Matthew Moore
	*/
	struct StatStages
	{
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
		@date 08/04/2026
		@since 0.8.5
		@version 0.9.10
		@author Matthew Moore
	*/
	struct DamageFormulaModifiers
	{
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
		@date 08/07/2026
		@since 0.3.0
		@version 0.9.16
		@author Matthew Moore
	*/
	struct BattleSlot
	{
		public:
			/*! @brief The temporary modifiers used by damage and battle calculations. */
			DamageFormulaModifiers mDamageFormulaModifiers{};

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
	};

	/*! @struct BattleState Battle/battleState.h
		@brief Stores the complete mutable state of a battle.
		@details The side vectors own their @ref BattleSlot values. The party vectors contain non-owning pointers to Pokemon objects.
	   Weather, terrain, entry hazards, and battle-start state are stored alongside the active side and party information.
		@warning BattleState does not own the Pokemon objects referenced by mPartyA, mPartyB, or the BattleSlot mPokemon members.
		@date 08/14/2026
		@since 0.3.0
		@version 0.10.3
		@author Matthew Moore
	*/
	struct BattleState
	{
		public:
			/*! @brief Constructs an empty battle state with default-initialized battle data.
				@since 0.10.3
				@version 0.10.3
			*/
			BattleState() = default;

			/*! @brief Constructs a battle state by copying all stored battle data.
				@note Pokemon pointers remain non-owning references in the copied state.
				@since 0.10.3
				@version 0.10.3
			*/
			BattleState(const BattleState &) = default;

			/*! @brief Constructs a battle state by moving stored battle data from another state.
				@note Pokemon pointers remain non-owning references in the moved state.
				@since 0.10.3
				@version 0.10.3
			*/
			BattleState(BattleState &&) noexcept = default;

			/*! @brief Replaces this battle state with a copy of another battle state.
				@note Pokemon pointers remain non-owning references after assignment.
				@since 0.10.3
				@version 0.10.3
			*/
			BattleState &operator=(const BattleState &) = default;

			/*! @brief Replaces this battle state by moving data from another battle state.
				@note Pokemon pointers remain non-owning references after assignment.
				@since 0.10.3
				@version 0.10.3
			*/
			BattleState &operator=(BattleState &&) noexcept = default;

			/*! @brief Destroys the battle state and releases storage owned by its value members.
				@note The Pokemon objects referenced by the state are not owned or destroyed by this operation.
				@since 0.10.3
				@version 0.10.3
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

			/*! @brief The battle-wide weather identifier. */
			WeatherID mWeatherID{};
			/*! @brief The battle-wide terrain identifier. */
			TerrainID mTerrainID{};

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
