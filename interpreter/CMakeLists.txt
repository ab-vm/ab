add_library(ab-interpreter
	include/Ab/InterpreterBuilder.hpp
	src/ab-interpreter-main.cpp
)

target_compile_features(ab-interpreter
	PUBLIC
		cxx_std_17
)

target_include_directories(ab-interpreter
	PUBLIC
		include/
	
)

target_link_libraries(ab-interpreter
	PUBLIC
		ab-core
)
