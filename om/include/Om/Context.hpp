#ifndef OM_CONTEXT_HPP_
#define OM_CONTEXT_HPP_

#include <Om/Config.hpp>
#include <Om/Debug.hpp>
#include <Om/System.hpp>
#include <Pith/Assert.hpp>

namespace Om {

enum class ContextError { SUCCESS, ERROR };

enum class ContextState { DEAD, INACTIVE, ACTIVE, PAUSED };

class Context {
public:
	Context(System& system) : system_{system}, state_{ContextState::DEAD} {
	}

	~Context() {
		PITH_ASSERT(state() == ContextState::DEAD);
	}

	inline auto init() -> ContextError {
		PITH_ASSERT(state() == ContextState::DEAD);
		PITH_ASSERT(system().attach(this) == AttachError::SUCCESS);
		state_ = ContextState::INACTIVE;
		return ContextError::SUCCESS;
	}

	inline auto kill() -> ContextError {
		PITH_ASSERT(state_ == ContextState::INACTIVE);
		system().detach(this);
		state_ = ContextState::DEAD;
		return ContextError::SUCCESS;
	}

	inline auto system() -> System& {
		return system_;
	}

	inline constexpr auto system() const -> const System& {
		return system_;
	}

	inline constexpr auto state() const -> ContextState {
		return state_;
	}

protected:
	friend class ActiveContext;

	inline auto activate() -> void {
		PITH_TRACE();
		OM_DEBUG_ASSERT(state_ == ContextState::INACTIVE);
		state_ = ContextState::ACTIVE;
		// TODO: Attach context to physical thread
		// TODO: Obtain shared heap access
	}

	inline auto deactivate() -> void {
		PITH_TRACE();
		OM_DEBUG_ASSERT(state_ == ContextState::ACTIVE);
		state_ = ContextState::INACTIVE;
		// TODO: Detach context from physical thread
		// TODO: Release shared heap access
	}

	inline auto pause() -> void {
		PITH_TRACE();
		OM_DEBUG_ASSERT(state_ == ContextState::ACTIVE);
		state_ = ContextState::INACTIVE;
		// TODO: Release shared heap access
	}

	inline auto resume() -> void {
		PITH_TRACE();
		OM_DEBUG_ASSERT(state_ == ContextState::INACTIVE);
		state_ = ContextState::ACTIVE;
		// TODO: Obtain shared heap access.
	}

private:
	System& system_;
	ContextState state_;
};

}  // namespace Om

#endif  // OM_CONTEXT_HPP_
