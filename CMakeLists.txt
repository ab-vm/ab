cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

set(AB_NAME ab)
set(AB_VERSION_MAJOR 0)
set(AB_VERSION_MINOR 0)
set(AB_VERSION_PATCH 1)
set(AB_VERSION ${AB_VERSION_MAJOR}.${AB_VERSION_MINOR}.${AB_VERSION_PATCH})

project(ab
  VERSION ${AB_VERSION}
  LANGUAGES CXX
)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

enable_testing()

configure_file(
  "${PROJECT_SOURCE_DIR}/include/ab/version.hpp.in"
  "${PROJECT_BINARY_DIR}/include/ab/version.hpp"
)

add_library(ab
  lib/ab/ab.cpp
  lib/ab/process.cpp
  lib/ab/version.cpp
)

target_link_libraries(ab
  ${CONAN_LIBS_OMR}
)

target_compile_features(ab
  PUBLIC
    cxx_auto_type
    cxx_variadic_templates
)

target_include_directories(ab
  PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_BINARY_DIR}/include"
)

add_executable(abc
  src/abc.cpp
)

target_link_libraries(abc
  ab
)

add_executable(abt
  test/abt.cpp
  test/jitbuildertest.cpp
)

target_link_libraries(abt
  ab
  ${CONAN_LIBS_GTEST}
)

add_test(
  NAME abt
  COMMAND abt
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
