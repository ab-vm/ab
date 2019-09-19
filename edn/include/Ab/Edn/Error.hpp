#ifndef AB_EDN_PARSEERROR_HPP_
#define AB_EDN_PARSEERROR_HPP_

#include <Ab/Config.hpp>
#include <Ab/Parse/Location.hpp>
#include <fmt/format.h>
#include <string>
#include <vector>

namespace Ab::Edn {

enum class ErrorLevel { INFO, WARNING, ERROR };

struct Error {
	ErrorLevel level;
	Parse::SrcRange pos;
	std::string msg;
};

inline bool operator==(const Error& lhs, const Error& rhs) {
	return lhs.level == rhs.level && lhs.pos == rhs.pos && lhs.msg == rhs.msg;
}

inline bool operator!=(const Error& lhs, const Error& rhs) { return !(lhs == rhs); }

class ErrorLog {
public:
	template <typename... Args>
	void error(Parse::SrcRange pos, std::string msg, Args&&... args) {
		errors_.push_back(
			{ErrorLevel::ERROR, pos, fmt::format(msg, std::forward<Args>(args)...)});
	}

	bool empty() const { return errors_.size() == 0; }

	std::vector<Error>& errors() { return errors_; }

	const std::vector<Error>& errors() const { return errors_; }

private:
	std::vector<Error> errors_;
};

inline bool operator==(const ErrorLog& lhs, const ErrorLog& rhs) {
	const std::size_t size = lhs.errors().size();
	if (size != rhs.errors().size()) {
		return false;
	}

	for (std::size_t i = 0; i < size; ++i) {
		if (lhs.errors()[i] != rhs.errors()[i]) {
			return false;
		}
	}

	return true;
}

inline bool operator!=(const ErrorLog& lhs, const ErrorLog& rhs) { return !(lhs == rhs); }

}  // namespace Ab::Edn

#endif  // AB_EDN_PARSEERROR_HPP_
