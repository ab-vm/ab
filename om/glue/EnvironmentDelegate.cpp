/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2017
 *
 *  This program and the accompanying materials are made available
 *  under the terms of the Eclipse Public License v1.0 and
 *  Apache License v2.0 which accompanies this distribution.
 *
 *      The Eclipse Public License is available at
 *      http://www.eclipse.org/legal/epl-v10.html
 *
 *      The Apache License v2.0 is available at
 *      http://www.opensource.org/licenses/apache2.0.php
 *
 * Contributors:
 *    Multiple authors (IBM Corp.) - initial implementation and documentation
 ******************************************************************************/

#include "ModronAssertions.h"
#include "omr.h"
#include "omrExampleVM.hpp"

#include "AtomicOperations.hpp"
#include "EnvironmentDelegate.hpp"
#include "EnvironmentStandard.hpp"
#include "GCExtensionsBase.hpp"
#include "SublistFragment.hpp"

OMR_VMThread*
MM_EnvironmentDelegate::attachVMThread(OMR_VM* omrVM, const char* threadName, uintptr_t reason) {
	OMR_VMThread* omrVMThread = NULL;
	omr_error_t rc            = OMR_ERROR_NONE;

	rc = OMR_Glue_BindCurrentThread(omrVM, threadName, &omrVMThread);
	if (OMR_ERROR_NONE != rc) {
		return NULL;
	}
	return omrVMThread;
}

void MM_EnvironmentDelegate::detachVMThread(
	OMR_VM* omrVM, OMR_VMThread* omrVMThread, uintptr_t reason) {
	if (NULL != omrVMThread) {
		OMR_Glue_UnbindCurrentThread(omrVMThread);
	}
}

void MM_EnvironmentDelegate::acquireVMAccess() {
	OMR_VM_Example* exampleVM = (OMR_VM_Example*)_env->getOmrVM()->_language_vm;
	omrthread_rwmutex_enter_read(exampleVM->_vmAccessMutex);
}

/**
 * Release shared VM acccess.
 */
void MM_EnvironmentDelegate::releaseVMAccess() {
	OMR_VM_Example* exampleVM = (OMR_VM_Example*)_env->getOmrVM()->_language_vm;
	omrthread_rwmutex_exit_read(exampleVM->_vmAccessMutex);
}

/**
 * Check whether another thread is requesting exclusive VM access. This method must be
 * called frequently by all threads that are holding shared VM access. If this method
 * returns true, the calling thread should release shared VM access as quickly as possible
 * and reacquire it if necessary.

 * @return true if another thread is waiting to acquire exclusive VM access
 */
bool MM_EnvironmentDelegate::isExclusiveAccessRequestWaiting() {
	OMR_VM_Example* exampleVM = (OMR_VM_Example*)_env->getOmrVM()->_language_vm;
	if ((0 < exampleVM->_vmExclusiveAccessCount) ||
	    omrthread_rwmutex_is_writelocked(exampleVM->_vmAccessMutex)) {
		return true;
	}
	if (NULL != _env->getExtensions()->gcExclusiveAccessThreadId) {
		return true;
	}
	return false;
}

/**
 * Acquire exclusive VM access. This method should only be called by the OMR runtime to
 * perform stop-the-world operations such as garbage collection. Calling thread will be
 * blocked until all other threads holding shared VM access have release VM access.
 */
void MM_EnvironmentDelegate::acquireExclusiveVMAccess() {
	if (0 == _env->getOmrVMThread()->exclusiveCount) {
		OMR_VM* omrVM             = _env->getOmrVM();
		OMR_VM_Example* exampleVM = (OMR_VM_Example*)omrVM->_language_vm;

		/* tell the rest of the world that a thread is going for exclusive VM< access */
		MM_AtomicOperations::add(&exampleVM->_vmExclusiveAccessCount, 1);

		/* unconditionally acquire exclusive VM access by locking the VM thread list mutex
		 */
		omrthread_rwmutex_enter_write(exampleVM->_vmAccessMutex);
		omrthread_monitor_enter(omrVM->_vmThreadListMutex);
	}
	_env->getOmrVMThread()->exclusiveCount += 1;
}

/**
 * Release exclusive VM acccess. If no other thread is waiting for exclusive VM access
 * this method will notify all threads waiting for shared VM access to continue and
 * acquire shared VM access.
 */
void MM_EnvironmentDelegate::releaseExclusiveVMAccess() {
	if (1 == _env->getOmrVMThread()->exclusiveCount) {
		OMR_VM_Example* exampleVM = (OMR_VM_Example*)_env->getOmrVM()->_language_vm;
		omrthread_monitor_exit(_env->getOmrVM()->_vmThreadListMutex);
		omrthread_rwmutex_exit_write(exampleVM->_vmAccessMutex);
		Assert_MM_true(0 < exampleVM->_vmExclusiveAccessCount);
		MM_AtomicOperations::subtract(&exampleVM->_vmExclusiveAccessCount, 1);
		_env->getOmrVMThread()->exclusiveCount -= 1;
	} else if (1 < _env->getOmrVMThread()->exclusiveCount) {
		_env->getOmrVMThread()->exclusiveCount -= 1;
	}
}

/**
 * Give up exclusive access in preparation for transferring it to a collaborating thread
 * (i.e. main-to-master or master-to-main). This may involve nothing more than
 * transferring OMR_VMThread::exclusiveCount from the owning thread to the another
 * thread that thereby assumes exclusive access. Implement if this kind of collaboration
 * is required.
 *
 * @return the exclusive count of the current thread before relinquishing
 * @see assumeExclusiveVMAccess(uintptr_t)
 */
uintptr_t MM_EnvironmentDelegate::relinquishExclusiveVMAccess(bool* deferredVMAccessRelease) {
	uintptr_t relinquishedExclusiveCount   = _env->getOmrVMThread()->exclusiveCount;
	_env->getOmrVMThread()->exclusiveCount = 0;
	return relinquishedExclusiveCount;
}

/**
 * Assume exclusive access from a collaborating thread (i.e. main-to-master or master-to-main).
 * Implement if this kind of collaboration is required.
 *
 * @param exclusiveCount the exclusive count to be restored
 * @see relinquishExclusiveVMAccess()
 */
void MM_EnvironmentDelegate::assumeExclusiveVMAccess(uintptr_t exclusiveCount) {
	_env->getOmrVMThread()->exclusiveCount = exclusiveCount;
}
