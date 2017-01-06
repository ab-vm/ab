#ifndef AB_PROCESS_HPP_
#define AB_PROCESS_HPP_

#include "ab/condition.hpp"

namespace ab {

class Process {
public:
  static Condition init();
  static Condition kill();

private:
  static bool initialized;
};

} // namespace ab

#endif // AB_PROCESS_HPP_
