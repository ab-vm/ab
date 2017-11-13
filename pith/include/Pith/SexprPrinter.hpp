#ifndef PITH_SEXPRPRINTER_HPP_
#define PITH_SEXPRPRINTER_HPP_

#include <iostream>
#include <ostream>

namespace Pith {

/// An object for printing indentation.
/// Output the indent object to print indentation levels.
struct Indent {
public:
	Indent(std::size_t depth = 0);

	Indent(const Indent& other) = default;

	inline auto operator++() -> Indent&;

	inline auto operator++(int) -> Indent;

	inline auto operator--() -> Indent&;

	inline auto operator--(int) -> Indent;

	inline auto depth() const -> std::size_t;

	inline auto depth(std::size_t depth) -> Indent&;

	friend auto operator<<(std::ostream& out, const Indent& indent) -> std::ostream&;

private:
	const std::size_t MAX_DEPTH = 10;
	std::size_t depth_{0};
};

inline Indent::Indent(std::size_t depth) : depth_{depth} {}

inline auto Indent::operator++() -> Indent& {
	if (depth_ < MAX_DEPTH) {
		++depth_;
	}
	return *this;
}

inline auto Indent::operator++(int) -> Indent {
	Indent copy = *this;
	if (depth_ < MAX_DEPTH) {
		depth_++;
	}
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
	if (depth > MAX_DEPTH) {
		depth = MAX_DEPTH;
	}
	depth_ = depth;
	return *this;
}

inline auto operator<<(std::ostream& out, const Indent& indent) -> std::ostream& {
	for (std::size_t i = 0; i < indent.depth(); i++) {
		out << "  ";
	}
	return out;
}

struct SexprPrinter {
	std::ostream& stream;
	bool fresh        = true;   // line is new
	bool needsSpacing = false;  // needs space between elements.
	Indent indent{0};

	// operator std::ostream&() { return stream; }

	inline auto freshenLine() -> void {
		if (!fresh) {
			fresh        = true;
			needsSpacing = false;
			stream << std::endl;
		}
	}

	inline auto spacing() -> void {
		if (fresh) {
			stream << indent;
			needsSpacing = false;
			fresh = false;
		}
		if (needsSpacing) {
			needsSpacing = false;
			fresh        = false;
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
	out.fresh        = false;
	return out;
}

inline auto operator<<(SexprPrinter& out, std::ostream& (*manipulator)(std::ostream&))
	-> SexprPrinter& {
	out.stream << manipulator;
	return out;
}

inline auto operator<<(Pith::SexprPrinter& out, const Pith::Indent& indent) -> Pith::SexprPrinter& {
	out.stream << indent;
	out.needsSpacing = false;
	out.fresh        = false;
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
	out.fresh        = false;
	out.needsSpacing = false;

	out.indent++;

	return out;
}

struct SexprEnd {};

extern const SexprEnd sexprEnd;

inline auto operator<<(SexprPrinter& out, SexprEnd) -> SexprPrinter& {
	out.stream << ")";
	out.stream.flush();
	out.fresh        = false;
	out.needsSpacing = true;
	out.indent--;

	return out;
}

struct Stringify {};

extern const Stringify stringify;

struct RawPrinter {
	SexprPrinter& sexpr;
};

template <typename T>
inline auto operator<<(const RawPrinter& out, T&& x) -> const RawPrinter& {
	out.sexpr.stream << std::forward<T>(x);
	return out;
}

struct RawStart {};

inline auto operator<<(SexprPrinter& out, const RawStart&) -> RawPrinter {
	out.spacing();
	return RawPrinter{out};
};

extern const RawStart rawStart;

struct RawEnd {};

extern const RawEnd rawEnd;

inline auto operator<<(const RawPrinter& out, const RawEnd&) -> SexprPrinter& {
	out.sexpr.fresh = false;
	out.sexpr.needsSpacing = true;
	return out.sexpr;
}

struct StringifyPrinter {
	SexprPrinter& sexpr;
};

template <typename T>
inline auto operator<<(const StringifyPrinter& out, T&& x) -> SexprPrinter& {
	return out.sexpr << rawStart << "\"" << x << "\"" << rawEnd;
}

inline auto operator<<(SexprPrinter& out, Stringify) -> StringifyPrinter {
	return StringifyPrinter{out};
}

}  // namespace Pith

#endif  // PITH_SEXPRPRINTERHPP_