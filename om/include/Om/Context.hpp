#ifndef OM_CONTEXT_HPP_
#define OM_CONTEXT_HPP_

#include <Om/Config.hpp>
#include <Om/Debug.hpp>
#include <Om/StackRootList.hpp>
#include <Om/System.hpp>
#include <Pith/Assert.hpp>

namespace Om {

enum class ContextError { SUCCESS, ERROR };

enum class ContextState { DEAD, INACTIVE, ACTIVE, PAUSED };

class Context {
public:
	inline Context(System& system);

	inline ~Context();

	inline auto init() -> ContextError;

	inline auto kill() -> ContextError;

	inline auto system() -> System&;

	inline auto system() const -> const System&;

	inline auto stackRoots() -> StackRootList&;

	inline auto stackRoots() const -> const StackRootList&;

	inline auto state() const -> ContextState;

protected:
	friend class ActiveContext;

	/// Activate this context.
	inline auto activate() -> void;

	/// Deactivate this context.
	inline auto deactivate() -> void;

	/// Pause an activated context.
	/// Used by GC internals. At a GC safepoint, all active contexts are paused.
	inline auto pause() -> void;

	/// Resume an active but paused context.
	inline auto resume() -> void;

private:
	System& system_;
	StackRootList stackRoots_;
	ContextState state_;
};

}  // namespace Om

#include <Om/Context.inl.hpp>

#endif  // OM_CONTEXT_HPP_
