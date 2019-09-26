cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

# Module path

list(APPEND CMAKE_MODULE_PATH
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules
)

# Versioning

set(AB_VERSION_MAJOR 0)
set(AB_VERSION_MINOR 0)
set(AB_VERSION_PATCH 1)
set(AB_VERSION ${AB_VERSION_MAJOR}.${AB_VERSION_MINOR}.${AB_VERSION_PATCH})

project(abigail
	VERSION ${AB_VERSION}
	LANGUAGES C CXX
)

# Module Imports

include(CPack)
include(FindClangTidy)
include(AbUtilities)

if (AB_USE_OMR)
	include(UseOmr)
endif(AB_USE_OMR)

# Global Configuration

get_git_commit(GIT_COMMIT)

set(AB_COMMIT ${GIT_COMMIT})

configure_file(
	"Config.hpp.in"
	"include/Ab/Config.hpp"
)

enable_testing()

link_directories(/usr/local/lib)

set(AB_COLOR_DIAGNOSTICS on CACHE BOOL "Enable/Disable colorized compiler output")

# Misc utilities

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

if(CLANG_TIDY_FOUND)
	set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_EXECUTABLE})
endif()

# ab_base: basic configuration

add_library(ab-base INTERFACE)

target_compile_features(ab-base
	INTERFACE
		cxx_std_17
)

target_compile_options(ab-base
	INTERFACE
		-Wall -Wextra
)

if(AB_COLOR_DIAGNOSTICS)
	target_compile_options(ab-base
		INTERFACE
			-fdiagnostics-color=always
	)
endif()

target_include_directories(ab-base
	INTERFACE
		${CMAKE_CURRENT_BINARY_DIR}/include
)

# External projects

add_subdirectory(external)

# Sub directories

add_subdirectory(ab)
add_subdirectory(asm)
add_subdirectory(commands)
add_subdirectory(core)
add_subdirectory(disasm)
add_subdirectory(doc)
add_subdirectory(edn)
add_subdirectory(help)
add_subdirectory(interpreter)
add_subdirectory(man)
add_subdirectory(parse)
add_subdirectory(run)
add_subdirectory(test)
add_subdirectory(util)
add_subdirectory(wast)
add_subdirectory(version)
