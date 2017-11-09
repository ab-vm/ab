#if !defined(OM_GC_HPP_)
#define OM_GC_HPP_

#include <Om/Config.hpp>
#include <Om/Size.hpp>
#include <Pith/Bytes.hpp>
#include <Pith/SharedLock.hpp>
#include <system_error>

using namespace Pith::ByteLiterals;

namespace Om {

struct HeapConfig {
	void* preferredAddress_{nullptr};
	std::size_t minimumSize_{1_GiB};
	std::size_t maximumSize_{1_MiB};
	std::size_t initialSize_{1_MiB};
};

class Heap {
public:
	inline Heap(const HeapConfig& cfg);

	inline ~Heap();

private:
	Pith::SharedLock lock_;
};

}  // namespace Om

#include <Om/Heap.inl.hpp>

#endif  // OM_GC_HPP_
