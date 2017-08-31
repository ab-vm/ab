#ifndef AB_MEMORY_HPP_
#define AB_MEMORY_HPP_

#include <Pith/Address.hpp>
#include <sys.mman.h>

#include <cstdint>
#include <cstdlib>
#include <type_traits>

namespace ab {

typedef unsigned long Value;

/// init an object
template <typename Type, typename... Args> bool init(Type t, Args... args);

#if 0  // heapCache
class Heap;

class HeapCache {
public:
	static const uint8_t defaultSize = 1_KiB;

	/// Raw byte allocator. May GC.
	inline uint8_t* allocateBytes(size_t size) {
		if (bytes < (start - end)) {
			if (!renewCache(bytes)) {
				return nullptr;
			}
		}
		auto x = rawMemory;
		rawMemory += bytes;
		return x;
	}

	bool renewCache(size_t size) {
		heap.allocateBytes(defaultSize + bytes);
	}

	inline uint8_t* allocateBytesNoGC(std::size_t bytes) {
		
	}
private:
	uint8_t* rawMemory;
	uint8_t* end;
};

#endif  // 0

class Heap {
public:
};

class MemoryConfig {
public:
	std::size_t initialPageCount_;
	std::size_t maxPageCount_;

	void verify() {
		PITH_ASSERT(maxPageCount_ <= initialPageCount_);
		PITH_ASSERT(maxPageCount > 1);
		PITH_ASSERT(initialPageCount > 1);
	}
};

enum class MemoryState { ACTIVE, DEAD };

enum class MemoryError {
	SUCCESS,
	ERROR
}

class Memory {
public:
	Memory() : memoryState_{MemoryState::DEAD} {};

	inline auto init(MemoryConfig& memoryConfig) -> MemoryError {
		PITH_ASSERT(state_ == MemoryState::DEAD);
		memoryConfig.verify();

		pageSize_ = memoryConfig.pageSize_;
		pageCount_ = memoryConfig.initialPageCount_;
		maxPageCount = memoryConfig.maxPageCount_;

		void* mapResult =
			mmap(nullptr, pageSize_ * pageCount_, PROT_NONE, MAP_ANON | MAP_PRIVATE);

		if (mapResult == MAP_FAILED) {
			// TODO implement error handling
			ASSERT_UNREACHABLE();
		}
		baseAddress_ = mapResult;

		mprotect(baseAddress_, pageCount_ * pageSize_, PROT_READ | PROT_WRITE);

		state_ = MemoryState.ALIVE;
		return MemoryError::SUCCESS;
	}

	inline auto kill() -> MemoryError {
		PITH_ASSERT(memoryState_ == MemoryState::ALIVE);
		memoryState = MemoryState.DEAD;
		return MemoryError::SUCCESS;
	}

	Pith::Address baseAddress() {
		return baseAddress_;
	};

	std::size_t size() {
		return pageCount_ * pageSize_;
	}

	std::size_t pageCount() {
		return pageCount_;
	}

	std::size_t maxSize() {
		return maxSize_;
	}

	std::size_t getMaxPageCount() {
		return maximumSize_ * pageCount_;
	}

	inline auto grow(std::size_t n) -> MemoryError {
		if (pageCount + pageCount_ > maxPageCount) {
			return MemoryError ::ERROR;
		}

		Span<Page> newPageSpan{pages_.end() + 1, n};
		std::size_t growSize = pageCount * pageSize_;

		Span<Page> newPages{pages_.end() + 1, n};
		int perm = Page::Permission::READ | Page::Permission::WRITE;
		Result<Page*, PageMapErrror> result = Page::map(newPages, perm);
		PITH_ASSERT(result);

		return MemoryError::SUCCESS;
	}

private:
	Span<Page> pages_;
	void* baseAddress_;
	std::size_t maxPageCount_;
	MemoryState state_;
};

class Memory {
public:
	Heap heap;

	/// Allocate an object on the heap.
	/// auto x = allocate<Type>(initializer, args...)
	/// initializer(Type* x, args...);
	/// Type must be a simply allocated struct.
	/// The initializer's return value is ignored.
	template <typename Type, typename InitFunc, typename... InitArgs>
	Type* allocate(MemoryContext* mx, InitFunc&& init, InitArgs&&... initArgs) {
		Type* x = allocateBytes<Type>(sizeof(Type));
		if (x != nullptr) {
			init(x, initArgs...);
		}
		return x;
		static_assert(std::is_pod<Type>(), "Type must be a simple struct");
	}
};

class MemoryContext {
public:
	MemoryContext(Memory& memory) : memory{memory} {
	}

private:
	Memory& memory;

	template <typename Type> Type* allocateBytes() {
		static_cast<Type>(allocateBytes(sizeof(Type)));
	}

	template <typename Type = void*> allocateBytes(std::size_t bytes) {
		// TODO: something better than malloc
		return static_cast<Type>(std::malloc(bytes));
	}
};

}  // namespace ab

#endif  // AB_MEMORY_HPP_