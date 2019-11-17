add_executable(ab-core-test
	ab-core-test-interpreter.cpp
	ab-core-test-linear-memory.cpp
	ab-core-test-main.cpp
	ab-core-test-process.cpp
	ab-core-test-runtime-env.cpp
	ab-core-test-func-builder.cpp
)

target_include_directories(ab-core-test
	PRIVATE
		include/
)

target_link_libraries(ab-core-test
	PRIVATE
		ab-core
		ab-test-base
		gtest
)

add_test(run-ab-core-test
	COMMAND ab-core-test
)

# TODO: Move WASM Reader test out of core.
