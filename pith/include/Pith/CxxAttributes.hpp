#ifndef PITH_CXXATTR_HPP_
#define PITH_CXXATTR_HPP_

#include <Pith/Config.hpp>

#if 1
#define PITH_UNUSED gnu::unused
#else
#define PITH_UNUSED maybe_unused
#endif

#endif // PITH_ATTR_HPP_
