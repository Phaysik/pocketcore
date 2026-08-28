/*! @file random.h
	@brief Contains the function declarations for creating a random number generator
	@date 08/03/2026
	@since 0.1.0
	@version 0.9.7
	@author Matthew Moore
*/

#ifndef INCLUDE_RANDOM_H
#define INCLUDE_RANDOM_H

#include <chrono>
#include <random>

#include "Core/attributeMacros.h"
#include "Core/cconcepts.h" // for Integral

/*! @namespace PocketCore::Utility Holds any useful functionality that doesn't fit anywhere else
	@since 0.1.0
	@version 0.1.0
	@author Matthew Moore
*/
namespace PocketCore::Utility
{
	/*! @class Random random.h "include/random.h"
		@brief Class for creating a random number generator
		@since 0.1.0
		@version 0.9.7
		@author Matthew Moore
	*/
	class Random
	{
		public:
			/*! @brief Gets a random number in the range [min, max] wtih a templated return type in case you need to cast the uniform
			   distribution result to a different type
				@tparam T The type to cast the uniform distribution result to
				@param[in] min The minimum value (inclusive)
				@param[in] max The maximum value (inclusive)
				@retval T The typecasted random number
				@since 0.1.0
				@version 0.3.0
				@author Matthew Moore
			*/
			template <PocketCore::Core::Integral T>
			ATTR_NODISCARD static T get(const T min, const T max) noexcept
			{
				return std::uniform_int_distribution<T>{min, max}(mTwister);
			}

			/*! @brief Gets a random number in the range [min, max) wtih a templated return type in case you need to cast the real
			   distribution result to a different type
				@tparam T The type to cast the real distribution result to
				@param[in] min The minimum value (inclusive)
				@param[in] max The maximum value (exclusive)
				@retval T The typecasted random number
				@since 0.9.7
				@version 0.9.7
				@author Matthew Moore
			*/
			template <PocketCore::Core::FloatingPoint T>
			ATTR_NODISCARD static T get(const T min, const T max) noexcept
			{
				return std::uniform_real_distribution<T>{min, max}(mTwister);
			}

			/*! @brief Gets #mTwister
				@retval std::mt19937 The global random number generator
				@since 0.1.0
				@version 0.1.0
				@author Matthew Moore
			*/
			ATTR_NODISCARD static std::mt19937 &getTwister() noexcept
			{
				return mTwister;
			}

		private:
			/*! @brief Creates the global random number generator
				@retval std::mt19937 The global random number generator
				@since 0.1.0
				@version 0.4.0
				@author Matthew Moore
			*/
			ATTR_NODISCARD static std::mt19937 generate() noexcept
			{
				std::random_device randomDevice{};

				// Create seed_seq with high-res clock and 7 random numbers from std::random_device
				std::seed_seq seedSequence{
					static_cast<std::seed_seq::result_type>(std::chrono::steady_clock::now().time_since_epoch().count()),
					randomDevice(),
					randomDevice(),
					randomDevice(),
					randomDevice(),
					randomDevice(),
					randomDevice(),
					randomDevice(),
				};

				return std::mt19937{seedSequence};
			}

			static inline std::mt19937 mTwister{generate()}; /*!< The global random number generator */
	};
} // namespace PocketCore::Utility

#endif
