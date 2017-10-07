# linux x86-32 build configuration on travis 

set(CMAKE_SYSTEM_PROCESSOR x86)
set(CMAKE_SYSTEM_NAME linux)
set(CMAKE_CXX_COMPILER clang)
set(CMAKE_CXX_FLAGS "-Wall -m32")
set(CMAKE_C_FLAGS "-Wall -m32")
set(CMAKE_EXE_LINKER_FLAGS "-m32")
