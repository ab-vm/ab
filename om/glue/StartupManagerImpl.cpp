/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2015, 2017
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

#include "CollectorLanguageInterfaceImpl.hpp"
#include "GCExtensionsBase.hpp"
#if defined(OMR_GC_MODRON_SCAVENGER)
#include "ConfigurationGenerational.hpp"
#endif /* defined(OMR_GC_MODRON_SCAVENGER) */
#if defined(OMR_GC_SEGREGATED_HEAP)
#include "ConfigurationSegregated.hpp"
#endif /* defined(OMR_GC_SEGREGATED_HEAP) */
#include "ConfigurationFlat.hpp"
#include "MarkingScheme.hpp"
#include "VerboseManagerImpl.hpp"

#include "StartupManagerImpl.hpp"

#if defined(OMR_GC_SEGREGATED_HEAP)
#define OMR_SEGREGATEDHEAP "-Xgcpolicy:segregated"
#define OMR_SEGREGATEDHEAP_LENGTH 21
#endif /* defined(OMR_GC_SEGREGATED_HEAP) */

bool MM_StartupManagerImpl::handleOption(MM_GCExtensionsBase* extensions, char* option) {
	bool result = MM_StartupManager::handleOption(extensions, option);

	if (!result) {
#if defined(OMR_GC_SEGREGATED_HEAP)
		if (0 == strncmp(option, OMR_SEGREGATEDHEAP, OMR_SEGREGATEDHEAP_LENGTH)) {
			/* OMRTODO: when we have a flag in extensions to use a segregated heap,
			 * perhaps we should set it to true here..
			 */
			_useSegregatedGC = true;
			result = true;
		}
#endif /* defined(OMR_GC_SEGREGATED_HEAP) */
	}

	return result;
}

MM_CollectorLanguageInterface* MM_StartupManagerImpl::createCollectorLanguageInterface(
	MM_EnvironmentBase* env) {
	return MM_CollectorLanguageInterfaceImpl::newInstance(env);
}

MM_VerboseManagerBase* MM_StartupManagerImpl::createVerboseManager(MM_EnvironmentBase* env) {
	return MM_VerboseManagerImpl::newInstance(env, env->getOmrVM());
}

char* MM_StartupManagerImpl::getOptions(void) {
	char* options = getenv("OMR_GC_OPTIONS");
	return options;
}

MM_Configuration* MM_StartupManagerImpl::createConfiguration(MM_EnvironmentBase* env) {
#if defined(OMR_GC_MODRON_SCAVENGER)
	MM_GCExtensionsBase* ext = MM_GCExtensionsBase::getExtensions(env->getOmrVM());
#endif /* defined(OMR_GC_MODRON_SCAVENGER) */
#if defined(OMR_GC_SEGREGATED_HEAP)
	if (_useSegregatedGC) {
		return MM_ConfigurationSegregated::newInstance(env);
	} else
#endif /* OMR_GC_SEGREGATED_HEAP */
#if defined(OMR_GC_MODRON_SCAVENGER)
		if (ext->scavengerEnabled) {
		return MM_ConfigurationGenerational::newInstance(env);
	} else
#endif /* defined(OMR_GC_MODRON_SCAVENGER) */
	{
		return MM_ConfigurationFlat::newInstance(env);
	}
}
