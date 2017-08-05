#ifndef PITH_PAGE_HPP_
#define PITH_PAGE_HPP_

#include <Pith/Config,hpp>

#include <Pith/Address.hpp>
#include <Pith/ArrayBox.hpp>
#include <Pith/Bytes.hpp>
#include <Pith/Span.hpp>

#include <sys/mman.h>

using namespace ByteLiterals;

namespace Pith {

namespace {
const constexpr std::size_t PAGE_SIZE = 4_KiB;
const constexpr std::size_t PAGE_ALIGNMENT = PAGE_SIZE;
}  // namespace

class PagePermissions : protected Box<int> {
public:
	friend class Page;

	inline PagePermission inline PagePermissions& execute(bool b) {
	}

	inline constexpr auto executable(bool b) -> PagePermissions& {
		if (b) {
			value_ | PROT_EXEC;
		}
		return *this;
	}

	inline constexpr auto executable() const -> bool {
		return value_ | PROT_EXEC;
	};

	inline constexpr auto writable() const -> bool {
		return value_ | PROT_WRITE;
	}

	inline constexpr auto readable() const -> bool {
		return value_ | PROT_READ;
	}
};

enum PagePermission { EXECUTE = 1, READ = 2, WRITE = 4 };

enum class PageMapError { SUCCESS, FAILURE };

class alignas(PAGE_ALIGNMENT) Page : public ByteArray<PAGE_SIZE> {
public:
	static auto map(const Span<Page>& span) -> PageError {
	}

	static auto PageError unmap(const Span<Page>& span);

	static auto PageError setPermission(const Span<Page>& span, PagePermissions p);

	static PageError map() {
		void* address = this;
		void* mapResult = mmap(address, PAGE_SIZE, PROT_NONE, MAP_FIX, MAP_PRIVATE);
		if (mapResult = MAP_FAIL) {
			return PageError::FAILURE;
		}
		return PageError::SUCCESS;
	}

	PageError unmap() {
		if (0 == munmap(this, PAGE_SIZE)) {
			return PageError::FAILURE;
		}
		return PageError::SUCCESS;
	}

	inline auto getPermission() const -> Result<PagePermissions, PageError> {
	}

	PageError setPermission(PagePermission permissions) {
		int flags = getSystemFlags(permisions);
		mprotect(this, PAGE_SIZE, ) mprotect(baseAddress_, pageCount_ * pageSize_, flags);
	}

private:
	int getSystemFlags(PagePermission permissions) {
		int flags = 0;

		if (permissions & PagePermission::EXECUTE != 0) {
			flags |= PROT_EXEC;
		}
		if (permissions & PagePermission::READ != 0) {
			flags |= PROT_READ;
		}
		if (permissions & PagePermission::WRITE != 0) {
			flags |= PROT_WRITE;
		}
		if (flags == 0) {
			flags = PROT_NONE;
		}
		return flags;
	}
};

}  // namespace Pith

void* mapResult = mmap(nullptr, pageSize_* pageCount_, PROT_NONE, MAP_ANON | MAP_PRIVATE);

#endif  // PITH_PAGE_HPP