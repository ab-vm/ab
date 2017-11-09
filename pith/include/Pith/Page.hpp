#ifndef PITH_PAGE_HPP_
#define PITH_PAGE_HPP_

#include <Pith/Config.hpp>
#include <Pith/Address.hpp>
#include <Pith/Bytes.hpp>
#include <Pith/Process.hpp>
#include <Pith/Result.hpp>
#include <errno.h>
#include <sys/mman.h>
#include <system_error>
#include <unistd.h>

namespace Pith {

struct PagePermission {
	static const constexpr int execute = PROT_EXEC;
	static const constexpr int write   = PROT_WRITE;
	static const constexpr int read    = PROT_READ;
	static const constexpr int none    = PROT_NONE;
};

struct PageError : public std::runtime_error {
	using std::runtime_error::runtime_error;
};

class Page {
public:
	Page() = delete;

	/// The size of a page. Must be determined at run time.
	static auto size() noexcept -> std::size_t;

	/// Will bring a page into memory, with no permissions.
	static auto map(const std::size_t n, const int permissions = PagePermission::none)
		-> Address;

	/// Will bring a page into memory, with no permissions.
	static auto
	map(const Address address, const std::size_t size,
	    const int permissions = PagePermission::none) -> Address;

	/// Unmap a page from memory.
	/// Returns 0 on success.
	static auto unmap(const Address address, const std::size_t size) -> void;

	static auto
	setPermissions(const Address address, const std::size_t size, const int permissions)
		-> void;
};

}  // namespace Pith

#include <Pith/Page.inl.hpp>

#endif  // PITH_PAGE_HPP_
