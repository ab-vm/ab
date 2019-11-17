
ab_add_jinja_cxx_template(
	INPUT  include/Ab/Wasm/OpCode.hpp.jinja
	OUTPUT include/Ab/Wasm/OpCode.hpp
	DATA_FILES
		${CMAKE_SOURCE_DIR}/data/wasm_operators.yaml
)

ab_add_jinja_cxx_template(
	INPUT  include/Ab/Wasm/ExprDispatch.hpp.jinja
	OUTPUT include/Ab/Wasm/ExprDispatch.hpp
	DATA_FILES
		${CMAKE_SOURCE_DIR}/data/wasm_operators.yaml
)

ab_add_jinja_cxx_template(
	INPUT  include/Ab/Opcode.hpp.jinja
	OUTPUT include/Ab/Opcode.hpp
	DATA_FILES
		${CMAKE_SOURCE_DIR}/data/abx_operators.yaml
)

ab_add_jinja_cxx_template(
	INPUT  include/Ab/FuncBuilder.hpp.jinja
	OUTPUT include/Ab/FuncBuilder.hpp
	DATA_FILES
		${CMAKE_SOURCE_DIR}/data/abx_operators.yaml
		${CMAKE_SOURCE_DIR}/data/types.yaml
)

ab_add_jinja_cxx_template(
	INPUT  include/Ab/Interpreter.hpp.jinja
	OUTPUT include/Ab/Interpreter.hpp
	DATA_FILES
		${CMAKE_SOURCE_DIR}/data/abx_operators.yaml
		${CMAKE_SOURCE_DIR}/data/interpreter_state.yaml
	TEMPLATE_INCLUDES
		${CMAKE_SOURCE_DIR}/templates/interpreter-utilities.jinja
)

ab_add_jinja_cxx_template(
	INPUT  src/ab-core-Interpreter.cpp.jinja
	OUTPUT src/ab-core-Interpreter.cpp
	DATA_FILES
		${CMAKE_SOURCE_DIR}/data/abx_operators.yaml
		${CMAKE_SOURCE_DIR}/data/interpreter_state.yaml
	TEMPLATE_INCLUDES
		${CMAKE_SOURCE_DIR}/templates/interpreter-utilities.jinja
)

ab_add_jinja_cxx_template(
	INPUT  include/Ab/XDisasm.hpp.jinja
	OUTPUT include/Ab/XDisasm.hpp
	DATA_FILES
		${CMAKE_SOURCE_DIR}/data/abx_operators.yaml
)

add_library(ab-core
	include/Ab/Wasm/OpCode.hpp
	include/Ab/Wasm/ExprDispatch.hpp
	include/Ab/XDisasm.hpp
	include/Ab/Opcode.hpp
	include/Ab/FuncBuilder.hpp
	include/Ab/Interpreter.hpp
	src/ab-core-Interpreter.cpp
	src/ab-core-Process.cpp
	src/ab-core-Version.cpp
	src/ab-core-VirtualMachine.cpp
)

target_compile_features(ab-core
	PUBLIC
		cxx_std_17
)

target_include_directories(ab-core
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
)

target_link_libraries(ab-core
	ab-base
	ab-util
)

install(
	TARGETS
		ab-core
)

add_subdirectory(test)
