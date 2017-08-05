
# define a new test binary.
function(add_om_test name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} om gtest_main)
    add_test(${name} ${name})
    set_target_properties(${name}
        PROPERTIES
            CXX_CLANG_TIDY ""  # Disable because gtest errors a lot.
            CXX_STANDARD 11
    )
endfunction(add_om_test)