#ifndef AB_PROCESS_HPP_
#define AB_PROCESS_HPP_

#include <Ab/Config.hpp>
#include <Om/Process.hpp>

namespace Ab {

/// Global init and shutdown.
/// Process-wide! Thread unsafe!
class Process {
public:
	static auto init() -> void;

	static auto kill() -> void;

	static auto initialized() -> bool;

private:
	static bool initialized_;
};

}  // namespace Ab

#include <Ab/Process.inl.hpp>

#endif  // AB_PROCESS_HPP_
