#ifndef PITH_PAGE_HPP_
#define PITH_PAGE_HPP_

#include <Pith/Config.hpp>
#include <Pith/Address.hpp>
#include <Pith/ArrayBox.hpp>
#include <Pith/Bytes.hpp>
#include <Pith/Result.hpp>
#include <Pith/Span.hpp>
#include <errno.h>
#include <sys/mman.h>

namespace Pith {

namespace {

const constexpr std::size_t PAGE_SIZE = kibibytes(4);
const constexpr std::size_t PAGE_ALIGNMENT = PAGE_SIZE;

}  // namespace

class alignas(PAGE_ALIGNMENT) Page {
public:
	struct Permissions {
		static const constexpr int EXECUTE = PROT_EXEC;
		static const constexpr int WRITE = PROT_WRITE;
		static const constexpr int READ = PROT_READ;
		static const constexpr int NONE = PROT_NONE;
	};

	/// Will bring a page into memory, with no permissions.
	static inline auto map(const Span<Page>& pages, int permissions = Page::Permissions::NONE)
		-> Result<Page*, int> {
		Page* p = (Page*)mmap(
			pages.value(), pages.size(), permissions, MAP_ANON | MAP_PRIVATE, 0, 0);

		if (p != nullptr) {
			return ok(p);
		} else {
			return err(errno);  // TODO: Return errno
		}
	}

	/// Unmap a page from memory.
	/// Returns 0 on success.
	static inline auto unmap(const Span<Page>& pages) -> int {
		int e = munmap(pages.value(), pages.size());
		if (e == 0) {
			return 0;
		} else {
			return errno;
		}
	}

	static inline auto setPermissions(const Span<Page>& pages, int permissions) -> int {
		return mprotect(pages.value(), pages.size(), permissions);
	}

	inline constexpr auto bytes() const -> const ByteArrayBox<PAGE_SIZE>& {
		return bytes_;
	}

	inline auto bytes() -> const ByteArrayBox<PAGE_SIZE>& {
		return bytes_;
	}

private:
	ByteArrayBox<PAGE_SIZE> bytes_;
};

static_assert(sizeof(Page) == PAGE_SIZE, "Page size must be correct.");
static_assert(alignof(Page) == PAGE_ALIGNMENT, "Page allignment must be correct.");

}  // namespace Pith

#endif  // PITH_PAGE_HPP_