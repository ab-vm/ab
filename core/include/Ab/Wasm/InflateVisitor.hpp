#ifndef AB_INFLATEVISITOR_HPP_
#define AB_INFLATEVISITOR_HPP_

namespace Ab {

template <typename T, typename R>
class ChainVisitor {};

template <typename T>
class ChainVisitor {};

/// Inflate pass will set up a va
class InflateVisitor {
	auto module_start(Module& m) -> void override {
	}

	auto module_end(Module& m) -> Module& {
		return m;
	}
};
