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
template <typename Type, typename... Args>
bool init(Type t, Args... args);

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
	std::size_t pageSize_;
};

enum class MemoryState { ACTIVE, DEAD };

enum class MemoryError {
	SUCCESS,
	ERROR
}

///
class Memory {
public:
	Memory() : memoryState_(MemoryState::DEAD){};

	MemoryError init(MemoryConfig& memoryConfig) {
		PITH_ASSERT(memoryState_ == MemoryState::DEAD);

		memoryState = MemoryState.ALIVE;

		pageSize_ = memoryCOnfig.pageSize_;
		pageCount_ = memoryConfig.initialPageCount_;
		maxPageCount = memoryConfig.maxPageCount;

		PITH_ASSERT(pageCount <= maxPageCount);

		void* mapResult = mmap(nullptr, pageSize_ * pageCount_, PROT_NONE, MAP_ANON | MAP_PRIVATE);

		if (mapResult == MAP_FAILED) {
			// TODO implement error handling
			ASSERT_UNREACHABLE();
		}
		baseAddress_ = mapResult;

		mprotect(baseAddress_, pageCount_ * pageSize_, PROT_READ | PROT_WRITE);

		return MemoryError::SUCCESS;
	}

	MemoryError kill() {
		PITH_ASSERT(memoryState_ == MemoryState::ALIVE);

		memoryState = MemoryState.DEAD;

		return MemoryError::SUCCESS;
	}

	Pith::Address getBaseAddress() {
		return baseAddress_;
	};

	std::size_t getSize() {
		return pageCount_ * page_Size_;
	}

	std::size_t getPageCount() {
		return pageCount_;
	}

	std::size_t getMaxSize() {
		return maximumSize_;
	}

	std::size_t getMaxPageCount() {
		return maximumSize_ * pageCount_;
	}

	MemoryError grow() {
		return grow(1);
	}

	MemoryError grow(size_t pageCount) {
		std::size_t growSize = pageCount * pageSize_;

		if (pageCount + pageCount_ > maxPageCount) {
			return MemoryError ::ERROR;
		}

		return MemoryError::SUCCESS;
	}

private:
	void* baseAddress_;
	std::size_t pageSize_;
	std::size_t pageCount_;
	std::size_t maxPageCount;
	MemoryState memoryState_;
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

	template <typename Type>
	Type* allocateBytes() {
		static_cast<Type>(allocateBytes(sizeof(Type)));
	}

	template <typename Type = void*>
	allocateBytes(std::size_t bytes) {
		// TODO: something better than malloc
		return static_cast<Type>(std::malloc(bytes));
	}
};

}  // namespace ab

#endif  // AB_MEMORY_HPP_