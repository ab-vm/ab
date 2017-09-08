#ifndef OM_CONTEXT_INL_HPP_
#define OM_CONTEXT_INL_HPP_

#include <Om/Context.hpp>

namespace Om {

Context::Context(System& system) : system_{system}, state_{ContextState::DEAD} {
}

Context::~Context() {
	PITH_ASSERT(state() == ContextState::DEAD);
}

auto Context::init() -> ContextError {
	PITH_ASSERT(state() == ContextState::DEAD);
	PITH_ASSERT(system().attach(this) == AttachError::SUCCESS);
	state_ = ContextState::INACTIVE;
	return ContextError::SUCCESS;
}

auto Om::Context::kill() -> ContextError {
	PITH_ASSERT(state_ == ContextState::INACTIVE);
	system().detach(this);
	state_ = ContextState::DEAD;
	return ContextError::SUCCESS;
}

auto Context::system() -> System& {
	return system_;
}

auto Context::system() const -> const System& {
	return system_;
}

auto Context::state() const -> ContextState {
	return state_;
}

auto Context::activate() -> void {
	PITH_TRACE();
	OM_DEBUG_ASSERT(state_ == ContextState::INACTIVE);
	state_ = ContextState::ACTIVE;
	// TODO: Attach context to physical thread
	// TODO: Obtain shared heap access
}

auto Context::deactivate() -> void {
	PITH_TRACE();
	OM_DEBUG_ASSERT(state_ == ContextState::ACTIVE);
	state_ = ContextState::INACTIVE;
	// TODO: Detach context from physical thread
	// TODO: Release shared heap access
}

auto Context::pause() -> void {
	PITH_TRACE();
	OM_DEBUG_ASSERT(state_ == ContextState::ACTIVE);
	state_ = ContextState::INACTIVE;
	// TODO: Release shared heap access
}

auto Context::resume() -> void {
	PITH_TRACE();
	OM_DEBUG_ASSERT(state_ == ContextState::INACTIVE);
	state_ = ContextState::ACTIVE;
	// TODO: Obtain shared heap access.
}

}  // namespace Om

#endif  // OM_CONTEXT_INL_HPP_
