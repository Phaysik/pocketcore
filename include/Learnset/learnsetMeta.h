/*! @file learnsetMeta.h
	@brief Defines the metadata stored for built-in and user-defined learnsets.
	@date 10/08/2026
	@since 0.12.24
	@version 0.12.50
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
		@brief Defines the method by which a move is acquired.
		@showenumvalues
		@since 0.12.24
		@version 0.12.50
		@author Matthew Moore
	*/
	enum class AcquisitionMethod : ub
	{
		/*! @brief Learns the move by leveling up. */
		LevelUp,
		/*! @brief Learns the move using a Technical Machine (TM). */
		TM,
		/*! @brief Learns the move as an egg move, typically inherited through breeding. */
		Egg,
		/*! @brief Learns the move from a move tutor. */
		Tutor,
	};

	/*! @struct MoveLearnEntry Learnset/learnsetMeta.h
		@brief Stores a move's ID, level requirement, and acquisition method.
		@date 10/08/2026
		@since 0.12.24
		@version 0.12.50
		@author Matthew Moore
	*/
	struct MoveLearnEntry
	{
		public:
			/*! @brief Compares two MoveLearnEntry instances for equivalent metadata.
				@details Compares all fields exactly.
				@param[in] other The MoveLearnEntry instance to compare.
				@return True when both instances contain equivalent metadata; otherwise false.
				@since 0.12.50
				@version 0.12.50
			*/
			ATTR_NODISCARD constexpr bool operator==(const MoveLearnEntry &other) const noexcept = default;

			// NOLINTBEGIN(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)

			/*! @brief Identifies the move made available by this learnset entry. */
			MoveID mMoveID{};

			/*! @brief Stores the level requirement for acquiring the move.
				@details Records metadata only; this struct does not validate or enforce the requirement.
			*/
			ub mLevelRequirement{1};

			/*! @brief Records the @ref AcquisitionMethod used to acquire the move. */
			AcquisitionMethod mAcquisitionMethod{AcquisitionMethod::LevelUp};

			// NOLINTEND(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)
	};

	/*! @struct LearnsetMeta Learnset/learnsetMeta.h
		@brief Stores one learnset's stable ID, owned display name, and owned move learn entries.
		@details The string owns the display name and the vector owns its move learn entries. Copying this metadata creates independent
	   copies of both, so the source name and entries do not need to outlive the copy stored in a registry.
		@date 10/08/2026
		@since 0.12.24
		@version 0.12.50
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
				@version 0.12.50
			*/
			ATTR_NODISCARD constexpr bool operator==(const LearnsetMeta &other) const = default;

			// NOLINTBEGIN(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)

			/*! @brief Owns the case-sensitive display name. */
			std::string mName{};

			/*! @brief Owns the move learn entries for this learnset. */
			std::vector<MoveLearnEntry> mEntries{};

			/*! @brief The stable built-in or user-assigned identifier. */
			LearnsetID mLearnsetID{};

			// NOLINTEND(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)
	};
} // namespace PocketCore::Learnset

#endif
