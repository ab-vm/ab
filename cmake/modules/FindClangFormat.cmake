include(FindPackageHandleStandardArgs)

find_program(ClangFormat_EXECUTABLE clang-format)

find_package_handle_standard_args(ClangFormat
	DEFAULT_MSG
	ClangFormat_EXECUTABLE
)

function(clang_format)
	if (NOT ClangFormat_FOUND)
		message(FATAL_ERROR "Cannot call clang-format")
	endif()

	cmake_parse_arguments("ARG" "" "INPUT;OUTPUT" "OPTIONS" "${ARGN}")

	if(NOT ARG_INPUT)
		message(SEND_ERROR "clang_format needs INPUT <file> parameter")
	endif()

	if(NOT ARG_OUTPUT)
		message(SEND_ERROR "clang_format needs OUTPUT <file> parameter")
	endif()

	if(ARG_UNPARSED_ARGUMENTS)
		message(SEND_ERROR "unrecognized parameter sent to clang_format")
	endif()

	get_filename_component(ARG_INPUT ${ARG_INPUT} ABSOLUTE BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
	get_filename_component(ARG_OUTPUT ${ARG_OUTPUT} ABSOLUTE BASE_DIR ${CMAKE_CURRENT_BINARY_DIR})

	add_custom_command(
		COMMAND ${ClangFormat_EXECUTABLE} ${ARG_OPTIONS} ${ARG_INPUT} > ${ARG_OUTPUT}
		MAIN_DEPENDENCY ${ARG_INPUT}
		OUTPUT ${ARG_OUTPUT}
	)
endfunction(clang_format)

function(copy_file)
	cmake_parse_arguments("ARG" "" "INPUT;OUTPUT" "" "${ARGN}")

	if(NOT ARG_INPUT)
		message(SEND_ERROR "copy_file needs INPUT <file> parameter")
	endif()

	if(NOT ARG_OUTPUT)
		message(SEND_ERROR "copy_file needs OUTPUT <file> parameter")
	endif()

	if(ARG_UNPARSED_ARGUMENTS)
		message(SEND_ERROR "unrecognized parameter sent to clang_format")
	endif()


	get_filename_component(ARG_INPUT ${ARG_INPUT} ABSOLUTE BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
	get_filename_component(ARG_OUTPUT ${ARG_OUTPUT} ABSOLUTE BASE_DIR ${CMAKE_CURRENT_BINARY_DIR})

	add_custom_command(
		COMMAND ${CMAKE_COMMAND} -E copy ${ARG_INPUT} ${ARG_OUTPUT}
		MAIN_DEPENDENCY ${ARG_INPUT}
		OUTPUT ${ARG_OUTPUT}
	)
endfunction(copy_file)

# Format the input file and place in output. If clang-format is not available, 
# simply copy the file without formatting.
# Usage:
#   clang_formatx(
#     INPUT   <input>
#     OUTPUT  <output>
#     OPTIONS <option>...
#   )
#
function(clang_formatx)
	cmake_parse_arguments("ARG" "" "INPUT;OUTPUT" "OPTIONS" "${ARGN}")

	if(ARG_UNPARSED_ARGUMENTS)
		message(SEND_ERROR "unrecognized parameter sent to clang_formatx")
	endif()

	if (ClangFormat_FOUND)
		clang_format(
			INPUT   ${ARG_INPUT}
			OUTPUT  ${ARG_OUTPUT}
			OPTIONS ${ARG_OPTIONS}
		)
	else()
		copy_file(
			INPUT   ${ARG_INPUT}
			OUTPUT  ${ARG_OUTPUT}
		)
	endif()
endfunction(clang_formatx)
