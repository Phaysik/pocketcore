/*! @file fixedMetadataConfiguration.testHelper.h
	@brief Provides a concrete test facade for fixed metadata registry configuration.
	@date 10/09/2026
	@since 0.12.50
	@version 0.12.50
	@author Matthew Moore
*/

#ifndef TEST_INCLUDE_CONFIGURATION_FIXED_METADATA_CONFIGURATION_TEST_HELPER_H
#define TEST_INCLUDE_CONFIGURATION_FIXED_METADATA_CONFIGURATION_TEST_HELPER_H

#include <string_view>

#include "../Registry/fixedMetadataRegistry.testHelper.h"
#include "Configuration/fixedMetadataRegistryConfiguration.h"
#include "Registry/registryError.h"

namespace PocketCore::Testing
{
	using PocketCore::Configuration::FixedMetadataRegistryConfiguration;
	using PocketCore::Registry::RegistryError;

	/*! @struct FixedConfigurationPolicy Configuration/fixedMetadataConfiguration.testHelper.h
		@brief Supplies diagnostic names and error categories for the dummy configuration.
		@details Reuses type error categories as test placeholders; no production domain is registered.
		@date 10/09/2026
		@since 0.12.50
		@version 0.12.50
		@author Matthew Moore
	*/
	struct FixedConfigurationPolicy
	{
		public:
			/*! @brief Names the configuration in diagnostics. */
			static constexpr std::string_view configurationName{"FixedConfiguration"};

			/*! @brief Names one dummy metadata entry in diagnostics. */
			static constexpr std::string_view entityName{"metadata"};

			/*! @brief Reports a duplicate dummy metadata name. */
			static constexpr RegistryError duplicateError{RegistryError::DuplicateMetadata};

			/*! @brief Reports an absent dummy metadata entry. */
			static constexpr RegistryError notFoundError{RegistryError::MetadataNotFound};
	};

	/*! @class FixedConfiguration Configuration/fixedMetadataConfiguration.testHelper.h
		@brief Exposes shared configuration operations over the dummy fixed registry for tests.
		@details Reuses @ref FixedRegistry, @ref Metadata, and @ref FixedMetaDataID. Public using declarations expose the template's lookup,
	   addition, batch addition, mutation, renaming, and removal operations without duplicating their implementations.
		@date 10/09/2026
		@since 0.12.50
		@version 0.12.50
		@author Matthew Moore
	*/
	class FixedConfiguration : private FixedMetadataRegistryConfiguration<FixedRegistry, Metadata, FixedMetaDataID, CAPACITY,
																		  &Metadata::mID, FixedConfigurationPolicy>
	{
		private:
			using Base = FixedMetadataRegistryConfiguration<FixedRegistry, Metadata, FixedMetaDataID, CAPACITY, &Metadata::mID,
															FixedConfigurationPolicy>;

		public:
			/*! @brief Constructs a configuration containing the dummy registry's built-in metadata. */
			constexpr FixedConfiguration() = default;

			using Base::addMetadata;
			using Base::addMetadataBatch;
			using Base::getAmountRegistered;
			using Base::getID;
			using Base::getMetadata;
			using Base::getName;
			using Base::getRegisteredEntries;
			using Base::getRegistry;
			using Base::hasEntry;
			using Base::mutateMetadata;
			using Base::removeMetadata;
			using Base::renameMetadata;
	};
} // namespace PocketCore::Testing

#endif
