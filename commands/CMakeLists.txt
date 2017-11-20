add_library(ab-commands
	src/ab-commands-asm.cpp
	src/ab-commands-disasm.cpp
	src/ab-commands-help.cpp
	src/ab-commands-run.cpp
	src/ab-commands-version.cpp
)

target_include_directories(ab-commands
	PUBLIC
		include
)

target_link_libraries(ab-commands
	PUBLIC
		ab-core
		ab-util
		ab-wast-parser
)
