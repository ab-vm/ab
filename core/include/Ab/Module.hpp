#ifndef AB_Module_HPP_
#define AB_Module_HPP_

#include <Ab/Config.hpp>
#include <Ab/Address.hpp>
#include <Ab/Assert.hpp>
#include <Ab/Context.hpp>
#include <Ab/Func.hpp>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace Ab {

struct ExportEntry {
	std::string name;
	std::size_t index;
};

using ExportTable = std::vector<ExportEntry>;

using FuncInstTable = std::vector<FuncInst*>;

/// A manager for the raw bytes that make up a module's data.
/// This is a special purpose datatype, used to pass ownership of bytes from
/// user code into the compiler. Because of this, this class is move-only.
///
/// This class allows users to specify a deleter function, in the case where
/// the backing data must be released through special means.
///
class ModuleStorage {
public:
	using Deleter = void (*)(void*);

	explicit ModuleStorage(Byte* address) noexcept : ModuleStorage(address, &std::free) {}

	ModuleStorage(Byte* address, Deleter deleter) noexcept
		: address_(address), deleter_(deleter) {}

	ModuleStorage(const ModuleStorage&) = delete;

	ModuleStorage(ModuleStorage&& other) noexcept {
		address_ = other.address_;
		deleter_ = other.deleter_;

		other.address_ = nullptr;
		other.deleter_ = nullptr;
	}

	~ModuleStorage() {
		if (address_) {
			deleter_(address_);
		}
	}

	ModuleStorage& operator=(const ModuleStorage&) = delete;

	ModuleStorage& operator=(ModuleStorage&&) = delete;

	Byte* address() const noexcept { return address_; }

	Deleter deleter() const noexcept { return deleter_; }

private:
	Byte* address_;
	Deleter deleter_;
};

/// A stateless representation of bytes.
///
class Module {
public:
	Module(ModuleStorage&& storage) : storage_(std::move(storage)) {}

	Module(const Module&) = delete;

	Module(Module&&) noexcept = default;

	~Module() noexcept = default;

	Byte* bytes() const noexcept { return storage_.address(); }

	ModuleStorage& storage() noexcept { return storage_; }

	const ModuleStorage& storage() const noexcept { return storage_; }

private:
	ModuleStorage storage_;
};

/// Module Instance.
/// An instantiated module, the runtime-side of a module.
///
class ModuleInst {
public:
	ModuleInst(const std::shared_ptr<Module>& module) : module_(module) {}

	ModuleInst(std::shared_ptr<Module>&& module) : module_(std::move(module)) {}

	/// Obtain the underlying, stateless representation of the module.
	/// Note that the module may be shared by multiple instantiations.
	///
	const std::shared_ptr<Module>& module() const noexcept { return module_; }

	/// Obtain the underlying bytes that represent the module.
	///
	const Byte* bytes() const noexcept { return module_->bytes(); }

	std::vector<FuncInst>& func_table() noexcept { return func_table_; }

	const std::vector<FuncInst>& func_table() const noexcept { return func_table_; }

	/// Helper to grab a function by index.
	///
	FuncInst* func(std::size_t index) noexcept {
		AB_ASSERT(index <= func_table_.size());
		return &func_table_[index];
	}

	/// Helper to grab a function by index.
	///
	const FuncInst* func(std::size_t index) const noexcept {
		AB_ASSERT(index <= func_table_.size());
		return &func_table_[index];
	}

#if 0
	/// Find an function by name.
	///
	Func* find_function(std::string_view name) noexcept {
		for (const auto& entry : export_table_) {
			if (entry.name == name) {
				return function_for(entry);
			}
		}
		return nullptr;
	}

	const Func* find_function(std::string_view name) const noexcept {
		for (const auto& entry : export_table_) {
			if (entry.name == name) {
				return function_for(entry);
			}
		}
		return nullptr;
	}

	/// Get the function corresponding to an entry in the module's export table.
	///
	Func* function_for(const ExportEntry& entry) noexcept {
		return func_table_.at(entry.index);
	}

	const Func* function_for(const ExportEntry& entry) const noexcept {
		return func_table_.at(entry.index);
	}
#endif  // 0

	const std::shared_ptr<Module> module_;
	std::vector<FuncInst> func_table_;
};

}  // namespace Ab

#endif  // AB_Module_HPP_
