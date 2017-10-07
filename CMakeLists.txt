cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(ab-vm LANGUAGES C CXX)

list(APPEND CMAKE_MODULE_PATH
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules
	${CMAKE_CURRENT_SOURCE_DIR}/external/omr/cmake/modules
)

include(FindClangTidy)
include(OmrPlatform)
include(Utilities)

# Global Configuration

omr_platform_global_setup()

get_git_commit(GIT_COMMIT)

enable_testing()

# OMR Configuration

set(OMR_GLUE_TARGET "om_omr_glue" CACHE PATH "Om Implements the OMR glue")
set(OMR_FVTEST OFF CACHE BOOL "Disable OMR Testing in Ab")
set(OMR_GC ON CACHE BOOL "Enable OMR GC in Ab, for Om")
set(OMR_JITBUILDER ON CACHE BOOL "Enable OMR Jitbuilder, for Ab")
set(OMR_WARNINGS_AS_ERRORS OFF CACHE BOOL "Disable warnings as errors")

# OMR Glue

set(OMR_GC_GLUE_TARGET "om_gc_glue" CACHE STRING "The gc glue target, must be interface library")
set(OMR_UTIL_GLUE_TARGET "om_util_glue" CACHE STRING "The util glue target, must be interface library")
set(OMR_RAS_GLUE_TARGET "om_ras_glue" CACHE STRING "The ras glue target, must be interface library")
set(OMR_CORE_GLUE_TARGET "om_core_glue" CACHE STRING "The core glue target, must be and interface library")

# Misc utilities

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

# Project setup

macro(ab_setup)

	# Use C++14

	set(CMAKE_CXX_STANDARD 14)
	set(CMAKE_CXX_STANDARD_REQUIRED true)
	set(CMAKE_CXX_EXTENSIONS off)

	# Compilation warnings

	add_compile_options(-Wall -Wextra -Wno-unused-parameter)

	# Use clang tidy for static analysis

	if(CLANG_TIDY_FOUND)
		set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_EXECUTABLE})
	endif()

endmacro()

# Sub directories

add_subdirectory(ab)
add_subdirectory(abi)
add_subdirectory(doc)
add_subdirectory(external)
add_subdirectory(om)
add_subdirectory(pith)
