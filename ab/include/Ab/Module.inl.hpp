#ifndef AB_MODULE_INL_HPP_
#define AB_MODULE_INL_HPP_

#include <Ab/Module.hpp>

namespace Ab {

Module::Module(Context& cx) : Om::NativeCell(cx, sizeof(Module)){};

}  // namespace Ab

#endif  // AB_MODULE_INL_HPP_
