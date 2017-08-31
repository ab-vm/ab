#ifndef OM_DEBUG_HPP_
#define OM_DEBUG_HPP_

#include <Om/Config.hpp>

#include <Pith/Assert.hpp>
#include <Pith/CppUtilities.hpp>
#include <Pith/Debug.hpp>

#include <iostream>

namespace Om {
namespace Debug {
namespace {

inline auto printTrace(const char* location) -> void {
	std::cerr << location << std::endl;
}

}  // namespace
}  // namespace Debug
}  // namespace Om

/// OM DEBUG TRACE
#ifdef OM_DEBUG
#define OM_DEBUG_TRACE() ::Om::Debug::printTrace(PITH_LOCATION_STRING());
#else
#define OM_DEBUG_TRACE()  // nothing
#endif

/// OM_DEBUG_ASSERT
#ifdef OM_DEBUG
#define OM_DEBUG_ASSERT(x) PITH_ASSERT(x)
#else
#define OM_DEBUG_ASSERT(x)  // nothing
#endif

#endif  // OM_DEBUG_HPP_
