set(AB_VERSION_MAJOR 0)
set(AB_VERSION_MINOR 0)
set(AB_VERSION_PATCH 1)

set(AB_COMMIT ${GIT_COMMIT})
set(AB_VERSION ${AB_VERSION_MAJOR}.${AB_VERSION_MINOR}.${AB_VERSION_PATCH})

project(ab
	VERSION ${AB_VERSION}
	LANGUAGES CXX
)

configure_file(
	"include/Ab/Config.hpp.in"
	"include/Ab/Config.hpp"
)

add_library(ab
	src/ErrorCategory.cpp
	src/Process.cpp
	src/Version.cpp
	src/Wasm/Loader.cpp
)

target_compile_features(ab
	PUBLIC
		cxx_auto_type
		cxx_variadic_templates
)

target_include_directories(ab
	PUBLIC
		$<BUILD_INTERFACE: include>
		$<BUILD_INTERFACE: ${CMAKE_CURRENT_BINARY_DIR}/include>
)

target_link_libraries(ab
	om
	pith
)

add_subdirectory(test)