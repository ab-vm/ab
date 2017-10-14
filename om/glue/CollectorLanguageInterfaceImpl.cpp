/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 1991, 2017
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
 *******************************************************************************/
#include "j9nongenerated.h"
#include "modronbase.h"

#if defined(OMR_GC_MODRON_CONCURRENT_MARK)
#include "CardTable.hpp"
#endif /* OMR_GC_MODRON_CONCURRENT_MARK */
#include "CollectorLanguageInterfaceImpl.hpp"
#if defined(OMR_GC_MODRON_CONCURRENT_MARK)
#include "ConcurrentSafepointCallback.hpp"
#endif /* OMR_GC_MODRON_CONCURRENT_MARK */
#if defined(OMR_GC_MODRON_COMPACTION)
#include "CompactScheme.hpp"
#endif /* OMR_GC_MODRON_COMPACTION */
#include "EnvironmentStandard.hpp"
#include "ForwardedHeader.hpp"
#include "GCExtensionsBase.hpp"
#include "HeapLinkedFreeHeader.hpp"
#include "MarkingScheme.hpp"
#include "MemorySubSpaceSemiSpace.hpp"
#include "MixedObjectScanner.hpp"
#include "OMRVMInterface.hpp"
#include "ObjectIterator.hpp"
#include "ObjectModel.hpp"
#include "ParallelGlobalGC.hpp"
#include "Scavenger.hpp"
#include "SlotObject.hpp"
#include "mminitcore.h"
#include "objectdescription.h"
#include "omr.h"
#include "omrExampleVM.hpp"
#include "omrvm.h"

/* This enum extends ConcurrentStatus with values > CONCURRENT_ROOT_TRACING. Values from this
 * and from ConcurrentStatus are treated as uintptr_t values everywhere except when used as
 * case labels in switch() statements where manifest constants are required.
 *
 * ConcurrentStatus extensions allow the client language to define discrete units of work
 * that can be executed in parallel by concurrent threads. ConcurrentGC will call
 * MM_CollectorLanguageInterfaceImpl::concurrentGC_collectRoots(..., concurrentStatus, ...)
 * only once with each client-defined status value. The thread that receives the call
 * can check the concurrentStatus value to select and execute the appropriate unit of work.
 */
enum { CONCURRENT_ROOT_TRACING1 = ((uintptr_t)((uintptr_t)CONCURRENT_ROOT_TRACING + 1)) };

/**
 * Initialization
 */
MM_CollectorLanguageInterfaceImpl*
MM_CollectorLanguageInterfaceImpl::newInstance(MM_EnvironmentBase* env) {
	MM_CollectorLanguageInterfaceImpl* cli = NULL;
	OMR_VM* omrVM = env->getOmrVM();
	MM_GCExtensionsBase* extensions = MM_GCExtensionsBase::getExtensions(omrVM);

	cli = (MM_CollectorLanguageInterfaceImpl*)extensions->getForge()->allocate(
		sizeof(MM_CollectorLanguageInterfaceImpl), MM_AllocationCategory::FIXED,
		OMR_GET_CALLSITE());
	if (NULL != cli) {
		new (cli) MM_CollectorLanguageInterfaceImpl(omrVM);
		if (!cli->initialize(omrVM)) {
			cli->kill(env);
			cli = NULL;
		}
	}

	return cli;
}

void MM_CollectorLanguageInterfaceImpl::kill(MM_EnvironmentBase* env) {
	OMR_VM* omrVM = env->getOmrVM();
	tearDown(omrVM);
	MM_GCExtensionsBase::getExtensions(omrVM)->getForge()->free(this);
}

void MM_CollectorLanguageInterfaceImpl::tearDown(OMR_VM* omrVM) {
}

bool MM_CollectorLanguageInterfaceImpl::initialize(OMR_VM* omrVM) {
	return true;
}
#if defined(OMR_GC_MODRON_SCAVENGER)
void MM_CollectorLanguageInterfaceImpl::scavenger_masterSetupForGC(MM_EnvironmentBase* env) {
	/* Do nothing for now */
}

void MM_CollectorLanguageInterfaceImpl::scavenger_workerSetupForGC_clearEnvironmentLangStats(
	MM_EnvironmentBase* env) {
	/* Do nothing for now */
}

void MM_CollectorLanguageInterfaceImpl::scavenger_reportScavengeEnd(
	MM_EnvironmentBase* envBase, bool scavengeSuccessful) {
	/* Do nothing for now */
}

void MM_CollectorLanguageInterfaceImpl::scavenger_mergeGCStats_mergeLangStats(
	MM_EnvironmentBase* envBase) {
	/* Do nothing for now */
}

void MM_CollectorLanguageInterfaceImpl::scavenger_masterThreadGarbageCollect_scavengeComplete(
	MM_EnvironmentBase* envBase) {
	/* Do nothing for now */
}

void MM_CollectorLanguageInterfaceImpl::scavenger_masterThreadGarbageCollect_scavengeSuccess(
	MM_EnvironmentBase* envBase) {
	/* Do nothing for now */
}

bool MM_CollectorLanguageInterfaceImpl::
	scavenger_internalGarbageCollect_shouldPercolateGarbageCollect(
		MM_EnvironmentBase* envBase, PercolateReason* reason, uint32_t* gcCode) {
	/* Do nothing for now */
	return false;
}

GC_ObjectScanner* MM_CollectorLanguageInterfaceImpl::scavenger_getObjectScanner(
	MM_EnvironmentStandard* env, omrobjectptr_t objectPtr, void* allocSpace, uintptr_t flags) {
#if defined(OMR_GC_MODRON_SCAVENGER_STRICT)
	Assert_MM_true(
		(GC_ObjectScanner::scanHeap == flags) ^ (GC_ObjectScanner::scanRoots == flags));
#endif /* defined(OMR_GC_MODRON_SCAVENGER_STRICT) */
	GC_ObjectScanner* objectScanner = NULL;
	objectScanner = GC_MixedObjectScanner::newInstance(env, objectPtr, allocSpace, flags);
	return objectScanner;
}

void MM_CollectorLanguageInterfaceImpl::scavenger_flushReferenceObjects(
	MM_EnvironmentStandard* env) {
	/* Do nothing for now */
}

bool MM_CollectorLanguageInterfaceImpl::scavenger_hasIndirectReferentsInNewSpace(
	MM_EnvironmentStandard* env, omrobjectptr_t objectPtr) {
	/* This method must be implemented and return true an object may hold any object references
	 * that are live but not reachable by traversing the reference graph from the root set or
	 * remembered set. Otherwise this default implementation should be used.
	 */
	return false;
}

bool MM_CollectorLanguageInterfaceImpl::scavenger_scavengeIndirectObjectSlots(
	MM_EnvironmentStandard* env, omrobjectptr_t objectPtr) {
	/* This method must be implemented if an object may hold any object references that are live
	 * but not reachable by traversing the reference graph from the root set or remembered set.
	 * In that case, this method should call MM_Scavenger::copyObjectSlot(..) for each such
	 * indirect object reference, ORing the boolean result from each call to
	 * MM_Scavenger::copyObjectSlot(..) into a single boolean value to be returned.
	 */
	return false;
}

void MM_CollectorLanguageInterfaceImpl::scavenger_backOutIndirectObjectSlots(
	MM_EnvironmentStandard* env, omrobjectptr_t objectPtr) {
	/* This method must be implemented if an object may hold any object references that are live
	 * but not reachable by traversing the reference graph from the root set or remembered set.
	 * In that case, this method should call MM_Scavenger::backOutFixSlotWithoutCompression(..)
	 * for each uncompressed slot holding a reference to an indirect object that is associated
	 * with the object.
	 */
}

void MM_CollectorLanguageInterfaceImpl::scavenger_backOutIndirectObjects(
	MM_EnvironmentStandard* env) {
	/* This method must be implemented if an object may hold any object references that are live
	 * but not reachable by traversing the reference graph from the root set or remembered set.
	 * In that case, this method should locate all such objects and call
	 * MM_Scavenger::backOutObjectScan(..) for each such object that is in the remembered set.
	 * For example,
	 *
	 * if (_extensions->objectModel.isRemembered(indirectObject)) {
	 *    _extensions->scavenger->backOutObjectScan(env, indirectObject);
	 * }
	 */
}

void MM_CollectorLanguageInterfaceImpl::scavenger_reverseForwardedObject(
	MM_EnvironmentBase* env, MM_ForwardedHeader* forwardedHeader) {
	if (forwardedHeader->isForwardedPointer()) {
		omrobjectptr_t originalObject = forwardedHeader->getObject();
		omrobjectptr_t forwardedObject = forwardedHeader->getForwardedObject();

		/* Restore the original object header from the forwarded object */
		GC_ObjectModel* objectModel = &(env->getExtensions()->objectModel);
		objectModel->setObjectSizeAndFlags(
			originalObject,
			objectModel->getConsumedSizeInBytesWithHeader(forwardedObject),
			objectModel->getObjectFlags(forwardedObject));

#if defined(OMR_INTERP_COMPRESSED_OBJECT_HEADER)
		/* Restore destroyed overlapped slot in the original object. This slot might need to
		 * be reversed as well or it may be already reversed - such fixup will be completed
		 * at in a later pass.
		 */
		forwardedHeader->restoreDestroyedOverlap();
#endif /* defined (OMR_INTERP_COMPRESSED_OBJECT_HEADER) */
	}
}

#if defined(OMR_INTERP_COMPRESSED_OBJECT_HEADER)
void MM_CollectorLanguageInterfaceImpl::scavenger_fixupDestroyedSlot(
	MM_EnvironmentBase* env, MM_ForwardedHeader* forwardedHeader,
	MM_MemorySubSpaceSemiSpace* subSpaceNew) {
	/* This method must be implemented if (and only if) the object header is stored in a
	 * compressed slot. in that case the other half of the full (omrobjectptr_t sized) slot may
	 * hold a compressed object reference that must be restored by this method.
	 */
	/* This assumes that all slots are object slots, including the slot adjacent to the header
	 * slot */
	if ((0 != forwardedHeader->getPreservedOverlap()) &&
	    !_extensions->objectModel.isIndexable(forwardedHeader)) {
		MM_GCExtensionsBase* extensions = MM_GCExtensionsBase::getExtensions(_omrVM);
		/* Get the uncompressed reference from the slot */
		fomrobject_t preservedOverlap =
			(fomrobject_t)forwardedHeader->getPreservedOverlap();
		GC_SlotObject preservedSlotObject(_omrVM, &preservedOverlap);
		omrobjectptr_t survivingCopyAddress = preservedSlotObject.readReferenceFromSlot();
		/* Check if the address we want to read is aligned (since mis-aligned reads may
		 * still be less than a top address but extend beyond it) */
		if (0 ==
		    ((uintptr_t)survivingCopyAddress &
		     (extensions->getObjectAlignmentInBytes() - 1))) {
			/* Ensure that the address we want to read is within part of the heap which
			 * could contain copied objects (tenure or survivor) */
			void* topOfObject = (void*)((uintptr_t*)survivingCopyAddress + 1);
			if (subSpaceNew->isObjectInNewSpace(survivingCopyAddress, topOfObject) ||
			    extensions->isOld(survivingCopyAddress, topOfObject)) {
				/* if the slot points to a reverse-forwarded object, restore the
				 * original location (in evacuate space) */
				MM_ForwardedHeader reverseForwardedHeader(survivingCopyAddress);
				if (reverseForwardedHeader.isReverseForwardedPointer()) {
					/* overlapped slot must be fixed up */
					fomrobject_t fixupSlot = 0;
					GC_SlotObject fixupSlotObject(_omrVM, &fixupSlot);
					fixupSlotObject.writeReferenceToSlot(
						reverseForwardedHeader
							.getReverseForwardedPointer());
					forwardedHeader->restoreDestroyedOverlap(
						(uint32_t)fixupSlot);
				}
			}
		}
	}
}
#endif /* OMR_INTERP_COMPRESSED_OBJECT_HEADER */
#endif /* OMR_GC_MODRON_SCAVENGER */

#if defined(OMR_GC_MODRON_COMPACTION)
void MM_CollectorLanguageInterfaceImpl::compactScheme_verifyHeap(
	MM_EnvironmentBase* env, MM_MarkMap* markMap) {
	Assert_MM_unimplemented();
}

void MM_CollectorLanguageInterfaceImpl::compactScheme_fixupRoots(
	MM_EnvironmentBase* env, MM_CompactScheme* compactScheme) {
	Assert_MM_unimplemented();
}

void MM_CollectorLanguageInterfaceImpl::compactScheme_workerCleanupAfterGC(
	MM_EnvironmentBase* env) {
	Assert_MM_unimplemented();
}

void MM_CollectorLanguageInterfaceImpl::compactScheme_languageMasterSetupForGC(
	MM_EnvironmentBase* env) {
	Assert_MM_unimplemented();
}
#endif /* OMR_GC_MODRON_COMPACTION */

#if defined(OMR_GC_MODRON_CONCURRENT_MARK)
MM_ConcurrentSafepointCallback*
MM_CollectorLanguageInterfaceImpl::concurrentGC_createSafepointCallback(MM_EnvironmentBase* env) {
	MM_EnvironmentStandard* envStd = MM_EnvironmentStandard::getEnvironment(env);
	return MM_ConcurrentSafepointCallback::newInstance(envStd);
}

uintptr_t
MM_CollectorLanguageInterfaceImpl::concurrentGC_getNextTracingMode(uintptr_t executionMode) {
	uintptr_t nextExecutionMode = CONCURRENT_TRACE_ONLY;
	switch (executionMode) {
	case CONCURRENT_ROOT_TRACING:
		nextExecutionMode = CONCURRENT_ROOT_TRACING1;
		break;
	case CONCURRENT_ROOT_TRACING1:
		nextExecutionMode = CONCURRENT_TRACE_ONLY;
		break;
	default:
		Assert_MM_unreachable();
	}

	return nextExecutionMode;
}

uintptr_t MM_CollectorLanguageInterfaceImpl::concurrentGC_collectRoots(
	MM_EnvironmentStandard* env, uintptr_t concurrentStatus, bool* collectedRoots,
	bool* paidTax) {
	MM_ParallelGlobalGC* globalCollector =
		(MM_ParallelGlobalGC*)_extensions->getGlobalCollector();
	MM_MarkingScheme* markingScheme = globalCollector->getMarkingScheme();
	uintptr_t bytesScanned = 0;
	*collectedRoots = true;
	*paidTax = true;

	switch (concurrentStatus) {
	case CONCURRENT_ROOT_TRACING1:
		markingScheme->markLiveObjectsRoots(env);
		break;
	default:
		Assert_MM_unreachable();
	}

	return bytesScanned;
}
#endif /* OMR_GC_MODRON_CONCURRENT_MARK */
