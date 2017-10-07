#include <Om/Config.hpp>
#include <Om/Heap.hpp>
#include <Om/System.hpp>
#include <Om/SystemLock.hpp>
#include <Pith/Assert.hpp>
#include <StartupManagerImpl.hpp>

namespace Om {

auto OmrSystem::init() -> OmrSystemError {
#if 0
	memset(&vm_, 0, sizeof(OMR_VM));
	vm_._runtime = &OmrProcess::runtime();
	vm_._language_vm = this;

	// todo: move this to Process::attach(*this);

	auto rc = omr_attach_vm_to_runtime(&vm_);
	PITH_ASSERT(rc == 0);

	// GC

	MM_StartupManagerImpl manager(&vm_);
	PITH_ASSERT(OMR_GC_IntializeHeapAndCollector(&vm_, &manager) == 0);
	MM_GCExtensionsBase* extensions = MM_GCExtensionsBase::getExtensions(&vm_);

	// GC Slave threads

	PITH_ASSERT(extensions->dispatcher->startUpThreads() == true);
#endif  // 0
	return OmrSystemError::SUCCESS;
}

auto OmrSystem::kill() -> OmrSystemError {
#if 0
	MM_GCExtensionsBase* extensions = MM_GCExtensionsBase::getExtensions(vm_);
	omr_error_t rc = OMR_ERROR_NONE;

	// GC Slave threads

	if (NULL != extensions->dispatcher) {
		extensions->dispatcher->shutDownThreads();
		extensions->dispatcher->kill(MM_EnvironmentBase::getEnvironment(omrVMThread));
		extensions->dispatcher = NULL;
	}

	return rc;
#endif  // 0

#if 0
	if (NULL != omrVMThread) {
		OMRPORT_ACCESS_FROM_OMRVMTHREAD(omrVMThread);
#if defined(OMR_GC)
		rc = OMR_GC_ShutdownDispatcherThreads(omrVMThread);
		if (OMR_ERROR_NONE != rc) {
			omrtty_printf("Failed to shutdown GC slave threads, rc=%d.\n", rc);
		}

		rc = OMR_GC_ShutdownCollector(omrVMThread);
		if (OMR_ERROR_NONE != rc) {
			omrtty_printf("Failed to shutdown Garbage Collector, rc=%d.\n", rc);
		}
#endif /* OMR_GC */

		omr_ras_cleanupMethodDictionary(omrVM);

		omr_ras_cleanupHealthCenter(omrVM, &(omrVM->_hcAgent));

		rc = omr_ras_cleanupTraceEngine(omrVMThread);
		if (OMR_ERROR_NONE != rc) {
			omrtty_printf("Failed to cleanup trace engine, rc=%d.\n", rc);
		}

		rc = OMR_Thread_Free(omrVMThread);
		if (OMR_ERROR_NONE != rc) {
			omrtty_printf("Failed to free OMR VMThread for main thread, rc=%d.\n", rc);
		}
	} else {
		fprintf(stderr, "No OMR VMThread so skipping shutdown phases that required an attached thread, rc=%d.\n", rc);
		fflush(stderr);
	}

	if (NULL != omrVM) {
		OMRPORT_ACCESS_FROM_OMRVM(omrVM);
#if defined(OMR_GC)
		rc = OMR_GC_ShutdownHeap(omrVM);
		if (OMR_ERROR_NONE != rc) {
			omrtty_printf("Failed to shutdown Heap Mememory Management, rc=%d.\n", rc);
		}
#endif /* OMR_GC */

		OMR_Runtime *omrRuntime = omrVM->_runtime;
		if (NULL != omrRuntime) {
			omr_detach_vm_from_runtime(omrVM);
			omr_destroy_runtime(omrRuntime);
			omrmem_free_memory(omrRuntime);
			omrVM->_runtime = NULL;
		}
		omrmem_free_memory(omrVM);
	} else {
		fprintf(stderr, "No OMR VM so skipping shutdown phases that required an initialized VM, rc=%d.\n", rc);
		fflush(stderr);
	}
#endif  // 0
	return OmrSystemError::SUCCESS;
}

constexpr const SystemConfig System::DEFAULT_CONFIG;

System::~System() {
	PITH_ASSERT(state_ == SystemState::DEAD);
}

auto System::init(const SystemConfig& config) -> SystemError {
	PITH_ASSERT(state_ == SystemState::DEAD);

	PITH_ASSERT(omr_.init() == OmrSystemError::SUCCESS);
	PITH_ASSERT(heap_.init(config.heap_) == HeapError::SUCCESS);
	PITH_ASSERT(lock_.init() == Pith::SharedLockError::SUCCESS);

	state_ = SystemState::ACTIVE;
	return SystemError::SUCCESS;
}

auto System::kill() -> SystemError {
	PITH_ASSERT(state_ == SystemState::ACTIVE);

	PITH_ASSERT(omr_.kill() == OmrSystemError::SUCCESS);
	heap_.kill();
	lock_.kill();
	PITH_ASSERT(contexts_.empty());
	state_ = SystemState::DEAD;
	return SystemError::SUCCESS;
}

auto System::attach(Context* cx) -> AttachError {
	contexts_.insert(cx);
	return AttachError::SUCCESS;
}

auto System::detach(Context* cx) -> AttachError {
	contexts_.erase(cx);
	return AttachError::SUCCESS;
}

}  // namespace Om
