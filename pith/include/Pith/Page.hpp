#ifndef PITH_PAGE_HPP_
#define PITH_PAGE_HPP_

#include <Pith/Config.hpp>
#include <Pith/Address.hpp>
#include <Pith/ArrayBox.hpp>
#include <Pith/Bytes.hpp>
#include <Pith/Result.hpp>
#include <Pith/Span.hpp>
#include <sys/mman.h>

namespace Pith {

namespace {
const constexpr std::size_t PAGE_SIZE = kibibytes(4);
const constexpr std::size_t PAGE_ALIGNMENT = PAGE_SIZE;
}  // namespace

class PagePermissions : public Box<int> {
public:
	static const constexpr int EXECUTE = PROT_EXEC;
	static const constexpr int WRITE = PROT_WRITE;
	static const constexpr int READ = PROT_READ;
	static const constexpr int NONE = PROT_NONE;

	using Box::Box;
};

enum PagePermission { EXECUTE = 1, READ = 2, WRITE = 4 };

enum class PageMapError { SUCCESS, FAILURE };

class alignas(PAGE_ALIGNMENT) Page : public ByteArrayBox<PAGE_SIZE> {
public:
	static inline auto map(const Span<Page>& span) -> Result<Page*, PageMapError> {
		void* p = mmap(span(), span.size(), PROT_NONE, MAP_ANON | MAP_PRIVATE, 0, 0);
		if (p != nullptr) {
			return ok(p);
		} else {
			/// TODO: Return errno
			return error(PageMapError::FAILURE);
		}
	}

	static inline auto unmap(const Span<Page>& span) -> PageMapError {
		int r = munmap(span.value(), span.size());
		if (r != 0) {
			return PageMapError::FAILURE;
		} else {
			// TODO: Return errno
			return PageMapError::SUCCESS;
		}
	}

	static inline auto setPermissions(const Span<Page>& span, PagePermissions permissions) -> int {
		return mprotect(span(), span.size(), permissions());
	}

	static inline auto getPermissions(const Page* page) -> PagePermissions {
		PITH_ASSERT_UNREACHABLE();
		return PagePermissions{0};
	}
};

static_assert(sizeof(Page) == PAGE_SIZE, "Page size must be correct.");
static_assert(alignof(Page) == PAGE_ALIGNMNENT, "Page allignment must be correct.");

}  // namespace Pith

#endif  // PITH_PAGE_HPP