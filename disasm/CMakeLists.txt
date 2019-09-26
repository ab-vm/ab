add_executable(ab-disasm
	src/ab-disasm-main.cpp
)

target_link_libraries(ab-disasm
	PUBLIC
		ab-base
		ab-core
		ab-commands
		ab-util
)

install(
	TARGETS
		ab-disasm
)
