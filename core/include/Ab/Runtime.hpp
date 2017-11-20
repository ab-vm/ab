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
	bool initialized() const {
#ifdef AB_USE_OMR
		return om().initialized();
#else
		return true;
#endif
	}

	int init() {
#ifdef AB_USE_OMR
		return om().init();
#else
		return true;
#endif
	}

	void kill() noexcept {
#ifdef AB_USE_OMR
		om().kill();
#endif
	}

#ifdef AB_USE_OMR
	OMR::Om::Runtime& om() { return om_; }

	const OMR::Om::Runtime& om() const { return om_; }

private:
	OMR::Om::Runtime om_;
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
