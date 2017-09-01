#ifndef AB_ERROR_HPP_
#define AB_ERROR_HPP_

#include <system_error>
#include <type_traits>

namespace Ab {

enum class Error {
	SUCCESS = 0,
	NONE = SUCCESS,
	OK = SUCCESS,
	FAIL = 1,
};

}  // namespace Ab

#endif  // AB_ERROR_HPP_
