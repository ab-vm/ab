#ifndef AB_MEMORY_HPP_
#define AB_MEMORY_HPP_

#include <Pith/Address.hpp>
#include <Pith/Assert.hpp>
#include <Pith/Bytes.hpp>
#include <Pith/Page.hpp>
#include <Pith/Result.hpp>
#include <Pith/Span.hpp>
#include <cstdint>

namespace Ab {

struct MemoryConfig {
	Pith::Address address{nullptr};
	std::size_t minPageCount{1};
	std::size_t maxPageCount{4};

	inline auto verify() const -> void;
};

class MemoryError : public std::runtime_error {
	using std::runtime_error::runtime_error;
};

/// The WASM contiguous raw memory subsystem.
/// web assembly gives programs low level access to a contiguous region of memory.
/// The Memory class manages that giant blob of memory. Per the spec, Memory can be grown, but does
/// not shrink. Each call
class Memory {
public:
	/// The size of a memory page.
	static auto pageSize() -> std::size_t;

	/// Bring up the memory subsystem with the default config.
	Memory();

	/// Bring up the memory subsytem.
	Memory(const MemoryConfig& config);

	~Memory();

	/// The address.
	inline auto addresss() const -> Pith::Address;

	/// The size of the allocated memory.
	inline auto size() const -> std::size_t;

	/// Grow the memory by n pages.
	auto grow(std::size_t n = 1) -> void;

	/// Shrink the memory by n pages.
	auto shrink(std::size_t n = 1) -> void;

private:
	auto reserve(const Pith::Address address, std::size_t n) -> Pith::Address;

	auto activate(const Pith::Address address, const std::size_t n) -> void;

	auto deactivate(const Pith::Address address, const std::size_t n) -> void;

	auto release(const Pith::Address address, const std::size_t n) -> void;

	Pith::Address address_;
	std::size_t pageCount_;
	std::size_t maxPageCount_;
};

#include <Ab/Memory.inl.hpp>

}  // namespace Ab

#endif  // AB_MEMORY_HPP_
