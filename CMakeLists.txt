cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

list(APPEND CMAKE_MODULE_PATH
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules
	${CMAKE_CURRENT_SOURCE_DIR}/external/omr/cmake/modules
)

# Global configuration

project(ab-vm)

include(Utilities)

enable_testing()

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

get_git_commit(GIT_COMMIT)

# OMR Configuration

set(OMR_GLUE_TARGET "om_glue" CACHE PATH "Om Implements the OMR glue" FORCE)
set(OMR_FVTEST     OFF CACHE BOOL "Disable OMR Testing in Ab" FORCE)
set(OMR_GC         ON  CACHE BOOL "Enable OMR GC in Ab, for Om" FORCE)
set(OMR_JITBUILDER OFF CACHE BOOL "Enable OMR Jitbuilder, for Ab" FORCE)

# Sub directories

add_subdirectory(ab)
add_subdirectory(external)
add_subdirectory(om)
add_subdirectory(pith)
