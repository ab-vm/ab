#ifndef OM_OMRTHREAD_HPP_
#define OM_OMRTHREAD_HPP_

#include <Om/Config.hpp>
#include <omrthread.h>

namespace Om {

class OmrThread {
public:
	inline OmrThread();

	inline ~OmrThread() noexcept;

	inline auto handle() const noexcept -> omrthread_t;

private:
	omrthread_t handle_;
};

}  // namespace Om

#include <Om/OmrThread.inl.hpp>

#endif  // OM_OMRTHREAD_HPP_
