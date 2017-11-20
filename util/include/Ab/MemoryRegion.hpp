#ifndef AB_MEMORYREGION_HPP_
#define AB_MEMORYREGION_HPP_

#include <type_traits>
#include <vector>
#include <cstdlib>

namespace Ab {

/// A fix-size slab of memory.
///
class MemoryChunk {
public:
	MemoryChunk(std::size_t size) {
		addr_ = std::malloc(size);
	}

	MemoryChunk(MemoryChunk&&) = default;

	MemoryChunk(const MemoryChunk&) = delete;

	~MemoryChunk() {
		std::free(addr_);
	}

	void* get() const { return addr_; }

private:
	void* addr_;
};

/// A region-based allocator.
///
class MemoryRegion {
public:
	MemoryRegion() {
		get_chunk();
	}

	~MemoryRegion() {}

	void* alloc(std::size_t size) {
		return 
	}

	void dealloc([[maybe_unused]] void* p) {}

private:

	get_chunk() {
		chunks_.push_back()
	}

	void* ptr_;
	void* end_;
	std::vector<void*> page_;
};

template <typename T>
class RegionAllocator {

};

} // namespace Ab

inline void* operator new(std::size_t size, Ab::MemoryRegion& region) {

};

/
inline void operator delete(void* ptr, Ab::MemoryRegion& region) {}

#endif // AB_MEMORYREGION_HPP_
