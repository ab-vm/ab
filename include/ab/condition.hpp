#ifndef AB_CONDITION_HPP_
#define AB_CONDITION_HPP_

namespace ab {

enum class Condition : int { success = 0, failure = 1 };

int toExitCode(const Condition &c);

} // namespace ab

#endif // AB_CONDITION_HPP_
