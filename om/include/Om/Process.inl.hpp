#ifndef OM_PROCESS_INL_HPP_
#define OM_PROCESS_INL_HPP_

#include <Om/Process.hpp>

namespace Om {

// static
inline auto OmrProcess::runtime() noexcept -> OMR_Runtime& {
	return runtime_;
}

// static
inline auto OmrProcess::port() noexcept -> OMRPortLibrary& {
	return port_;
}

}  // namespace Om

#endif  // OM_PROCESS_INL_HPP_
