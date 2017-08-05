#include <Ab/Process.hpp>

#include <cassert>

namespace Ab {

// static
bool Process::initialized_ = false;

// static
auto Process::init() -> Error {
	assert(initialized_ == false);
	return Error::SUCCESS;
}

// static
auto Process::kill() -> Error {
	assert(initialized_ == true);
	return Error::SUCCESS;
}

} // namespace ab
