# define a new test binary.
# Usage: add_ab_util_test(<name> [<libs>...])
function(add_ab_util_test name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} gtest_main ab-util ${ARGN})
    add_test(${name} ${name})
    set_target_properties(${name}
        PROPERTIES
            CXX_CLANG_TIDY ""  # Disable because gtest errors a lot.
    )
endfunction(add_ab_util_test)

add_ab_util_test(TestArrayBox)
add_ab_util_test(TestAssert)
add_ab_util_test(TestBox)
add_ab_util_test(TestConstant)
add_ab_util_test(TestMaybe)
add_ab_util_test(TestPage)
add_ab_util_test(TestResult)
add_ab_util_test(TestSexpr)
add_ab_util_test(TestSharedLock)
add_ab_util_test(TestSpan)
add_ab_util_test(TestStringSpan)
add_ab_util_test(TestVarInt)
add_ab_util_test(TestVec)
