#ifndef PITH_RESULT_HPP_
#define PITH_RESULT_HPP_

#include <Pith/Config.hpp>
#include <Pith/Assert.hpp>
#include <Pith/SafeDestructor.hpp>
#include <Pith/Box.hpp>
#include <utility>

namespace Pith {

/// A value indicating a result that is OK, or not-an-error.
template <typename T>
class Ok : public Box<T> {
public:
	using Box<T>::Box;
};

/// Ok constructor.
template <typename T>
inline constexpr auto ok(T&& t) -> Ok<T> {
	return Ok<T>(t);
}

/// A value indicating error.
template <typename E>
class Error : public Box<E> {
public:
	using Box<E>::Box;
};

/// Error constructor
template <typename E>
inline constexpr auto error(E&& e) -> Error<E> {
	return Error<E>(e);
}

enum class ResultKind { OK, ERROR };

/// An operation result.
/// Constructed from Ok or Error objects.
template <typename T, typename E>
class Result {
public:

	template <typename Tx>
	inline constexpr Result(const Ok<Tx> & x) : value_{x()}, kind_{ResultKind::OK} {
	}

	template <typename Tx>
	inline constexpr Result(Ok<Tx>&& x) : value_{std::move(x())}, kind_{ResultKind::OK} {
	}

	template <typename Ex>
	inline constexpr Result(Error<Ex>& x) : error_{x()}, kind_{ResultKind::ERROR} {
	}

	template <typename Ex>
	inline constexpr Result(Error<Ex>&& x) : error_{std::move(x())}, kind_{ResultKind::ERROR} {
	}

	inline ~Result() {
		switch(kind_) {
			case ResultKind::OK:
				SafeDestructor<T>{}(value_);
				break;
			case ResultKind::ERROR:
				SafeDestructor<E>{}(error_);
				break;
		}
	}

	inline constexpr auto kind() const -> ResultKind {
		return kind_;
	}

	inline constexpr auto isOk() const -> bool {
		return kind() == ResultKind::OK;
	}

	inline constexpr auto isError() const -> bool {
		return kind() == ResultKind::ERROR;
	}

	inline auto value() -> T & {
		PITH_ASSERT(kind() == ResultKind::OK);
		return value_;
	}

	inline auto value() const -> const T & {
		PITH_ASSERT(kind() == ResultKind::OK);
		return value_;
	}

	inline auto error() -> E & {
		PITH_ASSERT(kind() == ResultKind::ERROR);
		return error_;
	}

	inline auto error() const -> const E {
		PITH_ASSERT(kind() == ResultKind::ERROR);
		return error_;
	}

	template <typename Tx>
	inline auto operator=(const Ok<Tx>& x) -> Result<T, E>& {
		if (kind_ == ResultKind::ERROR) {
			SafeDestructor<E>{}(error_);
		}
		value_ = x();
		kind_ = ResultKind::OK;
		return *this;
	}

	template <typename Tx>
	inline auto operator=(Ok<Tx>&& x) -> Result<T, E>& {
		if (kind_ == ResultKind::ERROR) {
			SafeDestructor<E>{}(error_);
		}
		value_ = std::move(x());
		kind_ = ResultKind::OK;
		return *this;
	}

	template <typename Ex>
	inline auto operator=(const Error<Ex>& x) -> Result<T, E>& {
		error_ = x();
		kind_ = ResultKind::ERROR;
		return *this;
	}

	template <typename Ex>
	inline auto operator=(Error<Ex>&& x) -> Result<T, E>& {
		error_ = std::move(x());
		kind_ = ResultKind::ERROR;
		return *this;
	}

	template <typename Tx, typename Ex>
	inline auto operator=(const Result<Tx, Ex>& x) -> Result<T, E>& {
		if (x.isValue()) {
			value_ = x.value();
			kind_ = ResultKind::OK;
		} else {
			error_ = x.error();
			kind_ = ResultKind::ERROR;
		}
		return *this;
	}

	template <typename Tx, typename Ex>
	inline auto operator=(Result<Tx, Ex>&& x) -> Result<T, E>& {
		if (x.isValue()) {
			value_ = std::move(x.value());
			kind_ = ResultKind::OK;
		} else {
			error_ = std::move(x.error());
			kind_ = ResultKind::ERROR;
		}
		return *this;
	}

	union {
		T value_;
		E error_;
	};

	ResultKind kind_;
};

}  // namespace Pith

#endif  // PITH_RESULT_HPP_
