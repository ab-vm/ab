#ifndef AB_LINEARMEMORY_HPP_
#define AB_LINEARMEMORY_HPP_

#include <Ab/Config.hpp>
#include <Ab/Address.hpp>
#include <Ab/Assert.hpp>
#include <Ab/Bytes.hpp>
#include <Ab/Page.hpp>
#include <Ab/Result.hpp>
#include <Ab/Span.hpp>
#include <cstdint>

namespace Ab {

class LinearMemoryError : public std::runtime_error {
	using std::runtime_error::runtime_error;
};

struct LinearMemoryConfig {
	MutAddress address         = nullptr;
	std::size_t page_count_min = 1;
	std::size_t page_count_max = 4;

	void verify() const {
		if (page_count_max < page_count_min) {
			throw LinearMemoryError(
				"LinearMemoryConfig validation error: minPageCount greater than "
				"max");
		}
	}
};

/// The contiguous memory subsystem.
///
/// web assembly gives programs low level access to a contiguous region of memory.
/// The LinearMemory class manages that giant blob of memory. Per the spec, LinearMemory can be
/// grown, but does not shrink.
///
class LinearMemory {
public:
	/// The size of a memory page.
	static std::size_t page_size() { return Page::size(); }

	/// Bring up the memory subsytem.
	///
	LinearMemory(const LinearMemoryConfig& config)
		: address_(nullptr), page_count_(0), config_(config) {
		config_.verify();
		address_ = reserve(config.address, config.page_count_max);
		grow(config.page_count_min);
	}

	/// Bring up the memory subsystem with the default config.
	///
	LinearMemory() : LinearMemory(LinearMemoryConfig()) {}

	~LinearMemory() { release(address_, max_size()); }

	/// The address.

	///
	MutAddress address() const noexcept { return address_; }

	/// The size of the currently allocated memory.
	///
	std::size_t size() const noexcept { return page_count_ * page_size(); }

	/// The maximum size the memory can grow to.
	///
	std::size_t max_size() const noexcept { return config_.page_count_max; }

	/// Grow the memory by n pages.
	///
	void grow(std::size_t n = 1) {
		// TODO: worry about overflow / underflow
		if (config_.page_count_max - page_count_ < n) {
			throw LinearMemoryError("Failed to grow, not enough reserved pages.");
		}
		activate(address_ + (page_count_ * page_size()), n);
		page_count_ += n;
	}

	/// Shrink the memory by n pages.
	///
	void shrink(std::size_t n = 1) {
		if (page_count_ < n) {
			throw LinearMemoryError("Failed to shrink: not enough active pages.");
		}
		/// TODO: Deactivate region
	}

	const LinearMemoryConfig& config() const noexcept { return config_; }

private:
	MutAddress reserve(const MutAddress address, std::size_t n) {
		return Page::map(address, n * page_size());
	}

	void activate(const MutAddress address, const std::size_t n) {
		// activate the memory region by requesting read/write permissions.
		auto permissions = PagePermission::READ | PagePermission::WRITE;
		Page::set_permissions(address, n * Page::size(), permissions);
	}

	void deactivate(const MutAddress address, const std::size_t n) {
		// deactivate the memory region by disabling all permissions. this should hopefully
		// cause the OS to unmap the memory.
		auto permissions = PagePermission::NONE;
		Page::set_permissions(address, n * Page::size(), permissions);
	}

	void release(const MutAddress address, const std::size_t n) {
		Page::unmap(address, n * Page::size());
	}

	MutAddress address_;
	std::size_t page_count_;
	const LinearMemoryConfig config_;
};

}  // namespace Ab

#endif  // AB_LINEARMEMORY_HPP_
