add_executable(ab
	src/ab-main.cpp
)

target_compile_features(ab
	PUBLIC
		cxx_std_17
)

target_link_libraries(ab
	PUBLIC
		ab-base
		ab-util
		ab-commands
		fmt::fmt
)
