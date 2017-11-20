#ifndef AB_PROCESS_HPP_
#define AB_PROCESS_HPP_

#include <Ab/Config.hpp>
#include <cstdlib>
#include <unistd.h>

namespace Ab {

struct SystemProperties {
public:
	SystemProperties() : page_size_(sysconf(_SC_PAGESIZE)) {}

	/// The memory page size.
	///
	std::size_t page_size() const noexcept { return page_size_; }

private:
	std::size_t page_size_;
};

/// Global init and shutdown. Process-wide! Thread unsafe!
class Process {
public:
	/// Obtain the invariant properties of the process.
	static const SystemProperties& properties() noexcept { return properties_; }

private:
	static SystemProperties properties_;
};

}  // namespace Ab

#endif  // AB_PROCESS_HPP_
