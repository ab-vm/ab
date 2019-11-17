#ifndef AB_PAGE_HPP_
#define AB_PAGE_HPP_

#include <Ab/Config.hpp>
#include <Ab/Address.hpp>
#include <Ab/Bytes.hpp>
#include <Ab/Process.hpp>
#include <Ab/Result.hpp>
#include <errno.h>
#include <sys/mman.h>
#include <system_error>
#include <unistd.h>

namespace Ab {

namespace PagePermission {
static const constexpr int EXECUTE = PROT_EXEC;
static const constexpr int WRITE   = PROT_WRITE;
static const constexpr int READ    = PROT_READ;
static const constexpr int NONE    = PROT_NONE;
};  // namespace PagePermission

struct PageError : public std::runtime_error {
	using std::runtime_error::runtime_error;
};

class Page {
public:
	Page() = delete;

	/// The size of a page. Must be determined at run time.
	static std::size_t size() noexcept { return Process::properties().page_size(); }

	/// Will bring a page into memory, with no permissions.
	static MutAddress
	map(MutAddress address, std::size_t size, int permissions = PagePermission::NONE) {
		auto p = mmap(to_mut_ptr(address), size, permissions, MAP_ANON | MAP_PRIVATE, 0, 0);
		if (p == nullptr) {
			throw PageError{"Failed to map pages"};
		}
		return to_mut_address(p);
	}

	/// Will bring a page into memory, with no permissions.
	static MutAddress map(const std::size_t size, const int permissions = PagePermission::NONE) {
		return map(nullptr, size, permissions);
	}

	/// Unmap a page from memory.
	/// Returns 0 on success.
	static void unmap(const MutAddress address, const std::size_t size) {
		auto e = munmap(to_mut_ptr(address), size);
		if (e != 0) {
			throw PageError{"Failed to unmap pages"};
		}
	}

	static void
	set_permissions(const MutAddress address, const std::size_t size, const int permissions) {
		auto e = mprotect(to_mut_ptr(address), size, permissions);
		if (e != 0) {
			throw PageError{"Failed to set page permissions"};
		}
	}
};

}  // namespace Ab

#endif  // AB_PAGE_HPP_
