#include <Om/Config.hpp>
#include <Om/Process.hpp>
#include <Pith/Assert.hpp>
#include <Pith/Debug.hpp>
#include <Pith/Process.hpp>
#include <cstring>
#include <omr.h>
#include <omrport.h>
#include <thread_api.h>

namespace Om {

OMR_Runtime OmrProcess::runtime_;
OMRPortLibrary OmrProcess::port_;

// static
auto OmrProcess::init() -> void {
	PITH_TRACE();
	initThreading();

	omrthread_t j9self = NULL;

	if (0 != omrthread_attach_ex(&j9self, J9THREAD_ATTR_DEFAULT)) {
		throw OmrProcessError("Failed to attach thread during OMR startup");
	}

	initPort();
	initRuntime();

	omrthread_detach(j9self);
}

// static
auto OmrProcess::initThreading() -> void {
	auto e = omrthread_init_library();
	if (e != 0) {
		throw OmrProcessError("OMR thread library failed to initialize");
	}
}

// static
auto OmrProcess::initPort() -> void {
	auto e = omrport_init_library(&port_, sizeof(OMRPortLibrary));
	if (e != 0) {
		throw OmrProcessError("OMR port library failed to initialize");
	}
}

// static
auto OmrProcess::initRuntime() -> void {
	memset(&runtime_, 0, sizeof(OMR_Runtime));
	runtime_._configuration._maximum_vm_count = 0;
	runtime_._vmCount                         = 0;
	runtime_._portLibrary                     = &port_;

	auto e = omr_initialize_runtime(&runtime_);
	if (e != 0) {
		throw OmrProcessError("OMR Runtime failed to initialize");
	}
}

// static
auto OmrProcess::kill() -> void {
	PITH_TRACE();
	omr_destroy_runtime(&runtime_);
	port_.port_shutdown_library(&port_);
	omrthread_shutdown_library();
}

// static
auto Process::init() -> void {
	PITH_TRACE();
	Pith::Process::init();
	OmrProcess::init();
}

/// static
auto Process::kill() -> void {
	PITH_TRACE();
	OmrProcess::kill();
	Pith::Process::kill();
}

// static
auto Process::attach([[gnu::unused]] System& system) -> void {
	PITH_TRACE();
}

// static
auto Process::detach([[gnu::unused]] System& system) -> void {
	PITH_TRACE();
}

}  // namespace Om
