#!/bin/bash

set -evx

if "x$TRAVIS_OS_NAME" = "xosx"; then
	brew install ninja
fi

git submodule update --init --recursive

mkdir build && cd build
cmake -G Ninja -C ../ci/travis/cache.cmake ..
cmake --build .
ctest -V .

