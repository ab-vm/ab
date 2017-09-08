#ifndef OM_ACTIVECONTEXT_HPP_
#define OM_ACTIVECONTEXT_HPP_

#include <Om/Config.hpp>
#include <Om/Context.hpp>

namespace Om {

class ActiveContext {
public:
	explicit inline ActiveContext(Context& context) : context_{context} {
		context_.activate();
	}

	explicit inline ActiveContext(ActiveContext&& other) : context_{other.context()} {
	}

	ActiveContext(const ActiveContext& cx) = delete;

	inline ~ActiveContext() {
		context_.deactivate();
	}

	inline auto context() -> Context& {
		return context_;
	}

	inline auto context() const -> const Context& {
		return context_;
	}

protected:
	friend class System;

	auto pause() -> void {
		PITH_TRACE();
		context_.state_ = ContextState::PAUSED;
	}

	auto resume() -> void {
		PITH_TRACE();
		context_.state_ = ContextState::ACTIVE;
	}

private:
	Context& context_;
};

}  // namespace Om

#endif  // OM_ACTIVECONTEXT_HPP_
