#ifndef AB_MEMORY_INL_HPP_
#define AB_MEMORY_INL_HPP_

#include <Ab/Memory.hpp>

inline auto MemoryConfig::verify() const -> void {
	PITH_ASSERT(minPageCount <= maxPageCount);
}

inline Memory::Memory() : Memory{MemoryConfig{}} {
}

inline Memory::Memory(const MemoryConfig& config)
	: address_{nullptr}, pageCount_{0}, maxPageCount_{config.maxPageCount} {
	config.verify();
	address_ = reserve(config.address, config.maxPageCount);
	grow(config.minPageCount);
}

inline Memory::~Memory() {
	// TODO: release(address_, reservedPages_);
}

inline auto Memory::grow(std::size_t n) -> void {
	// TODO: worry about overflow / underflow
	if (maxPageCount_ - pageCount_ < n) {
		throw MemoryError{"Failed to grow, not enough reserved pages."};
	}
	// TODO: Activate new region
}

inline auto Memory::shrink(std::size_t n) -> void {
	if (pageCount_ < n) {
		throw MemoryError{"Failed to shrink: not enough active pages."};
	}
	/// TODO: Deactivate region
}

inline auto Memory::reserve(const Pith::Address address, std::size_t n) -> Pith::Address {
	return Pith::Page::map(address, n);
}
inline auto Memory::activate(const Pith::Address address, const std::size_t n) -> void {
	// activate the memory region by requesting read/write permissions.
	auto permissions = Pith::PagePermission::read | Pith::PagePermission::write;
	Pith::Page::setPermissions(address, n * Pith::Page::size(), permissions);
}

inline auto Memory::deactivate(const Pith::Address address, const std::size_t n) -> void {
	// deactivate the memory region by disabling all permissions. this should hopefully cause
	// the OS to unmap the memory.
	auto permissions = Pith::PagePermission::none;
	Pith::Page::setPermissions(address, n * Pith::Page::size(), permissions);
}

inline auto Memory::release(const Pith::Address address, const std::size_t n) -> void {
	Pith::Page::unmap(address, n * Pith::Page::size());
}

#endif  // AB_MEMORY_INL_HPP_
