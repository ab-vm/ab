function(get_git_commit output_variable)
	exec_program(
		"git"
		${CMAKE_CURRENT_SOURCE_DIR}
		ARGS "describe" "--all" "--long" "HEAD"
		OUTPUT_VARIABLE ${output_variable}
	)
endfunction(get_git_commit)

function(project_version project major minor patch)
	set(${project}_MAJOR CACHE STRING "Major version number" FORCE)
	set(${project}_MINOR CACHE STRING "minor version number" FORCE)
	set(${project}_PATCH CACHE STRING "Patch number" FORCE)
	set(${project}_COMMIT CACHE STRING "COMMIT-NOTFOUND" FORCE)

	get_git_commit(${project}_COMMIT)

	set(${project}_VERSION CACHE STRING
		"${major}.${minor}.${patch}"
		"Project Version" FORCE
	)
endfunction(project_version)

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
			CXX_STANDARD 14
	)
endfunction(add_ab_test)
