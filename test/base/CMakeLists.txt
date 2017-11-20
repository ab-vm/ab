
add_library(ab-test-base
	include/Ab/Test/Env.hpp
	src/ab-test-base-env.cpp
)

target_include_directories(ab-test-base
	PUBLIC
		include/
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
)

target_link_libraries(ab-test-base
	PUBLIC
		ab-base
		ab-util
		gtest
)

configure_file(
	include/Ab/Test/Config.hpp.in
	include/Ab/Test/Config.hpp
)
