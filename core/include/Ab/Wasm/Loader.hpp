#ifndef AB_WASM_LOADER_HPP_
#define AB_WASM_LOADER_HPP_

#include <Ab/Config.hpp>
#include <Ab/Context.hpp>
#include <Ab/Module.hpp>

#include <Ab/Wasm/OpCode.hpp>

#include <memory>
#include <unordered_map>
#include <vector>
#include <cstdint>

namespace Ab::Wasm {

enum class LoaderError { UNKNOWN_ERROR };

// class VerificationError {

// };

// /// Module verifier
// class Verifier {

// };

// void writeUint32(std::uint8_t* addr, std::uint32_t offset) {
// 	auto ptr = reinterpret_cast<std::uint32_t*>(addr);
// 	*ptr = offset;
// }

// void writeInt32(std::uint8_t* addr, std::int32_t offset) {
// 	auto ptr = reinterpret_cast<std::int32_t*>(addr);
// 	*ptr = offset;
// }

// inline bool compileFunctionBody(std::uint8_t* body, std::size_t size) {

// 	std::vector<std::int32_t> blkstack;

// 	for(std::int32_t offset = 0; offset < size; ++offset) {

// 		std::uint8_t* addr = body + offset;
// 		auto op = *reinterpret_cast<Wasm::OpCode*>(addr);

// 		// fmt::print("compiling: {}\n", to_string(op));

// 		switch (op) {
// 		case Wasm::OpCode::BLOCK:
// 			blkstack.push_back(offset);
// 			offset += 1;
// 			break;
// 		case Wasm::OpCode::LOOP:
// 			blkstack.push_back(offset);
// 			offset += 1;
// 			break;
// 		case Wasm::OpCode::ELSE:
// 			blkstack.pop_back();
// 			offset += 1;
// 			break;
// 		case Wasm::OpCode::END:
// 			blkstack.pop_back();
// 			offset += 1;
// 			break;
// 		case Wasm::OpCode::BR:
// 			writeInt32(addr + 1, blkstack.back() - offset);
// 			offset += 5;
// 			break;
// 		case Wasm::OpCode::BR_IF:
// 			break;
// 		default:

// 		}
// 	}
// }

/// Simple translation from wasm operators to internal operators.
/// The following translations are performed by the compiler:
/// - map  translator (wasm-module -> x-module)
// class Compiler {
// 	auto operator()(Context& cx, void* start, std::size_t nbytes);

// private:

// 	std::uint32_t blkoffset() const { return blkstack }

// 	void pushblk(std::uint32_t offset) const { blkstack.push_back(offset_); }

// 	void popblk() const { blkstack_.pop(); }

// 	/// convert a label to an offset by rewriting the byte stream.



// 	compileBlock

// 	bool compileExpression(std::uint8_t* expr) {
// 	auto op = static_cast<Wasm::OpCode>(*expr);
// 	switch(op)
// 	}

// private:
// 	std::uint32_t offset_;

// };

/// link-translation
// class Linker {
// };

// enum class RewriteError {

// };

// class InstructionIterator {}


// using ModuleTable = std::unordered_map<std::string, std::shared_ptr<Module>>;

// class BasicLoader {};

// class Cache {

// };

// class CacheLoader {

// };

// template <typename T>
// class WasmReader {};

class Loader {
public:
	Loader() {}

	~Loader() {
		// for (auto& module : modules_) {
		// 	unload(module);
		// }
	}

	// auto ldpath() { return ldpath_; }

	// const auto ldpath() const { return ldpath_; }

	// std::string resolve(std::string name) const {
	// 	for (const auto& path : ldpath_) {
	// 		if module_exists()
	// 	}
	// }

	// auto load(const std::string& name) -> std::shared_ptr<Module> {
	// 	auto rname = resolve(name);
	// 	Linker linker(modules_);

	// 	Span<Byte> mapped_mod = map(rname);

	// 	linker.link(mapped_mod);
		
	// }

	/// Load a module from a mmap'd input.
	// auto operator()(Context& cx, void* start, std::size_t nbytes) ->std::shared_ptr<Module>;

private:
	std::vector<std::string> ldpath_;
	// ModuleTable modules_;
};

}  // namespace Ab::Wasm

#endif  // AB_WASM_LOADER_HPP_
