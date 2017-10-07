#ifndef PITH_PAGE_INL_HPP_
#define PITH_PAGE_INL_HPP_

#include <Pith/Page.hpp>

namespace Pith {

inline auto Page::size() noexcept -> std::size_t {
	return Process::properties().pageSize();
}

inline auto Page::map(const std::size_t size, const int permissions) -> Address {
	return map(nullptr, size, permissions);
}

inline auto Page::map(const Address address, const std::size_t size, const int permissions)
	-> Address {
	auto p = mmap(toPointer(address), size, permissions, MAP_ANON | MAP_PRIVATE, 0, 0);

	if (p == nullptr) {
		throw PageError{"Failed to map pages"};
	}

	return static_cast<Address>(p);
}

inline auto Page::unmap(const Address address, const std::size_t size) -> void {
	auto e = munmap(toPointer(address), size);
	if (e != 0) {
		throw PageError{"Failed to unmap pages"};
	}
}

inline auto Page::setPermissions(const Address address, const std::size_t size, const int permissions) -> void {
	auto e = mprotect(toPointer(address), size, permissions);
	if (e != 0) {
		throw PageError{"Failed to set page permissions"};
	}
}

}  // namespace Pith

#endif  // PITH_PAGE_INL_HPP_
