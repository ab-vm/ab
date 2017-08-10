cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

list(APPEND CMAKE_MODULE_PATH
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules
	${CMAKE_CURRENT_SOURCE_DIR}/external/omr/cmake/modules
)

project(ab-vm)

enable_testing()

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

include(cmake/Utilities.cmake)

exec_program(
	"git"
	${CMAKE_CURRENT_SOURCE_DIR}
	ARGS "describe" "--all" "--long" "HEAD"
	OUTPUT_VARIABLE GIT_COMMIT
)

add_subdirectory(ab)
add_subdirectory(external)
add_subdirectory(om)
add_subdirectory(pith)
