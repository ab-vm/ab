include(FindPackageHandleStandardArgs)

find_program(ClangFormat_EXECUTABLE clang-format)

find_package_handle_standard_args(ClangFormat
	DEFAULT_MSG
	ClangFormat_EXECUTABLE
)

if(NOT ClangFormat_FOUND)
	return()
endif()

