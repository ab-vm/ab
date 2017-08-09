#!/bin/bash

set -evx
export JOBS=4

mkdir build
cd build
cmake ..
cmake --build .
ctest -V
