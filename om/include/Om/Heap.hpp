#if !defined(OM_GC_HPP_)
#define OM_GC_HPP_

#include <Om/Config.hpp>
#include <Om/Size.hpp>
#include <Pith/Bytes.hpp>
#include <Pith/SharedLock.hpp>
#include <system_error>

using namespace Pith::ByteLiterals;

namespace Om {

enum class HeapError {
	SUCCESS, ERROR
};

struct HeapConfig {
	void* preferredAddress_;
	std::size_t minimumSize_;
	std::size_t maximumSize_;
	std::size_t initialSize_;
};

class Heap {
public:
	static const constexpr HeapConfig DEFAULT_CONFIG = {
		.preferredAddress_ = nullptr,
		.maximumSize_ = 1_GiB,
		.minimumSize_ = 1_MiB,
		.initialSize_ = 1_MiB
	};

	static constexpr inline auto defaultConfig() -> const HeapConfig & {
		return DEFAULT_CONFIG;
	}

	Heap();
	~Heap();

	auto init(const HeapConfig& config) -> HeapError;
	auto kill() -> HeapError;

private:
	HeapConfig config_;
	Pith::SharedLock lock_;
	bool dead_;
};

}  // namespace Om

#endif  // OM_GC_HPP_