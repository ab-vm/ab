#ifndef AB_LOADING_HPP_
#define AB_LOADING_HPP_

#include <Ab/Module.hpp>
#include <Ab/Resolver.hpp>
#include <Ab/Context.hpp>
#include <Ab/VirtualMachine.hpp>

namespace Ab {

/// Compile a storage buffer into a module.
///
/// Ownership of the storage is moved into the module.
///
inline Module* compile(Context& cx, ModuleStorage&& storage) {
	return new Module(std::move(storage));
}

/// Compile bytes into a module.
///
/// Ownership of the bytes is transferred to the module.
/// When the Module is destroyed, the bytes will be released via std::free.
///
inline Module* compile(Context& cx, Byte* bytes) {
	return compile(cx, ModuleStorage(bytes));
}

inline ModuleInst* instantiate(Context& cx, std::shared_ptr<Module> module) {
	return new ModuleInst(module);
}

inline ModuleInst* instantiate(Context& cx, std::shared_ptr<Module>&& module) {
	return new ModuleInst(std::move(module));
}

ModuleInst* instantiate_file(Context& cx, const std::string& filename);

/// Load and link a module into the VM.
///
Module* load_module(Context& cx, const char* filename);

// Module* load_module(Context& cx, const std::string& filename);

Module* load_module(Context& cx, const std::string& filename);

} // namespace Ab

#endif // AB_LOADING_HPP_
