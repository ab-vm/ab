==============
Building Ab VM
==============

.. highlight:: bash

Requirements
============

- CMake version 3.2 or greater
- Git


The Basics
==========

::

	# Clone the project
	git clone https://github.com/ab-vm/ab
	
	# Create a build directory
	mkdir ab/build
	cd ab/build

	# Run cmake and configure the build
	cmake ..

	# build it!
	cmake --build .

	# Test it!
	ctest .
