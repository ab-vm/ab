#ifndef PITH_PROCESS_INL_HPP_
#define PITH_PROCESS_INL_HPP_

#include <Pith/Process.hpp>

namespace Pith {

inline auto SystemProperties::init() -> void {
	pageSize_ = sysconf(_SC_PAGESIZE);
}

inline auto SystemProperties::pageSize() const noexcept -> std::size_t {
	return pageSize_;
}

inline auto Process::init() -> void {
	properties_.init();
	initialized_ = true;
}

inline auto Process::kill() -> void {
	initialized_ = false;
}

inline auto Process::initialized() noexcept -> bool {
	return initialized_;
}

inline auto Process::properties() noexcept -> const SystemProperties& {
	return properties_;
}

}  // namespace Pith

#endif  // PITH_PROCESS_INL_HPP_
