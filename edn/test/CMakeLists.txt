add_executable(ab-edn-test
	ab-edn-test-lexer.cpp
	ab-edn-test-parser.cpp
	ab-edn-test-token-tree.cpp
	ab-edn-test-ir.cpp
)

target_link_libraries(ab-edn-test
	PRIVATE
		ab-edn
		ab-util
		ab-test-base
		ab-test-main
)

add_test(
	NAME run-ab-edn-test
	COMMAND ab-edn-test --gtest_color=yes
)
