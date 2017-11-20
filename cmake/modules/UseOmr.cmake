if(USE_OMR_)
	return()
endif()
set(USE_OMR_ 1)

list(APPEND CMAKE_MODULE_PATH
	${CMAKE_CURRENT_SOURCE_DIR}/external/omr/cmake/modules
)

include(OmrPlatform)

omr_platform_global_setup()

# OMR Configuration
	
set(OMR_EXAMPLE OFF CACHE INTERNAL "Disable the OMR example VM")
set(OMR_FVTEST OFF CACHE INTERNAL "Disable OMR Testing in Ab")
set(OMR_GC ON CACHE INTERNAL "Enable OMR GC in Ab, for Om")
set(OMR_JITBUILDER ON CACHE INTERNAL "Enable OMR Jitbuilder, for Ab")
set(OMR_OM ON CACHE INTERNAL "Enable Om")
set(OMR_WARNINGS_AS_ERRORS OFF CACHE INTERNAL "Disable warnings as errors")

