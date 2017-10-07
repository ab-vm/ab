find_program(CLANG_TIDY_EXECUTABLE NAMES clang-tidy)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(CLANG_TIDY DEFAULT_MSG
	CLANG_TIDY_EXECUTABLE
)
