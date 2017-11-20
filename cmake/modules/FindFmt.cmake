include(FindPackageHandleStandardArgs)

find_path(Fmt_INCLUDE_DIR fmt/core.h)
find_library(Fmt_LIBRARY fmt)

find_package_handle_standard_args(Fmt
	DEFAULT_MSG
	Fmt_LIBRARY
	Fmt_INCLUDE_DIR
)

if(Fmt_FOUND AND NOT TARGET Fmt::Fmt)
	add_library(Fmt::Fmt UNKNOWN IMPORTED GLOBAL)
	set_target_properties(Fmt::Fmt
		PROPERTIES
			IMPORTED_LOCATION ${Fmt_LIBRARY}
			INTERFACE_INCLUDE_DIRECTORIES ${Fmt_INCLUDE_DIR}
			INTERFACE_COMPILE_FEATURES cxx_std_11
	)
endif()
