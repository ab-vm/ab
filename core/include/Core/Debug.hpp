#ifndef OM_DEBUG_HPP_
#define OM_DEBUG_HPP_

/// Defines a set of debug-only macros and APIs.
/// The macros in this header are conditionally enabled by the OM_DEBUG define. To enable debug code with cmake, use:
/// `cmake -DOM_DEBUG=On`

#include <Om/Config.hpp>

#ifdef OM_DEBUG
#include <cassert>
#endif

/// @macro OM_DBG_ASSERT(assertion)
/// Assert that the assertion is true. Be careful, when OM_DEBUG is disabled, the assertion won't be compiled.)
#ifdef OM_DEBUG
#define OM_DBG_ASSERT(x) assert(x)
#else
#define OM_DBG_ASSERT(x)  // nothing
#endif

#endif  // OM_DEBUG_HPP_