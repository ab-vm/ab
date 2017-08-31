#ifndef AB_ERRORCATEGORY_HPP_
#define AB_ERRORCATEGORY_HPP_

#include <string>
#include <system_error>

namespace Ab {

class ErrorCategory : public std::error_category {
public:
	virtual auto name() const noexcept -> const char* override;
	virtual auto message(int code) const noexcept -> std::string override;

private:
	static constexpr char const* const name_ = "Ab::Error";
};

extern const ErrorCategory ERROR_CATEGORY;

static inline constexpr auto errorCategory() noexcept -> const std::error_category& {
	return ERROR_CATEGORY;
}

}  // namespace Ab

#endif  // AB_ERRORCATEGORY_HPP_
