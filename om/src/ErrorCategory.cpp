#include <Ab/Error.hpp>
#include <Ab/ErrorCategory.hpp>

namespace Ab {

auto ErrorCategory::name() const noexcept -> const char* {
	return name_;
}

auto ErrorCategory::message(int code) const noexcept -> std::string {
	auto e = static_cast<Error>(code);
	switch (e) {
		case Error::success: return "success";
		case Error::generic: return "generic";
	}
}

const ErrorCategory ERROR_CATEGORY;

} // namespace Ab
