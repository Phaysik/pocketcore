/*! @file moveMeta.cpp
	@brief Contains the function definitions for creating a moveMeta
	@date 08/03/2026
	@since 0.5.3
	@version 0.9.0
	@author Matthew Moore
*/

#include "Move/moveMeta.h"

#include <array>
#include <span>

#include "Core/attributeMacros.h"
#include "Effect/builtInEffectID.h"
#include "Effect/effectID.h"

namespace PocketCore::Move
{
	using PocketCore::Effect::BuiltinEffectID;
	using PocketCore::Effect::EffectID;
	using PocketCore::Effect::toEffectID;

	constexpr std::array<EffectID, 10> BASE_ATTACK_EFFECTS{
		toEffectID(BuiltinEffectID::PsychicTerrainPriorityBlock),
		toEffectID(BuiltinEffectID::CriticalHit),
		toEffectID(BuiltinEffectID::BaseDamage),
		toEffectID(BuiltinEffectID::Targets),
		toEffectID(BuiltinEffectID::Weather),
		toEffectID(BuiltinEffectID::Terrain),
		toEffectID(BuiltinEffectID::Randomization),
		toEffectID(BuiltinEffectID::Stab),
		toEffectID(BuiltinEffectID::TypeEffectiveness),
		toEffectID(BuiltinEffectID::BurnDamageReduction),
	};

	constexpr std::array<EffectID, 11> BASE_ATTACK_WITH_RECOIL{
		toEffectID(BuiltinEffectID::PsychicTerrainPriorityBlock),
		toEffectID(BuiltinEffectID::CriticalHit),
		toEffectID(BuiltinEffectID::BaseDamage),
		toEffectID(BuiltinEffectID::Targets),
		toEffectID(BuiltinEffectID::Weather),
		toEffectID(BuiltinEffectID::Terrain),
		toEffectID(BuiltinEffectID::Randomization),
		toEffectID(BuiltinEffectID::Stab),
		toEffectID(BuiltinEffectID::TypeEffectiveness),
		toEffectID(BuiltinEffectID::BurnDamageReduction),
		toEffectID(BuiltinEffectID::Recoil),
	};

	constexpr std::array<EffectID, 11> BASE_ATTACK_WITH_STATUS{
		toEffectID(BuiltinEffectID::PsychicTerrainPriorityBlock),
		toEffectID(BuiltinEffectID::CriticalHit),
		toEffectID(BuiltinEffectID::BaseDamage),
		toEffectID(BuiltinEffectID::Targets),
		toEffectID(BuiltinEffectID::Weather),
		toEffectID(BuiltinEffectID::Terrain),
		toEffectID(BuiltinEffectID::Randomization),
		toEffectID(BuiltinEffectID::Stab),
		toEffectID(BuiltinEffectID::TypeEffectiveness),
		toEffectID(BuiltinEffectID::BurnDamageReduction),
		toEffectID(BuiltinEffectID::StatusApply),
	};

	constexpr std::array<EffectID, 11> BASE_ATTACK_WITH_FLINCH{
		toEffectID(BuiltinEffectID::PsychicTerrainPriorityBlock),
		toEffectID(BuiltinEffectID::CriticalHit),
		toEffectID(BuiltinEffectID::BaseDamage),
		toEffectID(BuiltinEffectID::Targets),
		toEffectID(BuiltinEffectID::Weather),
		toEffectID(BuiltinEffectID::Terrain),
		toEffectID(BuiltinEffectID::Randomization),
		toEffectID(BuiltinEffectID::Stab),
		toEffectID(BuiltinEffectID::TypeEffectiveness),
		toEffectID(BuiltinEffectID::BurnDamageReduction),
		toEffectID(BuiltinEffectID::Flinch),
	};

	ATTR_CONST std::span<const EffectID> baseAttackEffects() noexcept
	{
		return BASE_ATTACK_EFFECTS;
	}

	ATTR_CONST std::span<const EffectID> baseAttackWithRecoil() noexcept
	{
		return BASE_ATTACK_WITH_RECOIL;
	}

	ATTR_CONST std::span<const EffectID> baseAttackWithStatus() noexcept
	{
		return BASE_ATTACK_WITH_STATUS;
	}

	ATTR_CONST std::span<const EffectID> baseAttackWithFlinch() noexcept
	{
		return BASE_ATTACK_WITH_FLINCH;
	}
} // namespace PocketCore::Move
