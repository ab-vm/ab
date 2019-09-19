#ifndef AB_MODULE_HPP_
#define AB_MODULE_HPP_

#include <Ab/Config.hpp>
#include <Ab/Context.hpp>

namespace Ab {

// /// A
class Module {
	
};

// class ExportEntry {
// };

// class ExportTable {
// };

// class ExportIterator {
// 	public:

// };

/// Derived from the Mozilla WASM API
/// https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/WebAssembly/Module
/// An internal, decoded module. Fully loaded and linked.
///
/// Internally, an XModule is a handle to a region of memory-mapped data.
class XModule {
public:
	/// Compile and load a module from src
	// XModule(void* start, std::size_t nbytes)
	// 	: start_(start), nbytes_(nbytes) {}

	// initialize() {

	// }

	// ~Module() {
	// 	unmap() 
	// }

	// customSections() {

	// }

	// ExportIterator exports() {

	// }

	// imports() {

	// }

private:
	// void* start_;
	// std::size_t nbytes_;
};

}  // namespace Ab

#endif  // AB_MODULE_HPP_
