#ifndef AB_INSTRUCTION_HPP_
#define AB_INSTRUCTION_HPP_

#include <cstdint>

namespace ab {

enum class Instruction : uint8_t {
  noOperation = 0,
  halt = 1,
  push,
  pop,
  drop,
  duplicate,
  rotate,
  add,
  define,
  call,
  yield,
};

} // namespace ab

#endif // AB_INSTRUCTION_HPP_
