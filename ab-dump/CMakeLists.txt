
ab_setup()


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_EXTENSIONS off)

add_executable(ab-dump
	src/ab-dump.cpp
)

target_link_libraries(ab-dump
	PUBLIC
		ab
)
