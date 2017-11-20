#ifndef AB_SEXPRPRINTER_INL_HPP_
#define AB_SEXPRPRINTER_INL_HPP_

#include <Ab/SexprPrinter.hpp>

namespace Ab {

inline Indent::Indent(std::size_t depth) : depth_{depth} {
}

inline auto Indent::operator++() -> Indent& {
	++depth_;
	return *this;
}

inline auto Indent::operator++(int) -> Indent {
	Indent copy = *this;
	depth_++;
	return copy;
}

inline auto Indent::operator--() -> Indent& {
	if (depth_ > 0) {
		--depth_;
	}
	return *this;
}

inline auto Indent::operator--(int) -> Indent {
	Indent copy = *this;
	if (depth_ > 0) {
		depth_--;
	}
	return copy;
}

inline auto Indent::depth() const -> std::size_t {
	return depth_;
}

inline auto Indent::depth(std::size_t depth) -> Indent& {
	depth_ = depth;
	return *this;
}

inline auto operator<<(std::ostream& out, const Indent& indent) -> std::ostream& {
	for (std::size_t i = 0; i < indent.depth(); i++) {
		out << "  ";
	}
	return out;
}

inline SexprPrinter::SexprPrinter(std::ostream& stream)
	: stream_{stream}, indent_{0}, fresh_{true}, needsSpacing_{false} {
}

inline auto SexprPrinter::stream() const -> std::ostream& {
	return stream_;
}

inline auto SexprPrinter::indent() -> Indent& {
	return indent_;
}

inline auto SexprPrinter::indent() const -> const Indent& {
	return indent_;
}

inline auto SexprPrinter::freshen() -> SexprPrinter& {
	if (!fresh_) {
		fresh_        = true;
		needsSpacing_ = false;
		stream_ << std::endl;
	}
	return *this;
}

inline auto SexprPrinter::spacing() -> SexprPrinter& {
	if (fresh_) {
		stream_ << indent_;
	} else if (needsSpacing_) {
		stream_ << " ";
	}
	needsSpacing_ = false;
	fresh_        = false;
	return *this;
}

inline auto SexprPrinter::needsSpacing() const -> bool {
	return needsSpacing_;
}

inline auto SexprPrinter::needsSpacing(bool p) -> SexprPrinter& {
	needsSpacing_ = p;
	return *this;
}

inline auto SexprPrinter::fresh() const -> bool {
	return fresh_;
}

inline auto SexprPrinter::fresh(bool p) -> SexprPrinter& {
	fresh_ = p;
	return *this;
}

inline auto SexprPrinter::dirty() -> SexprPrinter& {
	fresh_        = false;
	needsSpacing_ = true;
	return *this;
}

template <typename T>
inline auto operator<<(SexprPrinter& out, T&& x) -> SexprPrinter& {
	out.spacing();
	out.stream() << std::forward<T>(x);
	out.dirty();
	return out;
}

inline auto operator<<(SexprPrinter& out, std::ostream& (*manipulator)(std::ostream&))
	-> SexprPrinter& {
	out.stream() << manipulator;
	return out;
}

inline auto operator<<(SexprPrinter& out, SexprStart) -> SexprPrinter& {
	out.spacing();
	out.stream() << "(";
	out.fresh(false);
	out.needsSpacing(false);
	out.indent()++;
	return out;
}

inline auto operator<<(SexprPrinter& out, SexprEnd) -> SexprPrinter& {
	out.stream() << ")";
	out.stream().flush();
	out.fresh(false);
	out.needsSpacing(true);
	out.indent()--;
	return out;
}

template <typename T>
inline auto operator<<(const StringifyPrinter& out, T&& x) -> SexprPrinter& {
	return out.sexpr << rawStart << "\"" << x << "\"" << rawEnd;
}

inline auto operator<<(SexprPrinter& out, Stringify) -> StringifyPrinter {
	return StringifyPrinter{out};
}

template <typename T>
inline auto operator<<(const RawPrinter& out, T&& x) -> const RawPrinter& {
	out.sexpr.stream() << std::forward<T>(x);
	return out;
}

inline auto operator<<(SexprPrinter& out, const RawStart&) -> RawPrinter {
	out.spacing();
	return RawPrinter{out};
};

inline auto operator<<(const RawPrinter& out, const RawEnd&) -> SexprPrinter& {
	out.sexpr.fresh(false);
	out.sexpr.needsSpacing(true);
	return out.sexpr;
}

}  // namespace Ab

#endif  // AB_SEXPRPRINTER_INL_HPP_
