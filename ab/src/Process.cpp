#include <Ab/Config.hpp>
#include <Ab/Process.hpp>
#include <Om/Process.hpp>
#include <Pith/Assert.hpp>

namespace Ab {

// static
bool Process::initialized_ = false;

// static
auto Process::init() -> ProcessError {
	PITH_ASSERT(initialized_ == false);

	auto e = Om::Process::init();
	if (e != Om::ProcessError::SUCCESS) {
		return ProcessError::OM_ERROR;
	}

	initialized_ = true;

	return ProcessError::SUCCESS;
}

// static
auto Process::kill() -> ProcessError {
	PITH_ASSERT(initialized_ == true);

	initialized_ = false;

	auto e = Om::Process::kill();
	if (e != Om::ProcessError::SUCCESS) {
		return ProcessError::OM_ERROR;
	}

	return ProcessError::SUCCESS;
}

}  // namespace Ab
