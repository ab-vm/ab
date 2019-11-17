#ifndef AB_RESOLVER_HPP_
#define AB_RESOLVER_HPP_

#include <Ab/Assert.hpp>
#include <Ab/Module.hpp>

#include <deque>
#include <memory>
#include <set>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

namespace Ab {

class FuncInst;

enum class ExternType { FUNC, GLOBAL, MEMORY };

struct Import {
	std::string module;
	std::string component;
	ExternType type;
};

enum class ExternValKind { UNRESOLVED, ERROR, FUNC };

/// A record of a value available for either import or export.
///
class ExternVal {
public:
	ExternVal() : kind_(ExternValKind::UNRESOLVED) {}

	ExternVal(FuncInst* func) : kind_(ExternValKind::FUNC), func_(func) {}

	ExternValKind kind() const noexcept { return kind_; };

	FuncInst* func() const noexcept {
		AB_ASSERT(kind_ == ExternValKind::FUNC);
		return func_;
	}

	bool resolved() const noexcept { return kind() != ExternValKind::UNRESOLVED; }

private:
	ExternValKind kind_;
	union {
		FuncInst* func_;
	};
};

/// A loader is responsible for on-demand location of modules.
///
/// A loader might be invoked manually by users in native code, or it could
/// happen as part of resolving a symbol.
///
class Loader {
public:
	virtual Module* load(const char* module) = 0;

	Module* load(const std::string& module) { return load(module.c_str()); }
};

/// Import resolver class. Locate imported names and
///
class Resolver {
public:
	virtual ~Resolver() noexcept = 0;

	virtual ExternVal resolve(std::string_view module, std::string_view symbol) = 0;
};

/// The global resolver.
/// Resolve symbols from the global namespace.
///
class BasicResolver final : public Resolver {
public:
	BasicResolver() {}

	virtual ~BasicResolver() noexcept override = default;

	virtual ExternVal resolve(std::string_view module, std::string_view symbol) override {
		return ExternVal();
	}
};

class Linker {
public:
	virtual ~Linker() noexcept = 0;

	virtual void link(ModuleInst* module) = 0;
};

/// Simple linker that does no resolution.
/// If there is a resolution that needs to be complete, the link fails.
///
class BasicLinker final : public Linker {
public:
	virtual ~BasicLinker() noexcept override = default;

	virtual void link(ModuleInst* module) override {}
};

}  // namespace Ab

#endif  // AB_RESOLVER_HPP_
