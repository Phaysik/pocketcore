/*! @file fixedMetadataRegistry.h
	@brief Provides shared fixed-capacity storage and lookup for metadata registries.
	@date 07/27/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_REGISTRY_FIXED_METADATA_REGISTRY_H
#define INCLUDE_REGISTRY_FIXED_METADATA_REGISTRY_H

#include <array>
#include <cassert>
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
		@note Lookup operations are O(n), where n is the number of registered entries. Storage operations do not allocate.
		@date 07/27/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	template <typename Metadata, typename StableID, us Capacity, StableID Metadata::*IDMember>
	class FixedMetadataRegistry
	{
		public:
			/*! @brief Returns metadata at an internal array index.
				@pre @p index < Capacity.
				@param[in] index The internal array index.
				@return A const reference that remains valid until replacement or registry destruction.
			*/
			ATTR_NODISCARD constexpr const Metadata &getEntry(const us index) const
			{
				assert(index < mEntries.size());
				return mEntries.at(index);
			}

			/*! @brief Looks up metadata by stable ID.
				@param[in] stableID The stable identifier to find.
				@return A non-owning pointer to metadata if registered, or nullptr otherwise. The pointer remains valid until the entry is
			   replaced or the registry is destroyed.
			*/
			ATTR_NODISCARD constexpr const Metadata *getMetadata(const StableID stableID) const
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
			*/
			ATTR_NODISCARD constexpr const std::optional<StableID> getID(const std::string_view &name) const
			{
				const us index{findEntryIndexByName(name)};

				if (index == mAmountRegistered)
				{
					return std::nullopt;
				}

				return mEntries.at(index).*IDMember;
			}

			/*! @brief Looks up a display name by stable ID.
				@param[in] stableID The stable identifier to find.
				@return The display name if registered, or std::nullopt otherwise.
			*/
			ATTR_NODISCARD constexpr const std::optional<std::string_view> getName(const StableID stableID) const
			{
				const Metadata *metadata{getMetadata(stableID)};

				if (metadata == nullptr)
				{
					return std::nullopt;
				}

				return metadata->mName;
			}

			/*! @brief Returns all currently registered metadata records.
				@return A read-only span that remains valid until mutation or destruction.
			*/
			ATTR_NODISCARD constexpr const std::span<const Metadata> getRegisteredEntries() const noexcept
			{
				return {mEntries.data(), mAmountRegistered};
			}

			/*! @brief Returns the number of registered records.
				@return The number of valid entries in storage.
			*/
			ATTR_NODISCARD constexpr us getAmountRegistered() const noexcept
			{
				return mAmountRegistered;
			}

			/*! @brief Returns the next numeric stable ID assigned to a custom record.
				@return The next underlying ID value.
			*/
			ATTR_NODISCARD constexpr us getNextID() const noexcept
			{
				return mNextID;
			}

			/*! @brief Finds an internal array index by stable ID.
				@param[in] stableID The stable identifier to find.
				@return The internal index if registered, or std::nullopt otherwise.
			*/
			ATTR_NODISCARD constexpr const std::optional<us> findIndexByID(const StableID stableID) const
			{
				const us index{findEntryIndexByID(stableID)};

				if (index == mAmountRegistered)
				{
					return std::nullopt;
				}

				return index;
			}

			/*! @brief Checks whether a display name is registered.
				@param[in] name The case-sensitive display name.
				@return True if the name is registered, otherwise false.
			*/
			ATTR_NODISCARD constexpr bool hasEntry(const std::string_view &name) const
			{
				return findEntryIndexByName(name) != mAmountRegistered;
			}

			/*! @brief Checks whether a stable ID is registered.
				@param[in] stableID The stable identifier to find.
				@return True if the ID is registered, otherwise false.
			*/
			ATTR_NODISCARD constexpr bool hasEntry(const StableID stableID) const
			{
				return findEntryIndexByID(stableID) != mAmountRegistered;
			}

			/*! @brief Replaces metadata at an internal array index.
				@pre @p index < Capacity.
				@param[in] index The internal array index.
				@param[in] metadata The complete metadata record to store.
			*/
			constexpr void setEntry(const us index, const Metadata &metadata)
			{
				assert(index < mEntries.size());
				mEntries.at(index) = metadata;
			}

			/*! @brief Sets the number of valid entries.
				@param[in] amount The new registered count.
			*/
			constexpr void setAmountRegistered(const us amount) noexcept
			{
				mAmountRegistered = amount;
			}

			/*! @brief Sets the next numeric stable ID counter.
				@param[in] nextID The next underlying ID value.
			*/
			constexpr void setNextID(const us nextID) noexcept
			{
				mNextID = nextID;
			}

			/*! @brief Increments the registered count. */
			constexpr void incrementAmountRegistered() noexcept
			{
				++mAmountRegistered;
			}

			/*! @brief Decrements the registered count. */
			constexpr void decrementAmountRegistered() noexcept
			{
				--mAmountRegistered;
			}

			/*! @brief Increments the next stable ID counter. */
			constexpr void incrementNextID() noexcept
			{
				++mNextID;
			}

		protected:
			/*! @brief Constructs empty storage with a specified next-ID counter.
				@param[in] nextID The first numeric ID available after built-in registration.
			*/
			explicit constexpr FixedMetadataRegistry(const us nextID) noexcept : mNextID{nextID} {}

			/*! @brief Appends one built-in metadata record.
				@pre @ref getAmountRegistered() < Capacity.
				@param[in] metadata The complete built-in metadata record to append.
			*/
			constexpr void addBuiltin(Metadata &&metadata)
			{
				assert(mAmountRegistered < mEntries.size());
				mEntries.at(mAmountRegistered) = std::move(metadata);
				++mAmountRegistered;
			}

		private:
			ATTR_NODISCARD constexpr us findEntryIndexByName(const std::string_view &name) const
			{
				for (us index{0}; index < mAmountRegistered; ++index)
				{
					if (mEntries.at(index).mName == name)
					{
						return index;
					}
				}

				return mAmountRegistered;
			}

			ATTR_NODISCARD constexpr us findEntryIndexByID(const StableID stableID) const
			{
				for (us index{0}; index < mAmountRegistered; ++index)
				{
					if (mEntries.at(index).*IDMember == stableID)
					{
						return index;
					}
				}

				return mAmountRegistered;
			}

		private:
			std::array<Metadata, Capacity> mEntries{};
			us mAmountRegistered{0};
			us mNextID{0};
	};
} // namespace PocketCore::Registry

#endif
