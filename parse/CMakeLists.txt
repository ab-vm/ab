add_library(ab-parse INTERFACE)

target_compile_features(ab-parse
	INTERFACE
		cxx_std_17
)

target_include_directories(ab-parse
	INTERFACE
		include/
)

target_link_libraries(ab-parse
	INTERFACE
		ab-core
		ab-util
)

add_subdirectory(test)
