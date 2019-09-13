if (_AB_UTILITIES_INC)
	return()
endif()
set(_AB_UTILITIES_INC true)

find_package(ClangFormat)
find_package(Ragel REQUIRED)

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
			CXX_STANDARD 17
	)
endfunction(add_ab_test)

# Helper to translate a source file to an absolute path.
function(ab_resolve_source_name output_var file_name)
	get_filename_component(resolved_file_name ${ARG_INPUT} ABSOLUTE BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
	set(${output_var} ${resolved_file_name} PARENT_SCOPE)
endfunction(ab_resolve_source_name)

# Add a jinja-generated source file
# Usage:
#  ab_add_generated_source(
#    INPUT <input>
#    OUTPUT <output>
#    DATA_FILES <data-file>...
#    TEMPLATE_INCLUDES <jinja-file>...
#  )
function(ab_add_jinja_template)
	cmake_parse_arguments("ARG" "" "INPUT;OUTPUT" "DATA_FILES;TEMPLATE_INCLUDES" "${ARGN}")

	get_filename_component(ARG_INPUT  ${ARG_INPUT}  ABSOLUTE BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
	get_filename_component(ARG_OUTPUT ${ARG_OUTPUT} ABSOLUTE BASE_DIR ${CMAKE_CURRENT_BINARY_DIR})

	add_custom_command(
		COMMAND
			python3 ${CMAKE_SOURCE_DIR}/scripts/jinja-generate.py
				--data-dir=${CMAKE_SOURCE_DIR}/data
				--include-dir=${CMAKE_SOURCE_DIR}/templates
				${ARG_INPUT}
				${ARG_OUTPUT}
		MAIN_DEPENDENCY ${ARG_INPUT}
		OUTPUT          ${ARG_OUTPUT}
		DEPENDS
			${CMAKE_SOURCE_DIR}/scripts/jinja-generate.py
			${ARG_DATA_FILES}
			${ARG_TEMPLATE_INCLUDES}
	)
endfunction(ab_add_jinja_template)

# Add a jinja generated source file, and reformat it with clang-format.
# Usage:
#  ab_add_jinja_cxx_template(
#    INPUT <input>
#    OUTPUT <output>
#    DATA_FILES <data-file>...
#    TEMPLATE_INCLUDES <jinja-file>...
#  )
function(ab_add_jinja_cxx_template)
	cmake_parse_arguments("ARG" "" "INPUT;OUTPUT" "DATA_FILES;TEMPLATE_INCLUDES" "${ARGN}")

	get_filename_component(ARG_INPUT  ${ARG_INPUT}  ABSOLUTE BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
	get_filename_component(ARG_OUTPUT ${ARG_OUTPUT} ABSOLUTE BASE_DIR ${CMAKE_CURRENT_BINARY_DIR})

	ab_add_jinja_template(
		INPUT      ${ARG_INPUT}
		OUTPUT     ${ARG_OUTPUT}.dirty
		DATA_FILES ${ARG_DATA_FILES}
	)

	clang_formatx(
		INPUT  ${ARG_OUTPUT}.dirty
		OUTPUT ${ARG_OUTPUT}
	)
endfunction(ab_add_jinja_cxx_template)

# Add a collection of jinja generated c++ source files.
# Usage:
#  ab_add_jinja_cxx_group(
#    <name>
#    SOURCES <cxx-jinja-file>...
#    DATA_FILES <yaml-file>...
#    JINJA_INCLUDE_DIRS <dir>...
#  )
function(ab_add_jinja_cxx_library)
	cmake_parse_arguments("ARG" "" "" "SOURCES;DATA_FILES;TEMPLATE_INCLUDES" "${ARGN}")
	foreach(source ${ARG_SOURCES})
		ab_add_jinja_cxx_template(
			INPUT             ${source}
			OUTPUT            ${source}
			DATA_FILES        ${ARG_DATA_FILES}
			TEMPLATE_INCLUDES ${ARG_TEMPLATE_INCLUDES}
		)
	endforeach()
endfunction(ab_add_jinja_cxx_library)
