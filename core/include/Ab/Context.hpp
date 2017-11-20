#ifndef AB_CONTEXT_HPP_
#define AB_CONTEXT_HPP_

#include <Ab/Config.hpp>
#include <Ab/Runtime.hpp>

#ifdef AB_USE_OMR
#include <OMR/Om/Context.hpp>
#include <OMR/Om/MemorySystem.hpp>
#endif

namespace Ab {

class System {
public:
	explicit System(Runtime& runtime) :
		runtime_(runtime)
#ifdef AB_USE_OMR
	, om_(runtime_.om())
#endif
	{}

	System(const System&) = delete;

	System(System&&) = default;

	~System() = default;

	/// Obtain the global runtime this system is attached to.
	Runtime& runtime() const {
		return runtime_;
	}

#ifdef AB_USE_OMR
	/// Obtain the om/gc subsystem.
	OMR::Om::MemorySystem& om() {
		return om_;
	}

	const OMR::Om::MemorySystem& om() const {
		return om_;
	}
#endif

private:
	Runtime& runtime_;

#ifdef AB_USE_OMR
	OMR::Om::MemorySystem om_;
#endif
};

class Context {
public:
	explicit Context(System& system) : system_(system)
#ifdef AB_USE_OMR
	, om_(system.om())
#endif
	{}

	Context(const Context&) = delete;

	Context(Context&&) = default;

	~Context() = default;

#ifdef AB_USE_OMR
	OMR::Om::RunContext& om() {
		return om_;
	}

	const OMR::Om::RunContext& om() const {
		return om_;
	}
#endif

	System& system() const {
		return system_;
	}

private:
	System& system_;
#ifdef AB_USE_OMR
	OMR::Om::RunContext om_;
#endif
};

class RunContext : public Context {};

}  // namespace Ab

#endif  // AB_CONTEXT_HPP_
