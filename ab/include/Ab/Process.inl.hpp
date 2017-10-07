#ifndef AB_PROCESS_INL_HPP_
#define AB_PROCESS_INL_HPP_

#include <Ab/Process.hpp>

namespace Ab {

inline auto Process::init() -> void {
	Om::Process::init();
}

inline auto Process::kill() -> void {
	Om::Process::kill();
}

} // namespace Ab

#endif // AB_PROCESS_INL_HPP_
