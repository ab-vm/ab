#ifndef PITH_PROCESS_HPP_
#define PITH_PROCESS_HPP_

#include <Pith/Config.hpp>
#include <cstdlib>
#include <unistd.h>

namespace Pith {

struct SystemProperties {
public:
	auto init() -> void;

	/// The memory page size.
	auto pageSize() const noexcept -> std::size_t;

private:
	std::size_t pageSize_;
};

/// Global init and shutdown. Process-wide! Thread unsafe!
class Process {
public:
	/// Process-wide initialization. Must be the first pith call.
	static inline auto init() -> void;

	/// Process-wide tear down. Must be the last pith call made.
	static inline auto kill() -> void;

	/// Obtain the invariant properties of the process.
	static inline auto properties() noexcept -> const SystemProperties&;

	/// True if init() has been called.
	static inline auto initialized() noexcept -> bool;

private:
	static bool initialized_;
	static SystemProperties properties_;
};

}  // namespace Pith

#include <Pith/Process.inl.hpp>

#endif  // PITH_PROCESS_HPP_
