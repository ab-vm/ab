#ifndef OM_ERROR_HPP_
#define OM_ERROR_HPP_

#include <string>
#include <system_error>

namespace Om {

class ErrorCategory : public std::error_category {
public:
	virtual auto name() const noexcept -> const char* override;
	virtual auto message(int code) const noexcept -> std::string override;

private:
	static constexpr char const* const name_ = "Om::Error";
};

extern const ErrorCategory ERROR_CATEGORY;

static inline constexpr auto errorCategory() noexcept -> const std::error_category& {
	return ERROR_CATEGORY;
}

}  // namespace Om

#endif  // OM_ERROR_HPP_
