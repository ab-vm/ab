#ifndef AB_SEXPR_HPP_
#define AB_SEXPR_HPP_

#include <Ab/Config.hpp>
#include <Ab/Indent.hpp>

#include <string>

namespace Ab::Sexpr {

/// A formatted printer.
/// Prints S-Expressions, ie (output (style lisp)).
class Formatter {
public:
	Formatter() : indent_{0}, fresh_{true}, needsSpacing_{false} {}

	/// Obtain the indentation state.
	/// To increase the indentation, try:
	/// printer.indent()++;
	auto indent() -> Indent& { return indent_; }

	auto indent() const -> const Indent& { return indent_; }

	/// Prints a newline, if the current line is dirty.
	auto freshen() -> Formatter& {
		if (!fresh_) {
			fresh_        = true;
			needsSpacing_ = false;
			buffer_ += "\n";
		}
		return *this;
	}

	/// Prints spacing between elements, or indentation if the line is fresh.
	auto spacing() -> Formatter& {
		if (fresh_) {
			buffer_ += indent_.tostring();
		} else if (needsSpacing_) {
			buffer_ += " ";
		}
		needsSpacing_ = false;
		fresh_        = false;
		return *this;
	}

	/// True if the line is dirty
	auto needsSpacing() const -> bool { return needsSpacing_; }

	/// Manually set the needs spacing flag.
	auto needsSpacing(bool p) -> Formatter& {
		needsSpacing_ = p;
		return *this;
	}

	/// True if output is currently at the beginning of a line.
	auto fresh() const -> bool { return fresh_; }

	/// Manually set the freshness, if you've been doing raw output.
	auto fresh(bool p) -> Formatter& {
		fresh_ = p;
		return *this;
	}

	/// Manually mark the line as dirty, needsSpacing.
	auto dirty() -> Formatter& {
		fresh_        = false;
		needsSpacing_ = true;
		return *this;
	}

	std::string& buffer() { return buffer_; }

	const std::string& buffer() const { return buffer_; }

	std::string to_string() const { return buffer(); }

	std::string get() const { return buffer(); }

protected:
	std::string buffer_;
	Indent indent_{0};
	bool fresh_        = true;   // line is new
	bool needsSpacing_ = false;  // needs space between elements.
};

auto operator<<(Formatter& out, char c) -> Formatter& {
	out.spacing();
	out.buffer() += c;
	out.dirty();
	return out;
}

auto operator<<(Formatter& out, const char* str) -> Formatter& {
	out.spacing();
	out.buffer() += str;
	out.dirty();
	return out;
}

auto operator<<(Formatter& out, const std::string& str) -> Formatter& {
	out.spacing();
	out.buffer() += str;
	out.dirty();
	return out;
}

/// Fresh line manipulator.
struct FreshLine {};

constexpr FreshLine FRESH;

inline auto operator<<(Formatter& out, const FreshLine&) -> Formatter& {
	out.freshen();
	return out;
}

/// S-Expression start manipulator.
struct Start {};

/// Begin an s-expression. Prints "(".
constexpr Start START;

inline auto operator<<(Formatter& out, const Start&) -> Formatter& {
	out.spacing();
	out.buffer() += "(";
	out.fresh(false);
	out.needsSpacing(false);
	++out.indent();
	return out;
}

/// S-Expression end manipulator.
struct End {};

/// End an s-expression. Prints ")".
constexpr End END;

inline auto operator<<(Formatter& out, const End&) -> Formatter& {
	out.buffer() += ")";
	out.fresh(false);
	out.needsSpacing(true);
	--out.indent();
	return out;
}

inline std::string stringify(const std::string& str) {
	return std::string("\"") + str + "\"";
}

}  // namespace Ab::Sexpr

// template <typename T>
// auto operator<<(Ab::Sexpr::Formatter& out, const T& x) -> Ab::Sexpr::Formatter& {
// 	return out << to_string(x);
// }

auto operator<<(Ab::Sexpr::Formatter& out, unsigned char x) -> Ab::Sexpr::Formatter& {
	fmt::print("asdfasdfasd\n");
	return out << fmt::format("{}", x);
}

auto operator<<(Ab::Sexpr::Formatter& out, unsigned short x) -> Ab::Sexpr::Formatter& {
	fmt::print("asdfasdfasd\n");
	return out << fmt::format("{}", x);
}

auto operator<<(Ab::Sexpr::Formatter& out, int x) -> Ab::Sexpr::Formatter& {
	return out << std::to_string(x);
}

auto operator<<(Ab::Sexpr::Formatter& out, unsigned int x) -> Ab::Sexpr::Formatter& {
	return out << std::to_string(x);
}


auto operator<<(Ab::Sexpr::Formatter& out, unsigned long x) -> Ab::Sexpr::Formatter& {
	fmt::print("xxxxxxxxxxxxxxx\n");
	return out << std::to_string(x);
}

auto operator<<(Ab::Sexpr::Formatter& out, unsigned long long x) -> Ab::Sexpr::Formatter& {
	fmt::print("thtergergergergerg\n");
	return out << std::to_string(x);
}

auto operator<<(Ab::Sexpr::Formatter& out, unsigned char x) -> Ab::Sexpr::Formatter& {
	fmt::print("asdfasdfasd\n");
	return out << fmt::format("{}", x);
}

#endif  // AB_SEXPR_HPP_
