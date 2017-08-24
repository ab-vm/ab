=======================
Coding Style Guidelines
=======================

.. highlight:: C++

Choice of language
==================

Ab is written in C++11 with minimal standard library usage. In particular, avoid standard library features introduced
in C++11. Tooling and scripts should be written in python3.

Formatting code
===============

Ab uses clang-format. Always run clang-format before committing files. When the clang-format configuration is updated,
it is OK to commit style changes as a part of a commit.

Priorities
==========

API first.
Legibility second.
Performance third.

Put public definitions first.

Fields, Getters, and Setters
============================

private/protected fields are named with trailing underscores. public fields don't have underscores.

Name the accessors after the field (sans trailing underscore).
Setters should take a single value, and return ``*this``, implementing a fluent interface.

Example::

	class Array {
	public:
		inline constexpr auto size() const -> std::size_t {
			return size_;
		}

		inline auto size(std::size_t x) -> Array& {
			size_ = x;
			return *this;
		}

	private:
		std::size_t size_;
	};

Constants and Enumermations
===========================

Constants are written as `MY_CONSTANT`. Constants should be namespaced. 
Use `static const constexpr` constants, *Do not use macros*.
Prefer `enum class` over plain `enum`s.

Macros and Preprocessor Definitions
===================================

Do not add definitions to the command line. Place compilation flags in the config header.

Avoid placing constants

Includes and Headers
====================

Include files relative to
``PACKAGE_MACRO()`` Are always written as uppercase.

Example::

	#include <Pith/Config.hpp> // Configuration first
	#include <Pith/*.hpp>      // Internal headers, alphabetical
	#include <external.hpp>    // External headers, alphabetical

Const by default
================

When in doubt, use const.

Namespaces, Includes, and Project Layout
========================================

The Ab project consists of a few seperate projects.

Each project has the following layout:

- project/ - top level directory
  - src/ - source code
  - include/

* One include directory per project
* One source per project
* One library per project

Place public headers in ``$PROJECT/include/``. When in doubt, APIs should be written as public.

All code should be under the project namespace (``Ab``/``Pith``/``Om``).

Structs vs Classes vs Namespaces

Namespaces are used when
File structure should mimic namespaces.

Use struct for simple POD-style objects. Use class when the object is complex.
Prefer not to use nested classes or 

## Standard Library Usage

Minimize usage of the standard library.