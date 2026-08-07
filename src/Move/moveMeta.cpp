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
#include "Effect/effectType.h"

namespace PocketCore::Move
{
	using PocketCore::Effect::EffectTypeID;

	constexpr std::array<EffectTypeID, 10> BASE_ATTACK_EFFECTS{
		EffectTypeID::PsychicTerrainPriorityBlock,
		EffectTypeID::CriticalHit,
		EffectTypeID::BaseDamage,
		EffectTypeID::Targets,
		EffectTypeID::Weather,
		EffectTypeID::Terrain,
		EffectTypeID::Randomization,
		EffectTypeID::Stab,
		EffectTypeID::TypeEffectiveness,
		EffectTypeID::BurnDamageReduction,
	};

	constexpr std::array<EffectTypeID, 11> BASE_ATTACK_WITH_RECOIL{
		EffectTypeID::PsychicTerrainPriorityBlock,
		EffectTypeID::CriticalHit,
		EffectTypeID::BaseDamage,
		EffectTypeID::Targets,
		EffectTypeID::Weather,
		EffectTypeID::Terrain,
		EffectTypeID::Randomization,
		EffectTypeID::Stab,
		EffectTypeID::TypeEffectiveness,
		EffectTypeID::BurnDamageReduction,
		EffectTypeID::Recoil,
	};

	constexpr std::array<EffectTypeID, 11> BASE_ATTACK_WITH_STATUS{
		EffectTypeID::PsychicTerrainPriorityBlock,
		EffectTypeID::CriticalHit,
		EffectTypeID::BaseDamage,
		EffectTypeID::Targets,
		EffectTypeID::Weather,
		EffectTypeID::Terrain,
		EffectTypeID::Randomization,
		EffectTypeID::Stab,
		EffectTypeID::TypeEffectiveness,
		EffectTypeID::BurnDamageReduction,
		EffectTypeID::StatusApply,
	};

	constexpr std::array<EffectTypeID, 11> BASE_ATTACK_WITH_FLINCH{
		EffectTypeID::PsychicTerrainPriorityBlock,
		EffectTypeID::CriticalHit,
		EffectTypeID::BaseDamage,
		EffectTypeID::Targets,
		EffectTypeID::Weather,
		EffectTypeID::Terrain,
		EffectTypeID::Randomization,
		EffectTypeID::Stab,
		EffectTypeID::TypeEffectiveness,
		EffectTypeID::BurnDamageReduction,
		EffectTypeID::Flinch,
	};

	constexpr std::array<EffectTypeID, 2> STATUS_CHANGE_EFFECTS{
		EffectTypeID::PsychicTerrainPriorityBlock,
		EffectTypeID::StatChange,
	};

	ATTR_CONST std::span<const EffectTypeID> baseAttackEffects() noexcept
	{
		return BASE_ATTACK_EFFECTS;
	}

	ATTR_CONST std::span<const EffectTypeID> baseAttackWithRecoil() noexcept
	{
		return BASE_ATTACK_WITH_RECOIL;
	}

	ATTR_CONST std::span<const EffectTypeID> baseAttackWithStatus() noexcept
	{
		return BASE_ATTACK_WITH_STATUS;
	}

	ATTR_CONST std::span<const EffectTypeID> baseAttackWithFlinch() noexcept
	{
		return BASE_ATTACK_WITH_FLINCH;
	}

	ATTR_CONST std::span<const EffectTypeID> statusChangeEffects() noexcept
	{
		return STATUS_CHANGE_EFFECTS;
	}
} // namespace PocketCore::Move
