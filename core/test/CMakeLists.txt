include(AbTest)

include_directories(include)

add_ab_test(ab-test-linear-memory ab-core)
add_ab_test(ab-test-InterpreterTest ab-core)
add_ab_test(ab-test-ProcessTest ab-core)
add_ab_test(TestXFunctionBuilder ab-core)

#add_ab_test(ab-test-ReaderTest ab-core)

add_subdirectory(Wasm)
