set(OM_VERSION_MAJOR 0)
set(OM_VERSION_MINOR 0)
set(OM_VERSION_PATCH 1)

set(OM_COMMIT ${GIT_COMMIT})
set(OM_VERSION ${OM_VERSION_MAJOR}.${OM_VERSION_MINOR}.${OM_VERSION_PATCH})

project(om
	VERSION ${OM_VERSION}
	LANGUAGES CXX
)

enable_testing()

configure_file(
	include/Om/Config.hpp.in
	include/Om/Config.hpp
)

set(OM_DEBUG On CACHE Bool "Enable/Disable Om debug features and checks.")

add_library(om
	src/Context.cpp
	src/Heap.cpp
	src/System.cpp
	src/Version.cpp
)

set_property(TARGET om PROPERTY CXX_STANDARD 11)

target_include_directories(om
	PUBLIC
		$<BUILD_INTERFACE: include>
		$<BUILD_INTERFACE: ${CMAKE_CURRENT_BINARY_DIR}/include>
)

target_link_libraries(om
	PUBLIC
		pith
)

add_subdirectory(glue)