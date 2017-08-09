#ifndef AB_MODULE_HPP_
#define AB_MODULE_HPP_

#include <Om/Ref.hpp>

namespace Ab {

class Instance;

/// Derived from the Mozilla WASM API
/// https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/WebAssembly/Module

class Module {
public:
	Module();
	~Module();
	auto instantiate() -> Om::Ref<Instance>;

private:

};

} // namespace Ab

#endif // AB_MODULE_HPP_