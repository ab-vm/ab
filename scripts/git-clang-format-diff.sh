#!/bin/bash

git diff -U0 --no-color HEAD^ | clang-format-diff.py -i -p1
