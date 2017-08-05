#ifndef OM_SYSTEM_HPP_
#define OM_SYSTEM_HPP_

#include <Om/Config.hpp>
#include <Om/Debug.hpp>
#include <Om/Error.hpp>
#include <Om/Heap.hpp>
#include <Om/Size.hpp>
#include <Om/SystemLock.hpp>
#include <Pith/Address.hpp>
#include <Pith/Bytes.hpp>
#include <set>

using namespace Pith::ByteLiterals;

namespace Om {

class Context;

enum class SystemState {
	DEAD, ACTIVE
};

struct SystemConfig {
	HeapConfig heap_;
};

enum class SystemError {
	SUCCESS, FAIL
};

enum class AttachError {
	SUCCESS, FAIL
};

class System {
public:

	static const constexpr SystemConfig DEFAULT_CONFIG = {
		.heap_ = Heap::defaultConfig()
	};

	static constexpr inline auto defaultConfig() -> const SystemConfig & {
		return DEFAULT_CONFIG;
	};

	System();

	~System();

	auto init(const SystemConfig& config) -> SystemError;

	auto kill() -> SystemError;

	inline constexpr auto state() const -> SystemState { return state_; }

protected:
	friend class Context;

	AttachError attach(Context* context);

	AttachError detach(Context* context);

private:
	SystemLock lock_;
	Heap heap_;
	std::set<Context*> contexts_;
	SystemState state_;
};

} // namespace Om

#endif // OM_SYSTEM_HPP_
