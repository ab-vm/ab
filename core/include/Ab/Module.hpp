#ifndef AB_MODULE_HPP_
#define AB_MODULE_HPP_

#include <Ab/Config.hpp>

#include <Ab/Address.hpp>
#include <Ab/Assert.hpp>
#include <Ab/Bytes.hpp>
#include <Ab/Func.hpp>
#include <Ab/VectorUtilities.hpp>

#include <cstddef>
#include <span>
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

	explicit ModuleStorage(std::span<Byte> bytes) noexcept : ModuleStorage(bytes, &std::free) {}

	ModuleStorage(std::span<Byte> bytes, Deleter deleter) noexcept
		: bytes_(bytes), deleter_(deleter) {}

	ModuleStorage(const ModuleStorage&) = delete;

	ModuleStorage(ModuleStorage&& other) noexcept {
		bytes_   = other.bytes_;
		deleter_ = other.deleter_;

		other.bytes_   = std::span<Byte>();
		other.deleter_ = nullptr;
	}

	~ModuleStorage() noexcept {
		if (address() && deleter_) {
			deleter_(address());
		}
	}

	ModuleStorage& operator=(const ModuleStorage&) = delete;

	ModuleStorage& operator=(ModuleStorage&&) = delete;

	Byte* address() const noexcept { return bytes_.data(); }

	Deleter deleter() const noexcept { return deleter_; }

	std::span<Byte> bytes() const noexcept { return bytes_; }

private:
	std::span<Byte> bytes_;
	Deleter deleter_;
};

using FuncTable = std::vector<Func>;

/// A stateless representation of bytes.
///
class Module {
public:
	Module(ModuleStorage&& storage) : storage_(std::move(storage)) {}

	Module(const Module&) = delete;

	Module(Module&&) noexcept = default;

	~Module() noexcept = default;

	std::span<Byte> bytes() const noexcept { return storage_.bytes(); }

	ModuleStorage& storage() noexcept { return storage_; }

	const ModuleStorage& storage() const noexcept { return storage_; }

	FuncTable& func_table() noexcept { return func_table_; }

	const FuncTable& func_table() const noexcept { return func_table_; }

	std::vector<FuncType>& type_table() noexcept { return type_table_; }

	const std::vector<FuncType>& type_table() const noexcept { return type_table_; }

	std::vector<std::uint32_t>& func_types() noexcept { return func_types_; }

	const std::vector<std::uint32_t>& func_types() const noexcept { return func_types_; }

	/// Get the type for the nth function in the module.
	///
	const FuncType& type_for(std::uint32_t funcidx) const noexcept {
		std::uint32_t typeidx = func_types_.at(funcidx);
		return type_table_.at(typeidx);
	}

private:
	ModuleStorage storage_;
	FuncTable func_table_;
	std::vector<FuncType> type_table_;
	std::vector<std::uint32_t> func_types_;
};

/// Module Instance.
/// An instantiated module, the runtime-side of a module.
///
class ModuleInst {
public:
	ModuleInst(const std::shared_ptr<Module>& module) : module_(module) { initialize(); }

	ModuleInst(std::shared_ptr<Module>&& module) : module_(std::move(module)) { initialize(); }

	/// Obtain the underlying, stateless representation of the module.
	/// Note that the module may be shared by multiple instantiations.
	///
	const std::shared_ptr<Module>& shared_module() const noexcept { return module_; }

	/// Obtain the underlying bytes that represent the module.
	///
	std::span<Byte> bytes() const noexcept { return module_->bytes(); }

	std::vector<FuncInst>& func_inst_table() noexcept { return func_inst_table_; }

	const std::vector<FuncInst>& func_inst_table() const noexcept { return func_inst_table_; }

	/// Helper to grab a function instance by index.
	///
	FuncInst* func_inst(std::size_t index) noexcept {
		AB_ASSERT(index < func_inst_table_.size());
		return &func_inst_table_[index];
	}

	/// Helper to grab a function instance by index.
	///
	const FuncInst* func_inst(std::size_t index) const noexcept {
		AB_ASSERT(index < func_inst_table_.size());
		return &func_inst_table_[index];
	}

#if 0  /////////////////////////////////////////////////////////////////////////

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

#endif  ////////////////////////////////////////////////////////////////////////

	const std::shared_ptr<Module> module_;
	std::vector<FuncInst> func_inst_table_;

private:
	void initialize() {
		func_inst_table_.reserve(module_->func_table().size());
		for (auto& func : module_->func_table()) {
			func_inst_table_.emplace_back(&func);
		}
	}
};

}  // namespace Ab

#endif  // AB_MODULE_HPP_
