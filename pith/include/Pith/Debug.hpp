#ifndef PITH_DEBUG_HPP_
#define PITH_DEBUG_HPP_

/// Defines a set of debug-only macros and APIs.
/// The macros in this header are conditionally enabled by the OM_DEBUG define.
/// To enable debug code with cmake, use:
/// `cmake -DOM_DEBUG=On`

#include <Pith/Config.hpp>
#include <Pith/CppUtilities.hpp>
#include <iostream>

namespace Pith {
namespace {

inline auto trace(const char* function) -> void {
	std::cerr << "TRACE!> " << function << std::endl;
}

}  // namespace
}  // namespace Pith

#define PITH_TRACE() ::Pith::trace(PITH_FUNCTION_STR())

#ifdef PITH_DEBUG
#define PITH_DEBUG_TRACE() PITH_TRACE()
#else
#define PITH_DEBUG_TRACE()  // nothing
#endif

/// @macro OM_DBG_ASSERT(assertion)
/// Assert that the assertion is true. Be careful, when OM_DEBUG is disabled,
/// the assertion won't be compiled.)
#ifdef PITH_DEBUG
#define PITH_DEBUG_ASSERT(x) PITH_ASSERT(x)
#else
#define PITH_DEBUG_ASSERT(x)  // nothing
#endif

#endif  // PITH_DEBUG_HPP_
