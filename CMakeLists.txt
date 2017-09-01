cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(ab-vm LANGUAGES C CXX)

list(APPEND CMAKE_MODULE_PATH
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules
	${CMAKE_CURRENT_SOURCE_DIR}/external/omr/cmake/modules
)

# OMR Configuration

set(OMR_GLUE_TARGET "om_omr_glue" CACHE PATH "Om Implements the OMR glue")
set(OMR_FVTEST OFF CACHE BOOL "Disable OMR Testing in Ab")
set(OMR_GC ON CACHE BOOL "Enable OMR GC in Ab, for Om")
set(OMR_JITBUILDER ON CACHE BOOL "Enable OMR Jitbuilder, for Ab")
set(OMR_WARNINGS_AS_ERRORS OFF CACHE BOOL "Disable warnings as errors")

# Global Configuration

include(OmrPlatform)
include(Utilities)

get_git_commit(GIT_COMMIT)

enable_testing()

# Misc utilities

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

# Project setup

macro(ab_setup)

	# Use C++14

	set(CMAKE_CXX_STANDARD 14)
	set(CMAKE_CXX_STANDARD_REQUIRED true)
	set(CMAKE_CXX_EXTENSIONS off)

	# Compilation warnings

	add_compile_options(-Werror -Wall -Wextra)

	# Use clang tidy for static analysis

	if(CLANG_TIDY_FOUND)
		set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_EXECUTABLE})
	endif()

endmacro()

# Sub directories

add_subdirectory(ab)
add_subdirectory(doc)
add_subdirectory(external)
add_subdirectory(om)
add_subdirectory(pith)
