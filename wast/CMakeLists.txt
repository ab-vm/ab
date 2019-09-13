ab_add_jinja_template(
	INPUT  src/ab-wast-FunctionParser.cpp.rl.jinja
	OUTPUT src/ab-wast-FunctionParser.cpp.rl
	DATA_FILES
		${CMAKE_SOURCE_DIR}/data/wasm_operators.yaml
)

add_ragel_sources(
	src/ab-wast-ModuleParser.cpp.rl
	src/ab-wast-Lexer.cpp.rl
	${CMAKE_CURRENT_BINARY_DIR}/src/ab-wast-FunctionParser.cpp.rl
)

add_library(ab-wast-parser
	include/Ab/Wast/Alph.hpp
	include/Ab/Wast/FunctionParser.hpp
	include/Ab/Wast/Lexer.hpp
	include/Ab/Wast/Location.hpp
	include/Ab/Wast/Parse.hpp
	include/Ab/Wast/Parser.hpp
	src/ab-wast-ModuleParser.cpp
	src/ab-wast-FunctionParser.cpp
	src/ab-wast-Lexer.cpp
)

target_link_libraries(ab-wast-parser
	PUBLIC
		ab-base
		ab-core
		ab-util
)

target_include_directories(ab-wast-parser
	PUBLIC
		include
		${CMAKE_CURRENT_BINARY_DIR}/include
)
