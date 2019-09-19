add_executable(ab-parse-test
	ab-parse-test-location.cpp
)

target_link_libraries(ab-parse-test
	PRIVATE
		ab-parse
		gtest_main
)

add_test(
	NAME run-ab-parse-test
	COMMAND ab-parse-test --gtest_color=yes
)
