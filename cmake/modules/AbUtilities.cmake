if (_AB_UTILITIES_INC)
	return()
endif()
set(_AB_UTILITIES_INC true)

include(AbJinja)

function(make_directory dir)
	add_custom_command(
		COMMAND ${CMAKE_COMMAND} -E make_directory ${dir}
		OUTPUTS ${dir}
	)
endfunction(make_directory)

function(get_git_commit output_variable)
	exec_program("git" ${CMAKE_CURRENT_SOURCE_DIR}
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

	set(${project}_VERSION CACHE STRING "${major}.${minor}.${patch}" "Project Version" FORCE)
endfunction(project_version)

# Helper to translate a source file to an absolute path.
function(ab_resolve_source_name output_var file_name)
	get_filename_component(resolved_file_name ${ARG_INPUT} ABSOLUTE BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
	set(${output_var} ${resolved_file_name} PARENT_SCOPE)
endfunction(ab_resolve_source_name)
