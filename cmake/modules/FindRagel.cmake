include(FindPackageHandleStandardArgs)
include(AbUtilities)

find_package(ClangFormat)

find_program(Ragel_EXECUTABLE ragel)

find_package_handle_standard_args(Ragel DEFAULT_MSG
	Ragel_EXECUTABLE
)

if(NOT Ragel_FOUND)
	return()
endif()

# Add a ragel-generated source file. Run clang-format on output.
# Usage:
#  add_ragel_source(
#    INPUT  <input>
#    OUTPUT <output>
#  )
function(add_ragel_source)
	cmake_parse_arguments("ARG" "" "INPUT;OUTPUT" "" "${ARGN}")

	get_filename_component(ARG_INPUT  ${ARG_INPUT}  ABSOLUTE BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
	get_filename_component(ARG_OUTPUT ${ARG_OUTPUT} ABSOLUTE BASE_DIR ${CMAKE_CURRENT_BINARY_DIR})
	get_filename_component(DIR        ${ARG_OUTPUT} DIRECTORY)

	add_custom_command(
		COMMAND cmake -E make_directory ${DIR}
		COMMAND ragel ${ARG_INPUT} -o ${ARG_OUTPUT}.dirty
		MAIN_DEPENDENCY ${ARG_INPUT}
		OUTPUT ${ARG_OUTPUT}.dirty
	)
	clang_formatx(
		INPUT ${ARG_OUTPUT}.dirty
		OUTPUT ${ARG_OUTPUT}
	)
endfunction(add_ragel_source)

function(add_ragel_sources)
	foreach(source IN LISTS ARGN)
		string(REGEX REPLACE ".rl$" "" output ${source})
		add_ragel_source(INPUT ${source} OUTPUT ${output})
	endforeach()
endfunction(add_ragel_sources)
