# find_package(Fmt REQUIRED)

add_library(ab-util
	src/ab-util-SharedLock.cpp
	src/ab-util-Process.cpp
)

target_compile_features(ab-util
	PUBLIC
		cxx_std_20
)

target_compile_options(ab-util
	PRIVATE
		${OMR_ENHANCED_WARNINGS_FLAG}
)

target_link_libraries(ab-util
	PUBLIC
		ab-base
		fmt::fmt
)

target_include_directories(ab-util
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
)

install(
	TARGETS
		ab-util
)

add_subdirectory(test)
