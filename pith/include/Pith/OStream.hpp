#ifndef PITH_OSTREAM_HPP_
#define PITH_OSTREAM_HPP_

#include <ostream>
#include <iostream>

namespace Pith {

struct Indent {
	std::size_t depth{0};
};

inline auto operator<<(std::ostream& out, const Indent& indent) -> std::ostream& {
	for (std::size_t i = 0; i < indent.depth && i < 10; i++) {
		out << "  ";
	}
	return out;
}

struct SexprPrinter {
	std::ostream& stream;
	bool fresh = true; // line is new
	bool needsSpacing = false; // needs space between elements.
	Indent indent{0};

	// operator std::ostream&() { return stream; }

	inline auto freshenLine() -> void {
		if (!fresh) {
			fresh = true;
			needsSpacing = false;
			stream << std::endl << indent;
		}
	}

	inline auto spacing() -> void {
		if (needsSpacing) {
			needsSpacing = false;
			fresh = false;
			stream << " ";
		}
	}
};

extern SexprPrinter debug;

template <typename T>
inline auto operator<<(SexprPrinter& out, T&& x) -> SexprPrinter& {
	out.spacing();
	out.stream << std::forward<T>(x);
	out.needsSpacing = true;
	out.fresh = false;
	return out;
}

inline auto operator<<(SexprPrinter& out, std::ostream& (*manipulator)(std::ostream&)) -> SexprPrinter& {
    out.stream << manipulator;
    return out;
}

inline auto operator<<(Pith::SexprPrinter& out, const Pith::Indent& indent) -> Pith::SexprPrinter& {
	out.stream << indent;
	out.needsSpacing = false;
	out.fresh = false;
	return out;
}

struct FreshLine {};

extern const FreshLine freshLine;

inline auto operator<<(SexprPrinter& out, Pith::FreshLine) -> Pith::SexprPrinter& {
	out.freshenLine();
	return out;
}

struct SexprStart {};

extern const SexprStart sexprStart;

inline auto operator<<(SexprPrinter& out, SexprStart) -> SexprPrinter& {
	out.spacing();
	out.stream << "(";
	out.fresh = false;
	out.needsSpacing = false;

	if (out.indent.depth <= 10)
		out.indent.depth++;

	return out;
}

struct SexprEnd {};

extern const SexprEnd sexprEnd;

inline auto operator<<(SexprPrinter& out, SexprEnd) -> SexprPrinter& {
	out.stream << ")";
	out.stream.flush();
	out.fresh = false;
	out.needsSpacing = true;

	if (out.indent.depth != 0)
			out.indent.depth--;

	return out;
}

struct Stringify {};

extern const Stringify stringify;

struct StringifyPrinter {
	SexprPrinter& sexpr;
};

template <typename T>
inline auto operator<<(const StringifyPrinter& out, T&& x) -> SexprPrinter& {
	out.sexpr << "\"";
	out.sexpr.needsSpacing = false;
	out.sexpr << x;
	out.sexpr.needsSpacing = false;
	out.sexpr << "\"";
	return out.sexpr;
}

inline auto operator<<(SexprPrinter& out, Stringify) -> StringifyPrinter {
	return StringifyPrinter{out};
}

}  // namespace Pith


#endif  // PITH_OSTREAM_HPP_