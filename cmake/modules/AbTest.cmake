if(_AB_TEST_INC)
	return()
endif()
set(_AB_TEST_INC true)

# define a new test binary.
# Usage: add_ab_test(<name> [<libs>...])
# Note that gtest_main will be automatically linked into the executable. You do not need to define main().
function(add_ab_test name)
	add_executable(${name} ${name}.cpp)
	target_link_libraries(${name} gtest_main ${ARGN})
	add_test(${name} ${name})
	set_target_properties(${name}
		PROPERTIES
			CXX_CLANG_TIDY ""  # Disable because gtest errors a lot.
			CXX_STANDARD 17
	)
endfunction(add_ab_test)
