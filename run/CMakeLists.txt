add_executable(ab-run
	src/ab-run-main.cpp
)

target_link_libraries(ab-run
	PRIVATE
		ab-core
		ab-util
		ab-interpreter
)

install(
	TARGETS
		ab-run
)
