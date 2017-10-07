#ifndef OM_PROCESS_HPP_
#define OM_PROCESS_HPP_

#include <Om/Config.hpp>
#include <omr.h>
#include <omrport.h>
#include <stdexcept>
#include <thread_api.h>

namespace Om {

class System;

class ProcessError : public std::runtime_error {
	using std::runtime_error::runtime_error;
};

class OmrProcessError : public ProcessError {
	using ProcessError::ProcessError;
};

class OmrProcess {
public:
	static auto init() -> void;

	static auto kill() -> void;

	static inline auto runtime() noexcept -> OMR_Runtime&;

	static inline auto port() noexcept -> OMRPortLibrary&;

private:
	static auto initThreading() -> void;

	static auto initPort() -> void;

	static auto initRuntime() -> void;

	static OMR_Runtime runtime_;
	static OMRPortLibrary port_;
};

class Process {
public:
	static auto init() -> void;

	static auto kill() -> void;

protected:
	friend class System;

	static auto attach(System& system) -> void;

	static auto detach(System& system) -> void;

private:
};

}  // namespace Om

#include <Om/Process.inl.hpp>

#endif  // OM_PROCESS_HPP_
