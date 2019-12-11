# This incantation will protect from madness
if(CMAKE_COMPILER_ID STREQUAL "AppleClang" OR
   CMAKE_COMPILER_ID STREQUAL "Clang")
	add_compile_options(
		-Wno-integer-cast
		-Wno-dangling-else
		-Wno-logical-op-parentheses
	)
elseif()
endif()

add_subdirectory(abseil-cpp)
add_subdirectory(fmt)
add_subdirectory(googletest EXCLUDE_FROM_ALL)

if(AB_USE_OMR)
	add_subdirectory(omr)
endif()
