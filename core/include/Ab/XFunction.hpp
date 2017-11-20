#ifndef AB_XFUNCTION_HPP_
#define AB_XFUNCTION_HPP_

#include <Ab/Address.hpp>

#include <cstddef>

namespace Ab {

struct XFunctionArgs {
	std::size_t nbytes;
};

// metadata
struct XFunctionHeader {
	std::size_t paramBytes;
	std::size_t localBytes;
};

struct XFunction {
	XFunctionHeader header;
	Byte body[];
};

static_assert(offsetof(XFunction, body) == 16);
static_assert(sizeof(XFunction) == sizeof(XFunctionHeader));

}  // namespace Ab

#endif // AB_XFUNCTION_HPP_
