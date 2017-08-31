#!/bin/bash

set -evx

git submodule update --init --recursive

mkdir build && cd build
cmake -C ../ci/travis/cache.cmake ..
cmake --build .
ctest -V .
