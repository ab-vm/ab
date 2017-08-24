find_package(Sphinx)

if(NOT Sphinx_FOUND)
	message(STATUS "Ab: Skipping doc directory: Sphinx not found")
	return()
endif()

add_custom_target(ab_docs ALL
	${SPHINX_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
)
