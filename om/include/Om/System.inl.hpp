#ifndef OM_SYSTEM_INL_HPP_
#define OM_SYSTEM_INL_HPP_

#include <Om/System.hpp>

namespace Om {

inline auto OmrSystem::vm() -> OMR_VM& {
	return vm_;
}

inline auto OmrSystem::vm() const -> const OMR_VM& {
	return vm_;
}

inline System::System(const SystemConfig& cfg) : omr_{}, lock_{}, heap_{cfg.heap} {
}

inline System::~System() noexcept {
	PITH_ASSERT(contexts_.empty());
}

inline auto System::attach(Context* cx) -> void {
	contexts_.insert(cx);
}

inline auto System::detach(Context* cx) -> void {
	contexts_.erase(cx);
}

}  // namespace Om

#endif  // OM_SYSTEM_INL_HPP_
