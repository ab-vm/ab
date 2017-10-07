==============================
Ab: A WASM interpreter library
==============================

.. highlight:: C++

Ab is a C++ library implementing a wasm execution engine.

Requirements
============

* Pith
* Om & OMR

Usage
=====

.. code-block:: C++

	int main(int argc, char ** argv) {
		Ab::Process::init();
		Ab::System system;
		Ab::Process::kill();
		return 0;
	}
