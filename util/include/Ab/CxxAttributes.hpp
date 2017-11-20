#ifndef AB_CXXATTR_HPP_
#define AB_CXXATTR_HPP_

#include <Ab/Config.hpp>

#if 1
#define AB_UNUSED gnu::unused
#else
#define AB_UNUSED maybe_unused
#endif

#endif  // AB_ATTR_HPP_
