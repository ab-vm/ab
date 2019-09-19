#ifndef AB_RUNTIME_HPP_
#define AB_RUNTIME_HPP_

#include <Ab/Config.hpp>

#ifdef AB_USE_OMR
#include <OMR/Om/Runtime.hpp>
#endif

#include <stdexcept>

namespace Ab {

/// Global init and shutdown.
/// Process-wide! Thread unsafe!
class Runtime {
public:
	Runtime() {
#ifndef AB_USE_OMR
		initialized_ = false;
#endif
	}

	bool initialized() const {
#ifdef AB_USE_OMR
		return om().initialized();
#else
		return initialized_;
#endif
	}

	int init() {
#ifdef AB_USE_OMR
		return om().init();
#else
		initialized_ = true;
		return 0;
#endif
	}

	void kill() noexcept {
#ifdef AB_USE_OMR
		om().kill();
#else
		initialized_ = false;
#endif
	}

#ifdef AB_USE_OMR
	OMR::Om::Runtime& om() { return om_; }

	const OMR::Om::Runtime& om() const { return om_; }
#endif

private:
#ifdef AB_USE_OMR
	OMR::Om::Runtime om_;
#else
	bool initialized_;
#endif
};

class AutoRuntime : public Runtime {
public:
	AutoRuntime() {
		auto e = init();
		if (e) throw std::exception();
	}

	~AutoRuntime() {
		kill();
	}
};

}  // namespace Ab

#endif  // AB_RUNTIME_HPP_
