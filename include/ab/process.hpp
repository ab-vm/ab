#ifndef AB_PROCESS_HPP_
#define AB_PROCESS_HPP_

#include "ab/condition.hpp"

namespace ab {

/// Global init and shutdown.
class Process {
public:
  /// Process-wide initialization. Must be the ab first call.
  static Condition init();

  /// Process-wide tear down. Must be the last ab call made.
  static Condition kill();

private:
  static bool initialized;
};

} // namespace ab

#endif // AB_PROCESS_HPP_
