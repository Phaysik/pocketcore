/*! @file moveRegistryConfiguration.cpp
	@brief Defines validated user customization operations for the move registry.
	@date 07/27/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#include "Configuration/moveRegistryConfiguration.h"

#include <expected>
#include <span>
#include <string_view>
#include <vector>

#include "Battle/battleTargetsAndTriggers.h"
#include "Core/attributeMacros.h"
#include "Effect/effectTrigger.h"
#include "Move/moveID.h"
#include "Move/moveMeta.h"

namespace PocketCore::Configuration
{
	using PocketCore::Battle::BattleTargetID;
	using PocketCore::Move::MoveID;
	using PocketCore::Move::MoveMeta;

	using PocketCore::Effect::EffectTrigger;

	ATTR_NODISCARD std::expected<MoveID, RegistryErrorInfo> MoveRegistryConfiguration::addMove(const MoveMeta &moveMeta)
	{
		return addMetadata(moveMeta);
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> MoveRegistryConfiguration::addMoves(const std::span<const MoveMeta> &moveMetas)
	{
		return addMetadataBatch(moveMetas, [](const MoveMeta &definition) { return MoveMeta{definition}; });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> MoveRegistryConfiguration::setMoveTriggers(
		const std::string_view &moveName, const std::span<const EffectTrigger> &triggers)
	{
		return mutateMetadata(moveName, "setMoveTriggers",
							  [&triggers](MoveMeta &metadata) { metadata.mTriggers.assign(triggers.begin(), triggers.end()); });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> MoveRegistryConfiguration::setMoveTriggers(
		const MoveID moveID, const std::span<const EffectTrigger> &triggers)
	{
		return mutateMetadata(moveID, "setMoveTriggers",
							  [&triggers](MoveMeta &metadata) { metadata.mTriggers.assign(triggers.begin(), triggers.end()); });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> MoveRegistryConfiguration::setMoveTarget(const std::string_view &moveName,
																								   const BattleTargetID target)
	{
		return mutateMetadata(moveName, "setMoveTarget", [&target](MoveMeta &metadata) { metadata.mTargetID = target; });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> MoveRegistryConfiguration::setMoveTarget(const MoveID moveID,
																								   const BattleTargetID target)
	{
		return mutateMetadata(moveID, "setMoveTarget", [&target](MoveMeta &metadata) { metadata.mTargetID = target; });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> MoveRegistryConfiguration::renameMove(const std::string_view &oldName,
																								const std::string_view &newName)
	{
		return renameMetadata(oldName, newName);
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> MoveRegistryConfiguration::updateMove(const std::string_view &moveName,
																								const MoveMeta &moveMeta)
	{
		return mutateMetadata(moveName, "updateMove", [&moveMeta](MoveMeta &metadata) {
			metadata.mHitCountPolicy = moveMeta.mHitCountPolicy;
			metadata.mTriggers = moveMeta.mTriggers;
			metadata.mName = moveMeta.mName;
			metadata.mMoveID = moveMeta.mMoveID;
			metadata.mTypeID = moveMeta.mTypeID;
			metadata.mPower = moveMeta.mPower;
			metadata.mTargetID = moveMeta.mTargetID;
			metadata.mRangeID = moveMeta.mRangeID;
			metadata.mAccuracy = moveMeta.mAccuracy;
			metadata.mPriority = moveMeta.mPriority;
			metadata.mSpecial = moveMeta.mSpecial;
		});
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> MoveRegistryConfiguration::updateMove(const MoveID moveID,
																								const MoveMeta &moveMeta)
	{
		return mutateMetadata(moveID, "updateMove", [&moveMeta](MoveMeta &metadata) {
			metadata.mHitCountPolicy = moveMeta.mHitCountPolicy;
			metadata.mTriggers = moveMeta.mTriggers;
			metadata.mName = moveMeta.mName;
			metadata.mMoveID = moveMeta.mMoveID;
			metadata.mTypeID = moveMeta.mTypeID;
			metadata.mPower = moveMeta.mPower;
			metadata.mTargetID = moveMeta.mTargetID;
			metadata.mRangeID = moveMeta.mRangeID;
			metadata.mAccuracy = moveMeta.mAccuracy;
			metadata.mPriority = moveMeta.mPriority;
			metadata.mSpecial = moveMeta.mSpecial;
		});
	}

	ATTR_NODISCARD std::expected<MoveID, RegistryErrorInfo> MoveRegistryConfiguration::removeMove(const std::string_view &moveName)
	{
		return removeMetadata(moveName);
	}

	ATTR_NODISCARD std::expected<MoveID, RegistryErrorInfo> MoveRegistryConfiguration::removeMove(const MoveID moveID)
	{
		return removeMetadata(moveID);
	}
} // namespace PocketCore::Configuration
