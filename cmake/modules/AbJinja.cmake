if(_AB_JINJA_INC)
	return()
endif()
set(_AB_JINJA_INC true)

include(AbUtilities)

find_package(Python3 REQUIRED COMPONENTS Interpreter)
find_package(ClangFormat)

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
			${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/jinja-generate.py
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
