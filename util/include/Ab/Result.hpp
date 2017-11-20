#ifndef AB_RESULT_HPP_
#define AB_RESULT_HPP_

#include <Ab/Config.hpp>
#include <Ab/Assert.hpp>
#include <Ab/Box.hpp>
#include <Ab/SafeDestructor.hpp>
#include <type_traits>
#include <utility>

namespace Ab {

/// A value indicating a result that is OK, or not-an-error.
template <typename T>
class Ok : public Box<T> {
public:
	using Box<T>::Box;
};

/// Ok constructor.
template <typename T, typename U = typename std::decay<T>::type>
inline constexpr auto ok(T&& x) -> Ok<U> {
	return Ok<U>(std::forward<T>(x));
}

/// A value indicating error.
template <typename T>
class Err : public Box<T> {
public:
	using Box<T>::Box;
};

/// Error constructor.
template <typename T, typename U = typename std::decay<T>::type>
inline constexpr auto err(T&& x) -> Err<U> {
	return Err<U>(std::forward<T>(x));
}

/// An operation result.
/// Constructed from Ok or Error objects.
template <typename T, typename E>
class Result {
public:
	enum class Kind { OK, ERR };

	template <typename Tx>
	inline constexpr Result(const Ok<Tx>& x) : value_{x()}, kind_{Kind::OK} {
	}

	template <typename Tx>
	inline constexpr Result(Ok<Tx>&& x) : value_{std::move(x())}, kind_{Kind::OK} {
	}

	template <typename Ex>
	inline constexpr Result(Err<Ex>& x) : err_{x()}, kind_{Kind::ERR} {
	}

	template <typename Ex>
	inline constexpr Result(Err<Ex>&& x) : err_{std::move(x())}, kind_{Kind::ERR} {
	}

	template <typename Tx, typename Ex>
	inline Result(Result<Tx, Ex>& x) {
		if (x) {
			new (&value_) T{x()};
			kind_ = Kind::OK;
		} else {
			new (&err_) E{x.err()};
			kind_ = Kind::ERR;
		}
	}

	template <typename Tx, typename Ex>
	inline Result(Result<Tx, Ex>&& x) {
		if (x) {
			new (&value_) T{std::move(x())};
			kind_ = Kind::OK;
		} else {
			new (&err_) E{std::move(x.err())};
			kind_ = Kind::ERR;
		}
	}

	inline ~Result() {
		switch (kind_) {
		case Kind::OK:
			SafeDestructor<T>{}(value_);
			break;
		case Kind::ERR:
			SafeDestructor<E>{}(err_);
			break;
		}
	}

	inline constexpr operator bool() const {
		return kind() == Kind::OK;
	}

	inline constexpr auto isOk() const -> bool {
		return kind() == Kind::OK;
	}

	inline constexpr auto isErr() const -> bool {
		return kind() == Kind::ERR;
	}

	inline constexpr auto kind() const -> Kind {
		return kind_;
	}

	inline auto operator()() -> T& {
		AB_ASSERT(kind() == Kind::OK);
		return value_;
	}

	inline auto operator()() const -> const T& {
		AB_ASSERT(kind() == Kind::OK);
		return value_;
	}

	inline auto err() -> E& {
		AB_ASSERT(kind() == Kind::ERR);
		return err_;
	}

	inline auto err() const -> const E {
		AB_ASSERT(kind() == Kind::ERR);
		return err_;
	}

	template <typename Tx>
	inline auto operator=(const Ok<Tx>& x) -> Result<T, E>& {
		if (kind_ == Kind::ERROR) {
			SafeDestructor<E>{}(err_);
		}
		value_ = x();
		kind_  = Kind::OK;
		return *this;
	}

	template <typename Tx>
	inline auto operator=(Ok<Tx>&& x) -> Result<T, E>& {
		if (kind_ == Kind::ERR) {
			SafeDestructor<E>{}(err_);
		}
		value_ = std::move(x());
		kind_  = Kind::OK;
		return *this;
	}

	template <typename Ex>
	inline auto operator=(const Err<Ex>& x) -> Result<T, E>& {
		err_  = x();
		kind_ = Kind::ERR;
		return *this;
	}

	template <typename Ex>
	inline auto operator=(Err<Ex>&& x) -> Result<T, E>& {
		err_  = std::move(x());
		kind_ = Kind::ERR;
		return *this;
	}

	template <typename Tx, typename Ex>
	inline auto operator=(const Result<Tx, Ex>& x) -> Result<T, E>& {
		if (x.isValue()) {
			value_ = x.value();
			kind_  = Kind::OK;
		} else {
			err_  = x.err();
			kind_ = Kind::ERR;
		}
		return *this;
	}

	template <typename Tx, typename Ex>
	inline auto operator=(Result<Tx, Ex>&& x) -> Result<T, E>& {
		if (x.isValue()) {
			value_ = std::move(x.value());
			kind_  = Kind::OK;
		} else {
			err_  = std::move(x.error());
			kind_ = Kind::ERR;
		}
		return *this;
	}

	union {
		T value_;
		E err_;
	};

	Kind kind_;
};

}  // namespace Ab

#endif  // AB_RESULT_HPP_
