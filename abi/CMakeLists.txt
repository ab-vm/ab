project(abi)
project_version(abi 0 1 0)

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
