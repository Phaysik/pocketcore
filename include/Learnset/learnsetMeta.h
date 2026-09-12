/*! @file learnsetMeta.h
	@brief Defines the metadata stored for built-in and user-defined learnsets.
	@date 09/11/2026
	@since 0.12.24
	@version 0.12.24
	@author Matthew Moore
*/

#ifndef INCLUDE_LEARNSET_LEARNSETMETA_H
#define INCLUDE_LEARNSET_LEARNSETMETA_H

#include <string>
#include <vector>

#include "Core/typedefs.h"
#include "Move/moveID.h"

#include "learnsetID.h"

namespace PocketCore::Learnset
{
	using PocketCore::Core::ub;
	using PocketCore::Move::MoveID;

	/*! @enum AcquisitionMethod
		@brief Defines the method by which a learnset is acquired.
		@since 0.12.24
		@version 0.12.24
		@author Matthew Moore
	*/
	enum class AcquisitionMethod : ub
	{
		LevelUp,
		TM,
		Egg,
		Tutor,
	};

	/*! @struct MoveLearnEntry Learnset/learnsetMeta.h
		@brief Stores a move's ID, level requirement, and acquisition method.
		@since 0.12.24
		@version 0.12.24
		@author Matthew Moore
	*/
	struct MoveLearnEntry
	{
		public:
			MoveID mMoveID{};
			ub mLevelRequirement{1};
			AcquisitionMethod mAcquisitionMethod{AcquisitionMethod::LevelUp};
	};

	/*! @struct LearnsetMeta Learnset/learnsetMeta.h
		@brief Stores one learnset's stable ID, display name, and owned trigger definitions.
		@details The trigger vector owns its elements and their effect vectors. The display name is a non-owning view whose backing storage
	   must remain valid while this metadata is registered.
		@date 09/10/2026
		@since 0.12.24
		@version 0.12.24
		@author Matthew Moore
	*/
	struct LearnsetMeta
	{
		public:
			/*! @brief Compares two LearnsetMeta instances for equivalent metadata.
				@details Compares all fields exactly.
				@param[in] other The LearnsetMeta instance to compare.
				@return True when both instances contain equivalent metadata; otherwise false.
				@since 0.12.24
				@version 0.12.24
			*/
			ATTR_NODISCARD constexpr bool operator==(const LearnsetMeta &other) const noexcept = default;

			// NOLINTBEGIN(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)

			/*! @brief The case-sensitive display name stored */
			std::string mName{};

			/*! @brief The move learn entries for this learnset. */
			std::vector<MoveLearnEntry> mEntries{};

			/*! @brief The stable built-in or user-assigned identifier. */
			LearnsetID mLearnsetID{};

			// NOLINTEND(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)
	};
} // namespace PocketCore::Learnset

#endif
