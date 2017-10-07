#ifndef OM_SYSTEM_HPP_
#define OM_SYSTEM_HPP_

#include <Om/Config.hpp>
#include <Om/Debug.hpp>
#include <Om/Error.hpp>
#include <Om/Heap.hpp>
#include <Om/Process.hpp>
#include <Om/Size.hpp>
#include <Om/SystemLock.hpp>
#include <Pith/Address.hpp>
#include <Pith/Bytes.hpp>
#include <omr.h>
#include <set>

using namespace Pith::ByteLiterals;

namespace Om {

class Context;

enum class OmrSystemError { SUCCESS, FAIL };

class OmrSystem {
public:
	auto init() -> OmrSystemError;

	auto kill() -> OmrSystemError;

	auto vm() -> OMR_VM&;

private:
	OMR_VM vm_;
};

struct SystemConfig {
	HeapConfig heap_;
};

enum class SystemState { DEAD, ACTIVE };

enum class SystemError { SUCCESS, FAIL };

enum class AttachError { SUCCESS, FAIL };

class System {
public:
	static const constexpr SystemConfig DEFAULT_CONFIG{Heap::defaultConfig()};

	static constexpr inline auto defaultConfig() -> const SystemConfig&;

	inline System();

	~System();

	auto init(const SystemConfig& config) -> SystemError;

	auto kill() -> SystemError;

	inline auto state() const -> SystemState;

	template <typename Function>
	inline auto mapRoots(const ExclusiveAccess& exclusive, Function&& function) -> void;

protected:
	friend class Context;

	AttachError attach(Context* context);

	AttachError detach(Context* context);

private:
	OmrSystem omr_;
	SystemLock lock_;
	Heap heap_;
	std::set<Context*> contexts_;
	SystemState state_;
};

}  // namespace Om

#include <Om/System.inl.hpp>

#endif  // OM_SYSTEM_HPP_
