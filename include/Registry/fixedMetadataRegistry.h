/*! @file fixedMetadataRegistry.h
	@brief Provides shared fixed-capacity storage and lookup for metadata registries.
	@date 09/03/2026
	@since 0.5.0
	@version 0.12.19
	@author Matthew Moore
*/

#ifndef INCLUDE_REGISTRY_FIXED_METADATA_REGISTRY_H
#define INCLUDE_REGISTRY_FIXED_METADATA_REGISTRY_H

#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <optional>
#include <span>
#include <string_view>
#include <utility>

#include "Core/attributeMacros.h"
#include "Core/typedefs.h"

namespace PocketCore::Registry
{
	using PocketCore::Core::us;

	/*! @class FixedMetadataRegistry Registry/fixedMetadataRegistry.h
		@brief Stores named metadata with stable IDs in fixed-capacity contiguous storage.
		@details Centralizes storage, counters, linear lookup, and mutation operations shared by registries such as abilities and items.
	   Domain registries remain responsible for constructing built-in metadata and exposing domain-specific API names.
		@tparam Metadata The metadata record type, including an mName member convertible to std::string_view.
		@tparam StableID The strongly typed stable identifier stored by each metadata record.
		@tparam Capacity The maximum number of metadata records stored by the registry.
		@tparam IDMember A pointer to the StableID member within Metadata.
		@note Stable-ID lookups are O(log n), while name lookups are O(n). Storage operations do not allocate.
		@date 09/03/2026
		@since 0.5.0
		@version 0.12.19
		@author Matthew Moore
	*/
	template <typename Metadata, typename StableID, us Capacity, StableID Metadata::*IDMember,
			  std::string_view Metadata::*NameMember = &Metadata::mName>
	class FixedMetadataRegistry
	{
		public:
			/*! @class Checkpoint Registry/fixedMetadataRegistry.h
				@brief Stores an opaque registry state that can be restored by @ref restoreCheckpoint.
				@details Callers can preserve a valid state for atomic rollback without directly reading or mutating the stable-ID counter.
				@date 09/03/2026
				@since 0.12.18
				@version 0.12.18
				@author Matthew Moore
			*/
			class Checkpoint
			{
				private:
					/*! @brief Captures registry state for append-only rollback.
						@param[in] owner The non-owning registry instance that creates and may restore this checkpoint. Must not be nullptr.
						@param[in] amountRegistered The number of registered entries at capture time.
						@param[in] nextID The next stable ID available at capture time.
						@param[in] mutationVersion The mutation version used to reject checkpoints invalidated by non-append mutations.
						@since 0.12.18
						@version 0.12.18
					*/
					constexpr Checkpoint(const FixedMetadataRegistry *const owner, const us amountRegistered, const us nextID,
										 const std::size_t mutationVersion) noexcept
						: mOwner{owner}, mAmountRegistered{amountRegistered}, mNextID{nextID}, mMutationVersion{mutationVersion}
					{}

					/*! @brief Non-owning pointer to the registry instance that created this checkpoint. */
					const FixedMetadataRegistry *mOwner;

					/*! @brief Number of registered entries captured by this checkpoint. */
					us mAmountRegistered;

					/*! @brief Next numeric stable ID captured by this checkpoint. */
					us mNextID;

					/*! @brief Registry mutation version captured for checkpoint validity checks. */
					std::size_t mMutationVersion;

					friend class FixedMetadataRegistry;
			};

			/*! @brief Returns metadata at an internal array index.
				@pre @p index < Capacity.
				@param[in] index The internal array index.
				@return A const reference that remains valid until replacement or registry destruction.
				@since 0.5.0
				@version 0.12.19
			*/
			ATTR_NODISCARD constexpr const Metadata *getEntry(const us index) const
			{
				if (index >= mEntries.size())
				{
					return nullptr;
				}

				return &mEntries.at(index);
			}

			/*! @brief Looks up metadata by stable ID.
				@param[in] stableID The stable identifier to find.
				@return A non-owning pointer to metadata if registered, or nullptr otherwise. The pointer remains valid until the entry is
			   replaced or the registry is destroyed.
				@since 0.5.0
				@version 0.10.7
			*/
			ATTR_NOINLINE ATTR_NODISCARD constexpr const Metadata *getMetadata(const StableID stableID) const
			{
				const us index{findEntryIndexByID(stableID)};

				if (index == mAmountRegistered)
				{
					return nullptr;
				}

				return &mEntries.at(index);
			}

			/*! @brief Looks up a stable ID by display name.
				@param[in] name The case-sensitive display name.
				@return The stable ID if registered, or std::nullopt otherwise.
				@since 0.5.0
				@version 0.6.0
			*/
			ATTR_NODISCARD constexpr const std::optional<StableID> getID(const std::string_view &name) const
			{
				const us index{findEntryIndexByName(name)};

				if (index == mAmountRegistered)
				{
					return std::nullopt;
				}

				return mEntries.at(index).*IDMember; // LCOV_EXCL_BR - Cannot fail when findEntryIndexByName is guaranteed to either be in
													 // range or mAmountRegistered
			}

			/*! @brief Looks up a display name by stable ID.
				@param[in] stableID The stable identifier to find.
				@return The display name if registered, or std::nullopt otherwise.
				@since 0.5.0
				@version 0.9.0
			*/
			ATTR_NODISCARD constexpr const std::optional<std::string_view> getName(const StableID stableID) const
			{
				const Metadata *metadata{getMetadata(stableID)};

				if (metadata == nullptr)
				{
					return std::nullopt;
				}

				return metadata->*NameMember;
			}

			/*! @brief Returns all currently registered metadata records.
				@return A read-only span that remains valid until mutation or destruction.
				@since 0.5.0
				@version 0.5.1
			*/
			ATTR_NODISCARD constexpr const std::span<const Metadata> getRegisteredEntries() const noexcept
			{
				return {mEntries.data(), mAmountRegistered};
			}

			/*! @brief Returns the number of registered records.
				@return The number of valid entries in storage.
				@since 0.5.0
				@version 0.5.0
			*/
			ATTR_NODISCARD constexpr us getAmountRegistered() const noexcept
			{
				return mAmountRegistered;
			}

			/*! @brief Returns the next numeric stable ID assigned to a custom record.
				@return The next underlying ID value.
				@since 0.5.0
				@version 0.5.0
			*/
			ATTR_NODISCARD constexpr us getNextID() const noexcept
			{
				return mNextID;
			}

			/*! @brief Finds an internal array index by stable ID.
				@param[in] stableID The stable identifier to find.
				@return The internal index if registered, or std::nullopt otherwise.
				@since 0.5.0
				@version 0.9.0
			*/
			ATTR_NODISCARD ATTR_PURE constexpr const std::optional<us> findIndexByID(const StableID stableID) const
			{
				const us index{findEntryIndexByID(stableID)}; // LCOV_EXCL_BR (from possible .at() throw which can't happen by invariants)

				if (index == mAmountRegistered)
				{
					return std::nullopt;
				}

				return index;
			}

			/*! @brief Checks whether a display name is registered.
				@param[in] name The case-sensitive display name.
				@return True if the name is registered, otherwise false.
				@since 0.5.0
				@version 0.5.1
			*/
			ATTR_NODISCARD constexpr bool hasEntry(const std::string_view &name) const
			{
				return findEntryIndexByName(name) != mAmountRegistered;
			}

			/*! @brief Checks whether a stable ID is registered.
				@param[in] stableID The stable identifier to find.
				@return True if the ID is registered, otherwise false.
				@since 0.5.0
				@version 0.5.0
			*/
			ATTR_NODISCARD constexpr bool hasEntry(const StableID stableID) const
			{
				return findEntryIndexByID(stableID) != mAmountRegistered;
			}

		protected:
			/*! @brief Replaces metadata at an internal array index.
				@pre @p index < Capacity.
				@param[in] index The internal array index.
				@param[in] metadata The complete metadata record to store.
				@since 0.5.0
				@version 0.12.18
			*/
			ATTR_NOINLINE constexpr void setEntry(const us index, const Metadata &metadata)
			{
				assert(index < mEntries.size());
				mEntries.at(index) = metadata;
				++mMutationVersion;

				if (index < mAmountRegistered)
				{
					rebuildIDIndex();
				}
			}

			/*! @brief Removes metadata at an internal index and compacts later entries with move assignment.
				@pre @p index < getAmountRegistered().
				@param[in] index The registered entry to remove.
				@since 0.8.7
				@version 0.12.18
			*/
			ATTR_NOINLINE constexpr void eraseEntry(const us index)
			{
				assert(index < mAmountRegistered);

				for (us current{index}; current + 1U < mAmountRegistered; ++current)
				{
					mEntries.at(current) = std::move(mEntries.at(static_cast<us>(current + 1U)));
				}

				mEntries.at(static_cast<us>(mAmountRegistered - 1U)) = Metadata{};
				--mAmountRegistered;
				++mMutationVersion;
				rebuildIDIndex();
			}

			/*! @brief Sets the number of valid entries.
				@param[in] amount The new registered count.
				@since 0.5.0
				@version 0.12.18
			*/
			constexpr void setAmountRegistered(const us amount) noexcept
			{
				assert(amount <= mEntries.size());
				mAmountRegistered = amount;
				++mMutationVersion;
				rebuildIDIndex();
			}

			/*! @brief Captures the current registry state for a later atomic rollback.
				@return An opaque checkpoint bound to the current registered count and next stable ID.
				@since 0.12.18
				@version 0.12.18
			*/
			ATTR_NODISCARD constexpr Checkpoint createCheckpoint() const noexcept
			{
				return {this, mAmountRegistered, mNextID, mMutationVersion};
			}

			/*! @brief Restores a previously captured registry state.
				@details Erases entries appended after @p checkpoint and restores the corresponding next stable ID. A checkpoint cannot
			   increase the registered count or supply an arbitrary ID.
				@pre Since creating @p checkpoint, the registry has only been mutated by @ref addEntry.
				@param[in] checkpoint The opaque state returned by @ref createCheckpoint.
				@since 0.12.18
				@version 0.12.18
			*/
			constexpr void restoreCheckpoint(const Checkpoint checkpoint)
			{
				assert(checkpoint.mOwner == this);
				assert(checkpoint.mAmountRegistered <= mAmountRegistered);
				assert(checkpoint.mMutationVersion == mMutationVersion);

				if (checkpoint.mOwner != this || checkpoint.mAmountRegistered > mAmountRegistered
					|| checkpoint.mMutationVersion != mMutationVersion)
				{
					return;
				}

				for (us index{checkpoint.mAmountRegistered}; index < mAmountRegistered; ++index)
				{
					mEntries.at(index) = Metadata{};
				}

				mAmountRegistered = checkpoint.mAmountRegistered;
				mNextID = checkpoint.mNextID;
				++mMutationVersion;
				rebuildIDIndex();
			}

			/*! @brief Increments the registered count.
				@since 0.5.0
				@version 0.12.18
			 */
			constexpr void incrementAmountRegistered() noexcept
			{
				assert(mAmountRegistered < mEntries.size());
				insertIDIndex(mEntries.at(mAmountRegistered).*IDMember, mAmountRegistered);
				++mAmountRegistered;
				++mMutationVersion;
			}

			/*! @brief Decrements the registered count.
				@since 0.5.0
				@version 0.12.18
			 */
			constexpr void decrementAmountRegistered() noexcept
			{
				assert(mAmountRegistered > 0U);
				removeIDIndex(mEntries.at(static_cast<us>(mAmountRegistered - 1U)).*IDMember, static_cast<us>(mAmountRegistered - 1U));
				--mAmountRegistered;
				++mMutationVersion;
			}

			/*! @brief Appends one metadata record, assigning it the next stable ID.
				@details Stamps @p IDMember with the assigned ID, stores the record after the last registered entry, and advances both the
			   registered count and the stable-ID counter together so the two can never drift apart.
				@pre @ref getAmountRegistered() < Capacity.
				@param[in] metadata The metadata record with name and domain-specific data populated.
				@return The stable ID assigned to the appended record.
				@since 0.12.18
				@version 0.12.18
			*/
			ATTR_NOINLINE constexpr StableID addEntry(Metadata metadata)
			{
				assert(mAmountRegistered < mEntries.size());

				const StableID assignedID{mNextID};
				metadata.*IDMember = assignedID;

				mEntries.at(mAmountRegistered) = std::move(metadata);
				insertIDIndex(assignedID, mAmountRegistered);
				++mAmountRegistered;
				++mNextID;

				return assignedID;
			}

			/*! @brief Returns mutable metadata at an internal array index without rebuilding lookup indexes.
				@details Derived registries may use this only to update metadata fields that are not referenced by @p IDMember or @p
			   NameMember.
				@pre @p index < Capacity.
				@param[in] index The internal array index.
				@return A mutable reference that remains valid until replacement or registry destruction.
				@since 0.12.5
				@version 0.12.5
			*/
			ATTR_NODISCARD constexpr Metadata &getMutableEntry(const us index)
			{
				assert(index < mEntries.size());
				return mEntries.at(index);
			}

			/*! @brief Constructs empty storage with a specified next-ID counter.
				@param[in] nextID The first numeric ID available after built-in registration.
				@since 0.5.0
				@version 0.5.0
			*/
			explicit constexpr FixedMetadataRegistry(const us nextID) noexcept : mNextID{nextID} {}

			/*! @brief Appends one built-in metadata record.
				@pre @ref getAmountRegistered() < Capacity.
				@param[in] metadata The complete built-in metadata record to append.
				@since 0.5.0
				@version 0.9.11
			*/
			ATTR_NOINLINE constexpr void addBuiltin(Metadata &&metadata)
			{
				assert(mAmountRegistered < mEntries.size());
				mEntries.at(mAmountRegistered) = std::move(metadata);
				incrementAmountRegistered();
			}

		private:
			/*! @struct IDIndexEntry Registry/fixedMetadataRegistry.h
				@brief Stores one searchable stable-ID to entry-index mapping.
				@details `mIDIndex` stores these entries sorted by stable ID, enabling binary-search lookup from stable ID to
				internal storage index without scanning all registered metadata.
				@date 08/03/2026
				@since 0.8.7
				@version 0.9.1
				@author Matthew Moore
			*/
			struct IDIndexEntry
			{
				public:
					/*! @brief Stable identifier used as the binary-search key. */
					StableID stableID{};

					/*! @brief Internal index into @ref mEntries for the keyed metadata record. */
					us entryIndex{};
			};

			/*! @brief Performs linear name lookup over registered entries.
				@param[in] name The case-sensitive metadata display name.
				@return The matching internal index, or @ref mAmountRegistered when not found.
				@note Time complexity is O(n), where n is @ref mAmountRegistered.
				@since 0.5.0
				@version 0.9.1
			*/
			ATTR_NOINLINE ATTR_NODISCARD constexpr us findEntryIndexByName(const std::string_view &name) const
			{
				for (us index{0}; index < mAmountRegistered; ++index)
				{
					if ((mEntries.at(index).*NameMember) == name)
					{
						return index;
					}
				}

				return mAmountRegistered;
			}

			/*! @brief Performs binary-search lookup over the sorted stable-ID index.
				@param[in] stableID The stable identifier to locate.
				@return The matching internal index, or @ref mAmountRegistered when not found.
				@note Time complexity is O(log n), where n is @ref mIndexedAmount.
				@since 0.5.0
				@version 0.9.1
			*/
			ATTR_NODISCARD ATTR_PURE constexpr us findEntryIndexByID(const StableID stableID) const
			{
				const auto indexedEntries{std::span<const IDIndexEntry>{mIDIndex}.first(mIndexedAmount)};
				const auto found{
					std::lower_bound(indexedEntries.begin(), indexedEntries.end(), stableID,
									 [](const IDIndexEntry &entry, const StableID value) { return entry.stableID < value; }),
				};

				if (found != indexedEntries.end() && found->stableID == stableID)
				{
					return found->entryIndex;
				}

				return mAmountRegistered;
			}

			/*! @brief Inserts one ID-index mapping while preserving sorted order.
				@pre @ref mIndexedAmount < Capacity.
				@param[in] stableID The stable identifier key.
				@param[in] entryIndex The corresponding internal entry index.
				@since 0.8.7
				@version 0.9.1
			*/
			constexpr void insertIDIndex(const StableID stableID, const us entryIndex)
			{
				assert(mIndexedAmount < Capacity);
				const IDIndexEntry newEntry{.stableID = stableID, .entryIndex = entryIndex};
				const auto indexedEntries{std::span<const IDIndexEntry>{mIDIndex}.first(mIndexedAmount)};
				const auto position{std::lower_bound(indexedEntries.begin(), indexedEntries.end(), newEntry, idIndexEntryLess)};
				const us insertIndex{static_cast<us>(std::distance(indexedEntries.begin(), position))};

				for (us index{mIndexedAmount}; index > insertIndex; --index)
				{
					mIDIndex.at(index) = std::move(mIDIndex.at(index - 1));
				}

				mIDIndex.at(insertIndex) = newEntry;
				++mIndexedAmount;
			}

			/*! @brief Removes one ID-index mapping and compacts remaining mappings.
				@param[in] stableID The stable identifier key.
				@param[in] entryIndex The corresponding internal entry index.
				@pre The mapping identified by @p stableID and @p entryIndex exists in @ref mIDIndex.
				@since 0.8.7
				@version 0.9.1
			*/
			constexpr void removeIDIndex(const StableID stableID, const us entryIndex)
			{
				const auto indexedEntries{std::span<const IDIndexEntry>{mIDIndex}.first(mIndexedAmount)};
				const IDIndexEntry indexedEntry{.stableID = stableID, .entryIndex = entryIndex};
				const auto position{std::lower_bound(indexedEntries.begin(), indexedEntries.end(), indexedEntry, idIndexEntryLess)};
				const us removeIndex{static_cast<us>(std::distance(indexedEntries.begin(), position))};
				assert(removeIndex < mIndexedAmount && mIDIndex.at(removeIndex).stableID == stableID
					   && mIDIndex.at(removeIndex).entryIndex == entryIndex);

				for (us index{removeIndex}; (index + 1) < mIndexedAmount; ++index)
				{
					mIDIndex.at(index) = std::move(mIDIndex.at(index + 1));
				}

				--mIndexedAmount;
			}

			/*! @brief Orders two ID-index entries by stable ID then by entry index.
				@param[in] lhs Left-hand index entry.
				@param[in] rhs Right-hand index entry.
				@return True when @p lhs is ordered before @p rhs.
				@since 0.8.7
				@version 0.9.1
			*/
			ATTR_NODISCARD static constexpr bool idIndexEntryLess(const IDIndexEntry &lhs, const IDIndexEntry &rhs) noexcept
			{
				return lhs.stableID < rhs.stableID || (lhs.stableID == rhs.stableID && lhs.entryIndex < rhs.entryIndex);
			}

			/*! @brief Rebuilds and resorts the stable-ID index from registered entries.
				@details Regenerates @ref mIDIndex from @ref mEntries for all indices in [0, @ref mAmountRegistered),
				then sorts the generated entries with @ref idIndexEntryLess.
				@since 0.8.7
				@version 0.12.18
			*/
			ATTR_NOINLINE constexpr void rebuildIDIndex() noexcept
			{
				for (us index{0}; index < mAmountRegistered; ++index)
				{
					mIDIndex.at(index) = IDIndexEntry{.stableID = mEntries.at(index).*IDMember, .entryIndex = index};
				}

				mIndexedAmount = mAmountRegistered;
				auto indexedEntries{std::span<IDIndexEntry>{mIDIndex}.first(mIndexedAmount)};
				std::sort(indexedEntries.begin(), indexedEntries.end(), idIndexEntryLess);
			}

		private:
			/*! @brief Fixed-capacity contiguous storage for metadata records.
				@details Entries in [0, @ref mAmountRegistered) are valid registered metadata.
			*/
			std::array<Metadata, Capacity> mEntries{};

			/*! @brief Sorted stable-ID index used for O(log n) stable-ID lookups.
				@details Entries in [0, @ref mIndexedAmount) are valid index mappings.
			*/
			std::array<IDIndexEntry, Capacity> mIDIndex{};

			/*! @brief Number of registered metadata entries in @ref mEntries. */
			us mAmountRegistered{0};

			/*! @brief Number of valid mappings currently populated in @ref mIDIndex. */
			us mIndexedAmount{0};

			/*! @brief Invalidates checkpoints after mutations other than append-only registration. */
			std::size_t mMutationVersion{0};

			/*! @brief Next numeric stable ID for caller-managed custom entry registration. */
			us mNextID{0};
	};
} // namespace PocketCore::Registry

#endif
