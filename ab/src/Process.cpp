#include <Ab/Process.hpp>

#include <cassert>

namespace Ab {

// static
bool Process::initialized_ = false;

// static
auto Process::init() -> Error {
	assert(initialized_ == false);
	initialized_ = true;
	return Error::SUCCESS;
}

// static
auto Process::kill() -> Error {
	assert(initialized_ == true);
	initialized_ = false;
	return Error::SUCCESS;
}

}  // namespace Ab
