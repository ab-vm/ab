#ifndef OM_SYSTEM_INL_HPP_
#define OM_SYSTEM_INL_HPP_

#include <Om/System.hpp>

namespace Om {

// static
constexpr auto System::defaultConfig() -> const SystemConfig& {
	return DEFAULT_CONFIG;
}

System::System() : state_{SystemState::DEAD} {
}

template <typename Function>
auto System::mapRoots(const ExclusiveAccess& exclusive, Function&& function) -> void {
	function(exclusive);
}

}  // namespace Om

#endif  // OM_SYSTEM_INL_HPP_
