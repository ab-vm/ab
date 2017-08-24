
find_program(SPHINX_EXECUTABLE
	NAMES sphinx-build
	PATH_SUFFIXES bin
	DOC "http://www.sphinx-doc.org/en/stable/"
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(Sphinx DEFAULT_MSG
	SPHINX_EXECUTABLE
)
