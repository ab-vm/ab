add_executable(ab-asm
	src/ab-asm-main.cpp
)

target_link_libraries(ab-asm
	PUBLIC
		ab-base
		ab-core
		ab-commands
		ab-util
		ab-wast-parser
		ab-edn
)

install(
	TARGETS
		ab-asm
)
