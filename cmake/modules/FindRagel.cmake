include(FindPackageHandleStandardArgs)

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
#  ab_add_ragel_source(
#    INPUT <input>
#    OUTPUT <output>
#  )
function(ab_add_ragel_source)
	cmake_parse_arguments("ARG" "" "INPUT;OUTPUT" "" "${ARGN}")
	get_filename_component(input ${ARG_INPUT} ABSOLUTE ${CMAKE_CURRENT_SOURCE_DIR})
	add_custom_command(
		COMMAND ragel ${input} -o ${ARG_OUTPUT}.dirty
		MAIN_DEPENDENCY ${ARG_INPUT}
		OUTPUT ${ARG_OUTPUT}.dirty
	)
	add_custom_command(
		COMMAND clang-format ${ARG_OUTPUT}.dirty > ${ARG_OUTPUT}
		MAIN_DEPENDENCY ${ARG_OUTPUT}.dirty
		OUTPUT ${ARG_OUTPUT}
	)
endfunction(ab_add_ragel_source)

function(ab_add_ragel_sources)
	foreach(source IN LISTS ARGN)
		string(REGEX REPLACE ".rl$" "" output ${source})
		ab_add_ragel_source(INPUT ${source} OUTPUT ${output})
	endforeach()
endfunction(ab_add_ragel_sources)
