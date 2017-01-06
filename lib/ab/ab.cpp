#include <ab.hpp>
#include <Jit.hpp>
#include <ilgen/MethodBuilder.hpp>
#include <ilgen/TypeDictionary.hpp>
#include <cstdint>
#include <iostream>

namespace ab {

int toExitCode(const Condition &c) { return static_cast<int>(c); }

} // namespace ab
