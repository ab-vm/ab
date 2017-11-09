#ifndef OM_OMRTHREAD_INL_HPP_
#define OM_OMRTHREAD_INL_HPP_

#include <Om/OmrThread.hpp>

namespace Om {

inline OmrThread::OmrThread() : handle_{nullptr} {
	if (0 != omrthread_attach_ex(&handle_, J9THREAD_ATTR_DEFAULT)) {
		throw std::runtime_error("Failed to attach thread during OMR startup");
	}
}

inline OmrThread::~OmrThread() noexcept {
	omrthread_detach(handle_);
}

inline auto OmrThread::handle() const noexcept -> omrthread_t {
	return handle_;
}

}  // namespace Om

#endif  // OM_OMRTHREAD_INL_HPP_
