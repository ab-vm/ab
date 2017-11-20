ab-test-main
============

ab-test-main defines a main function, which ab tests can use, so they don't
have to define their own. The main function initializes the Ab::Test::env
object before starting up gtest and running the tests.

ab-test-main works by injecting an interface source into test executables.

