#ifndef AB_XFUNCTION_HPP_
#define AB_XFUNCTION_HPP_

#include <Ab/Address.hpp>

#include <cstdint>
#include <cstddef>

namespace Ab {

class XModule;

struct XFunctionId {
	std::int32_t value;
};

/// Handle to a 
struct XFunctionHandle {
	XModule* module;
	XFunctionId id;
};

} // namespace Ab

#endif // AB_XFUNCTION_HPP_
