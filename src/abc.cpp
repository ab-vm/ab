#include "ab.hpp"
#include <iostream>

namespace ab {

Condition repl(std::istream &in) {
  while (true) {
    return Condition::success;
  }
};

} // namespace ab

extern "C" int main() {
  ab::Process::init();
  std::cout << ab::programName << " " << ab::programVersion << std::endl;
  std::cout << ab::abigail << std::endl;

  auto condition = ab::repl(std::cin);
  ab::ab_test();
  condition = ab::Process::kill();
  return ab::toExitCode(condition);
}

/*

input syntax

: SYMBOL body ;

 */
