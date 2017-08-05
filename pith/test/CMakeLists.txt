# define a new test binary.
# Usage: add_pith_test(<name> [<libs>...])
function(add_pith_test name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} gtest_main pith ${ARGN})
    add_test(${name} ${name})
    set_target_properties(${name}
        PROPERTIES
            CXX_CLANG_TIDY ""  # Disable because gtest errors a lot.
            CXX_STANDARD 11
    )
endfunction(add_pith_test)

add_pith_test(TestArray)
add_pith_test(TestAssert)
add_pith_test(TestBox)
add_pith_test(TestConstant)
add_pith_test(TestLeb128)
add_pith_test(TestMaybe)
add_pith_test(TestResult)
add_pith_test(TestSharedLock)
add_pith_test(TestVarInt)
