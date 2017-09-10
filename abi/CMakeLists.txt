project(abi LANGUAGES C CXX VERSION 0.0.1)

ab_setup()

add_executable(abi
	src/abi.cpp
)

target_include_directories(abi
	PUBLIC
		include/
)

target_link_libraries(abi
	PUBLIC
		ab
)
