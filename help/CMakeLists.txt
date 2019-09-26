add_executable(ab-help
	src/ab-help-main.cpp
)

target_link_libraries(ab-help
	PRIVATE
		ab-base
		ab-util
		fmt::fmt
)

install(
	TARGETS ab-help
)
