#include "ab/process.hpp"
#include "Jit.hpp"
#include <cassert>

namespace ab {

bool Process::initialized = false;

Condition Process::init() {
  assert(initialized == false);
  bool jitOk = initializeJit();
  if (!jitOk) {
    return Condition::failure;
  }
  initialized = true;
  return Condition::success;
}

Condition Process::kill() {
  assert(initialized == true);
  shutdownJit();
  initialized = false;
}
}
