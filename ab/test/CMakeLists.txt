ab_setup()

include_directories(include)

add_ab_test(TestMemory ab)
add_ab_test(TestInterpreter ab)
add_ab_test(TestProcess ab)
add_ab_test(TestReader ab)

add_subdirectory(Wasm)
