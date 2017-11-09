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

class OmrSystem {
public:
	OmrSystem();

	~OmrSystem() noexcept;

	auto vm() -> OMR_VM&;

	auto vm() const -> const OMR_VM&;

private:
	OMR_VM vm_;
};

struct SystemConfig {
	HeapConfig heap;
};

class System {
public:
	inline System(const SystemConfig& cfg);

	inline ~System() noexcept;

protected:
	friend class Context;

	inline auto attach(Context* context) -> void;

	inline auto detach(Context* context) -> void;

private:
	OmrSystem omr_;
	SystemLock lock_;
	Heap heap_;
	std::set<Context*> contexts_;
};

}  // namespace Om

#include <Om/System.inl.hpp>

#endif  // OM_SYSTEM_HPP_
