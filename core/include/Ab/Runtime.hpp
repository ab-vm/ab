#ifndef AB_RUNTIME_HPP_
#define AB_RUNTIME_HPP_

#include <Ab/Config.hpp>

#ifdef AB_USE_OMR
#include <OMR/Om/Runtime.hpp>
#endif

#include <stdexcept>

namespace Ab {

/// Global init and shutdown. Process-wide! Thread unsafe!
///
class Runtime {
public:
	Runtime() {
		initialized_ = false;
	}

	bool initialized() const {
		return initialized_;
	}

	int init() {
		initialized_ = true;
		return 0;
	}

	void kill() noexcept {
		initialized_ = false;
	}


private:
	bool initialized_;
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
