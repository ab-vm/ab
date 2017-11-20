add_executable(ab-parse-test
	ab-parse-test-location.cpp
)

target_link_libraries(ab-parse-test
	PRIVATE
		ab-parse
		gtest_main
)

add_test(run-ab-parse-test
	COMMAND ab-parse-test-location
)
