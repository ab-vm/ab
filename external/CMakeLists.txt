# This incantation will protect from madness
add_compile_options(
	-Wno-integer-cast
	-Wno-dangling-else
	-Wlogical-op-parentheses
)

add_subdirectory(googletest)
add_subdirectory(omr)
