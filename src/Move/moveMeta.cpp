/*! \file moveMeta.cpp
	\brief Contains the function definitions for creating a moveMeta
	\date --/--/----
	\version x.x.x
	\since x.x.x
	\author Matthew Moore
*/

#include "Move/moveMeta.h"

#include <array>
#include <span>

#include "Core/attributeMacros.h"
#include "Effect/builtInEffectID.h"

namespace PocketCore::Move
{
	using PocketCore::Effect::BuiltinEffectID;

	constexpr std::array<BuiltinEffectID, 10> BASE_ATTACK_EFFECTS{
		BuiltinEffectID::PsychicTerrainPriorityBlock,
		BuiltinEffectID::CriticalHit,
		BuiltinEffectID::BaseDamage,
		BuiltinEffectID::Targets,
		BuiltinEffectID::Weather,
		BuiltinEffectID::Terrain,
		BuiltinEffectID::Randomization,
		BuiltinEffectID::Stab,
		BuiltinEffectID::TypeEffectiveness,
		BuiltinEffectID::BurnDamageReduction,
	};

	constexpr std::array<BuiltinEffectID, 11> BASE_ATTACK_WITH_RECOIL{
		BuiltinEffectID::PsychicTerrainPriorityBlock,
		BuiltinEffectID::CriticalHit,
		BuiltinEffectID::BaseDamage,
		BuiltinEffectID::Targets,
		BuiltinEffectID::Weather,
		BuiltinEffectID::Terrain,
		BuiltinEffectID::Randomization,
		BuiltinEffectID::Stab,
		BuiltinEffectID::TypeEffectiveness,
		BuiltinEffectID::BurnDamageReduction,
		BuiltinEffectID::Recoil,
	};

	constexpr std::array<BuiltinEffectID, 11> BASE_ATTACK_WITH_STATUS{
		BuiltinEffectID::PsychicTerrainPriorityBlock,
		BuiltinEffectID::CriticalHit,
		BuiltinEffectID::BaseDamage,
		BuiltinEffectID::Targets,
		BuiltinEffectID::Weather,
		BuiltinEffectID::Terrain,
		BuiltinEffectID::Randomization,
		BuiltinEffectID::Stab,
		BuiltinEffectID::TypeEffectiveness,
		BuiltinEffectID::BurnDamageReduction,
		BuiltinEffectID::StatusApply,
	};

	constexpr std::array<BuiltinEffectID, 11> BASE_ATTACK_WITH_FLINCH{
		BuiltinEffectID::PsychicTerrainPriorityBlock,
		BuiltinEffectID::CriticalHit,
		BuiltinEffectID::BaseDamage,
		BuiltinEffectID::Targets,
		BuiltinEffectID::Weather,
		BuiltinEffectID::Terrain,
		BuiltinEffectID::Randomization,
		BuiltinEffectID::Stab,
		BuiltinEffectID::TypeEffectiveness,
		BuiltinEffectID::BurnDamageReduction,
		BuiltinEffectID::Flinch,
	};

	ATTR_CONST std::span<const BuiltinEffectID> baseAttackEffects() noexcept
	{
		return BASE_ATTACK_EFFECTS;
	}

	ATTR_CONST std::span<const BuiltinEffectID> baseAttackWithRecoil() noexcept
	{
		return BASE_ATTACK_WITH_RECOIL;
	}

	ATTR_CONST std::span<const BuiltinEffectID> baseAttackWithStatus() noexcept
	{
		return BASE_ATTACK_WITH_STATUS;
	}

	ATTR_CONST std::span<const BuiltinEffectID> baseAttackWithFlinch() noexcept
	{
		return BASE_ATTACK_WITH_FLINCH;
	}
} // namespace PocketCore::Move
