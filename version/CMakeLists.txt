add_executable(ab-version
	src/ab-version-main.cpp
)

target_link_libraries(ab-version
	PRIVATE
		ab-core
)

install(
	TARGETS
		ab-version
)
