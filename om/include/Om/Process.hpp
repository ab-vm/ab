#ifndef OM_PROCESS_HPP_
#define OM_PROCESS_HPP_

#include <Om/Config.hpp>

namespace Om {

enum class ProcessError {
	SUCCESS, ERROR
};

class Process {
public:
	static auto init() -> ProcessError;
	static auto kill() -> ProcessError;
};

} // namespace Om

#endif // OM_PROCESS_HPP_
