#include <Om/Config.hpp>
#include <Om/Process.hpp>

namespace Om {

auto Process::init() -> ProcessError {
	return ProcessError::SUCCESS;
}

auto Process::kill() -> ProcessError {
	return ProcessError::SUCCESS;
}

} // namespace Om
