/*! @file battleState.cpp
	@brief Defines out-of-line operations for battle state storage.
	@date 09/28/2026
	@since 0.10.3
	@version 0.12.45
	@author Matthew Moore
*/

#include "Battle/battleState.h"

#include <ios>
#include <sstream>

#include "Battle/battleHelpers.h"
#include "Core/typedefs.h"

namespace PocketCore::Battle
{
	using PocketCore::Core::us;

	BattleState::~BattleState() noexcept = default;

#if ATTR_ONLY_GCC
	// GCC suggests returns_nonnull for references even though the attribute accepts only pointer returns.
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wsuggest-attribute=returns_nonnull"
#endif

	std::ostream &operator<<(std::ostream &outStream, const StatStages &statStages)
	{
		outStream << "Stat Stages {\n"
				  << "  Attack: " << static_cast<us>(statStages.mAttack) << '\n'
				  << "  Defense: " << static_cast<us>(statStages.mDefense) << '\n'
				  << "  Special Attack: " << static_cast<us>(statStages.mSpAttack) << '\n'
				  << "  Special Defense: " << static_cast<us>(statStages.mSpDefense) << '\n'
				  << "  Speed: " << static_cast<us>(statStages.mSpeed) << '\n'
				  << "  Accuracy: " << static_cast<us>(statStages.mAccuracy) << '\n'
				  << "  Evasion: " << static_cast<us>(statStages.mEvasion) << '\n';

		outStream << '}';

		return outStream;
	}

	std::ostream &operator<<(std::ostream &outStream, const DamageFormulaModifiers &damageFormulaModifiers)
	{
		outStream << "Damage Formula Modifiers {\n"
				  << "  Health: " << damageFormulaModifiers.mHealthModifier << '\n'
				  << "  Attack: " << damageFormulaModifiers.mAttackModifier << '\n'
				  << "  Defense: " << damageFormulaModifiers.mDefenseModifier << '\n'
				  << "  Special Attack: " << damageFormulaModifiers.mSpecialAttackModifier << '\n'
				  << "  Special Defense: " << damageFormulaModifiers.mSpecialDefenseModifier << '\n'
				  << "  Speed: " << damageFormulaModifiers.mSpeedModifier << '\n';

		outStream << '}';

		return outStream;
	}

	std::ostream &operator<<(std::ostream &outStream, const BattleSlot &battleSlot)
	{
		std::ostringstream modifiersStream;
		modifiersStream << battleSlot.mDamageFormulaModifiers;

		std::ostringstream pokemonStream;
		pokemonStream << *battleSlot.mPokemon;

		std::ostringstream statStagesStream;
		statStagesStream << battleSlot.mStatStages;

		outStream << "Battle Slot {\n" << indentBlock(modifiersStream.str()) << '\n';
		outStream << "  Volatile Status IDs: [";

		for (std::size_t index{0}; index < battleSlot.mVolatileStatuses.size(); ++index)
		{
			outStream << (index == 0U ? "" : ", ") << battleSlot.mVolatileStatuses.at(static_cast<us>(index)).mStatusID.getValue();
		}

		outStream << std::boolalpha << "]\n"
				  << indentBlock(pokemonStream.str()) << '\n'
				  << indentBlock(statStagesStream.str()) << '\n'
				  << "  Choice Locked Move ID: " << battleSlot.mChoiceLockedMove.getValue() << '\n'
				  << "  Position: " << static_cast<us>(battleSlot.mPosition) << '\n'
				  << "  Sleep: " << static_cast<us>(battleSlot.mSleepCounter) << '\n'
				  << "  Toxic: " << static_cast<us>(battleSlot.mToxicCounter) << '\n'
				  << "  Protection: " << static_cast<us>(battleSlot.mProtectionCounter) << '\n'
				  << "  Is Protected: " << battleSlot.mIsProtected << '\n'
				  << "  Is Flinched: " << battleSlot.mIsFlinched << '\n'
				  << "  Is Grounded: " << battleSlot.mIsGrounded << '\n'
				  << "  Faint Processed: " << battleSlot.mFaintProcessed << '\n';

		outStream << '}';

		return outStream;
	}

#if ATTR_ONLY_GCC
	#pragma GCC diagnostic pop
#endif
} // namespace PocketCore::Battle
