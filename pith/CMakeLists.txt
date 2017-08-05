cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

set(PITH_VERSION_MAJOR 0)
set(PITH_VERSION_MINOR 1)
set(PITH_VERSION_PATCH 0)

set(PITH_VERSION "${PITH_VERSION_MAJOR}.${PITH_VERSION_MINOR}.${PITH_VERSION_PATCH}")

set(PITH_COMMIT ${GIT_COMMIT})

project(pith VERSION ${PITH_VERSION} LANGUAGES CXX)

configure_file(
	include/Pith/Config.hpp.in
	include/Pith/Config.hpp
)

add_library(pith SHARED
	src/SharedLock.cpp
	src/Pith.cpp
	src/Version.cpp
)

target_include_directories(pith
	PUBLIC
		$<BUILD_INTERFACE: include>
		$<BUILD_INTERFACE: ${CMAKE_CURRENT_BINARY_DIR}/include>
)

add_subdirectory(test)

target_link_libraries(pith
	PRIVATE
		"-e pith_main"
)