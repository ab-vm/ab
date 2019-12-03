#ifndef AB_LOADING_HPP_
#define AB_LOADING_HPP_

#include <span>

#include <Ab/Bytes.hpp>
#include <Ab/Context.hpp>
#include <Ab/Module.hpp>
#include <Ab/Resolver.hpp>
#include <Ab/VirtualMachine.hpp>
#include <memory>


namespace Ab {

/// Compile a storage buffer into a module.
///
/// Ownership of the storage is moved into the module.
///
std::shared_ptr<Module> compile(Context& cx, ModuleStorage&& storage);

/// Compile bytes into a module.
///
/// Ownership of the bytes is transferred to the module.
/// When the Module is destroyed, the bytes will be released via std::free.
///
inline std::shared_ptr<Module> compile(Context& cx, std::span<Byte> bytes) {
	return compile(cx, ModuleStorage(bytes));
}

/// Instantiate a compiled module.
///
/// The instantiation is owned by the VM.
/// When the VM is destroyed, the module instance will be destroyed.
/// @returns a pointer to the newly instantiated module instance
///
inline ModuleInst* instantiate(Context& cx, const std::shared_ptr<Module>& module) {
	return new ModuleInst(module);
}

inline ModuleInst* instantiate(Context& cx, std::shared_ptr<Module>&& module) {
	return new ModuleInst(std::move(module));
}

/// Instantiate a byte buffer.
///
/// The bytes are compiled to a module, which is then immediately instantiated.
/// Ownership of the bytes is transferred to the module.
/// When the Module is destroyed, the bytes will be released via std::free.
/// The Module will be destroyed when there are no more instances.
///
inline ModuleInst* instantiate(Context& cx, std::span<Byte> bytes) {
	return instantiate(cx, compile(cx, bytes));
}

ModuleInst* instantiate_file(Context& cx, const std::string& filename);

/// Load and link a module into the VM.
///
Module* load_module(Context& cx, const char* filename);

// Module* load_module(Context& cx, const std::string& filename);

Module* load_module(Context& cx, const std::string& filename);

}  // namespace Ab

#endif  // AB_LOADING_HPP_
