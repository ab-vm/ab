add_executable(ab
	src/ab-main.cpp
)

target_compile_features(ab
	PUBLIC
		cxx_std_20
)

target_link_libraries(ab
	PUBLIC
		ab-base
		ab-core
		ab-util
		fmt::fmt
)

install(
	TARGETS
		ab
)
