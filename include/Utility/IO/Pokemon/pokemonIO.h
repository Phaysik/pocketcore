/*! @file pokemonIO.h
	@brief Contains the function declarations for handling IO operations for Pokemon
	@date 10/14/2026
	@since 0.12.51
	@version 0.12.51
	@author Matthew Moore
*/

#ifndef INCLUDE_UTILITY_IO_INPUT_H
#define INCLUDE_UTILITY_IO_INPUT_H

#include <functional>
#include <ostream>
#include <utility>

#include "Core/concepts.h"
#include "Core/typedefs.h"

namespace PocketCore::Utility::IO::Pokemon
{
	using PocketCore::Core::InvocableWithArgs;
	using PocketCore::Core::ub;

	/*! @brief Writes the numeric value of one Pokemon data ID with an index-dependent separator.
		@details Invokes the callable once as an lvalue with the supplied index and inserts the result's getValue() into the stream.
		Writes no separator for index zero, or a comma followed by a space otherwise. Does not write brackets or a newline.
		@tparam Func A callable satisfying @ref PocketCore::Core::InvocableWithArgs for Func& and ub, whose result exposes getValue()
		and whose getValue() result supports stream insertion.
		@param[in,out] outStream The non-owning output stream reference used only during this call.
		@param[in,out] func The non-owning callable reference used only during this call; invocation may modify its state.
		@param[in] index The data slot index passed to the callable and used to select the separator.
		@pre The index satisfies the callable's indexing requirements.
		@throws std::ios_base::failure If stream insertion fails and the corresponding stream exception mask is enabled.
		@note Exceptions from the callable, getValue(), and stream insertion propagate to the caller. Output may be partially written.
		@date 10/14/2026
		@since 0.12.51
		@version 0.12.51
		@author Matthew Moore
	*/
	template <typename Func>
		requires InvocableWithArgs<Func &, ub>
	void outputPokemonDataID(std::ostream &outStream, Func &&func, const ub index)
	{
		Func &&forwardedFunction{std::forward<Func>(func)};

		outStream << (index == 0U ? "" : ", ") << std::invoke(forwardedFunction, index).getValue();
	}
} // namespace PocketCore::Utility::IO::Pokemon

#endif
