#ifndef AB_MODULE_HPP_
#define AB_MODULE_HPP_

#include <Ab/Config.hpp>
#include <Ab/Context.hpp>
#include <Om/NativeCell.hpp>
#include <Om/Ref.hpp>

namespace Ab {

class Instance;

namespace Wasm {

/// Derived from the Mozilla WASM API
/// https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/WebAssembly/Module
class Module {
public:
	/// Compile and load a module from src
	Module();

	~Module();

	// auto customSections() const -> ???;

	// auto exports() const -> ???;

	// auto imports() const -> ???;

private:
};

}  // namespace Wasm
}  // namespace Ab

#endif  // AB_MODULE_HPP_
