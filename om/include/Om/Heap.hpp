#if !defined(OM_GC_HPP_)
#define OM_GC_HPP_

#include <Om/Config.hpp>
#include <Om/Size.hpp>
#include <Pith/Bytes.hpp>
#include <Pith/SharedLock.hpp>
#include <system_error>

using namespace Pith::ByteLiterals;

namespace Om {

enum class HeapError { SUCCESS, ERROR };

struct HeapConfig {
	void* preferredAddress_;
	std::size_t minimumSize_;
	std::size_t maximumSize_;
	std::size_t initialSize_;
};

enum class HeapCondition { DEAD, ACTIVE };

class Heap {
public:
	static constexpr inline auto defaultConfig() -> const HeapConfig&;

	inline Heap();
	inline ~Heap();

	auto init(const HeapConfig& config) -> HeapError;
	auto kill() -> HeapError;

private:
	static const constexpr HeapConfig DEFAULT_CONFIG{nullptr, 1_GiB, 1_MiB, 1_MiB};

	Pith::SharedLock lock_;
	HeapCondition condition_;
};

}  // namespace Om

#include <Om/Heap.inl.hpp>

#endif  // OM_GC_HPP_