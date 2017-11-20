include(FindPackageHandleStandardArgs)

find_program(BREATHE_EXECUTABLE
	NAMES breathe-apidoc
)

find_package_handle_standard_args(BREATHE DEFAULT_MSG
	BREATHE_EXECUTABLE
)
