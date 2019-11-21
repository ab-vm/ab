find_package(Ragel REQUIRED)

add_ragel_source(
	INPUT  src/ab-edn-lexer.cpp.rl
	OUTPUT src/ab-edn-lexer.cpp
)

add_library(ab-edn
	include/Ab/Edn/Ir.hpp
	include/Ab/Edn/Lexer.hpp
	include/Ab/Edn/Parser.hpp
	include/Ab/Edn/Printing.hpp
	include/Ab/Edn/Token.hpp
	src/ab-edn-globals.cpp
	src/ab-edn-parser.cpp
	src/ab-edn-lexer.cpp
)

target_include_directories(ab-edn
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
)

target_link_libraries(ab-edn
	PUBLIC
		ab-core
		ab-parse
)

target_compile_features(ab-edn
	PUBLIC
		cxx_std_20
)

install(
	TARGETS
		ab-edn
)

add_subdirectory(test)
