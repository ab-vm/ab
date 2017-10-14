#ifndef AB_MODULE_HPP_
#define AB_MODULE_HPP_

#include <Ab/Config.hpp>
#include <Om/NativeCell.hpp>
#include <Om/Ref.hpp>

namespace Ab {

class Instance;

/// Derived from the Mozilla WASM API
/// https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/WebAssembly/Module

class Module : public Om::NativeCell {
public:
	Module();

	~Module();

	auto mark(Om::MarkContext& cx) -> void;

private:
};

}  // namespace Ab

#endif  // AB_MODULE_HPP_
