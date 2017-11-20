add_library(ab-test-main INTERFACE)

target_sources(ab-test-main
	INTERFACE
		${CMAKE_CURRENT_SOURCE_DIR}/src/ab-test-main-main.cpp
)

target_link_libraries(ab-test-main
	INTERFACE
		ab-base
		ab-util
		ab-test-base
		gtest
)
