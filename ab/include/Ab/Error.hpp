#ifndef AB_ERROR_HPP_
#define AB_ERROR_HPP_

#include <Ab/ErrorCategory.hpp>

#include <type_traits>
#include <system_error>

namespace Ab {

enum class Error {
	SUCCESS = 0,
	NONE = SUCCESS,
	OK = SUCCESS,
	FAIL = 1,
};

} // namespace Ab

namespace std {

template <>
struct is_error_condition_enum<Ab::Error> : public std::true_type {};

inline static auto make_error_condition(Ab::Error e) -> error_condition {
	return error_condition(static_cast<int>(e), Ab::errorCategory());
}

} // namespace std

#endif // AB_ERROR_HPP_
