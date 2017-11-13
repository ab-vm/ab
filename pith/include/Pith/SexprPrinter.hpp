#ifndef PITH_SEXPRPRINTER_HPP_
#define PITH_SEXPRPRINTER_HPP_

#include <Pith/Config.hpp>
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
	static const constexpr std::size_t MAX_DEPTH = 10;
	std::size_t depth_{0};
};

/// A formatted printer.
/// Prints S-Expressions, ie (output (style lisp)).
class SexprPrinter {
public:
	SexprPrinter(std::ostream& out);

	/// Obtain the underlying stream object for unformatted output.
	inline auto stream() const -> std::ostream&;

	/// Obtain the indentation state.
	/// To increase the indentation, try:
	/// printer.indent()++;
	inline auto indent() -> Indent&;

	inline auto indent() const -> const Indent&;

	/// Prints a newline, if the current line is dirty.
	inline auto freshen() -> SexprPrinter&;

	/// Prints spacing between elements, or indentation if the line is fresh.
	inline auto spacing() -> SexprPrinter&;

	/// True if the line is dirty
	inline auto needsSpacing() const -> bool;

	/// Manually set the needs spacing flag.
	inline auto needsSpacing(bool p) -> SexprPrinter&;

	/// True if output is currently at the beginning of a line.
	inline auto fresh() const -> bool;

	/// Manually set the freshness, if you've been doing raw output.
	inline auto fresh(bool p) -> SexprPrinter&;

	/// Manually mark the line as dirty, needsSpacing.
	inline auto dirty() -> SexprPrinter&;

protected:
	std::ostream& stream_;
	Indent indent_{0};
	bool fresh_        = true;   // line is new
	bool needsSpacing_ = false;  // needs space between elements.
};

/// A debug s-expression printer.
extern SexprPrinter debug_out;

/// Print anything to the s-expression stream.
/// The type T must be streamable to std::ostream.
template <typename T>
inline auto operator<<(SexprPrinter& out, T&& x) -> SexprPrinter&;

/// Forward a manipulator to the underlying ostream.
inline auto operator<<(SexprPrinter& out, std::ostream& (*manipulator)(std::ostream&))
	-> SexprPrinter&;

/// Fresh line manipulator.
struct FreshLine {};

extern const FreshLine freshLine;

/// Begin a fresh line. If the current line is already fresh, do nothing.
inline auto operator<<(SexprPrinter& out, Pith::FreshLine) -> Pith::SexprPrinter& {
	out.freshen();
	return out;
}

/// S-Expression start manipulator.
struct SexprStart {};

extern const SexprStart sexprStart;

/// Begin an s-expression. Prints "(".
inline auto operator<<(SexprPrinter& out, SexprStart) -> SexprPrinter&;

/// S-Expression end manipulator.
struct SexprEnd {};

extern const SexprEnd sexprEnd;

/// End an s-expression. Prints ")".
inline auto operator<<(SexprPrinter& out, SexprEnd) -> SexprPrinter&;

/// Stringify the next thing. A manipulator that will wrap the text of the next object in quotes.
/// This manipulator only affects the one next object. Usage:
///   sexpr << stringify << "abcd";
/// output:
///   "abcd"
/// When you output a Stringify object to an SexprPrinter, the printer becomes a StringifyPrinter. The stringify printer will stream
struct Stringify {};

extern const Stringify stringify;

/// A printer that stringifies one thing, then reverts back to a SexprPrinter.
struct StringifyPrinter {
	SexprPrinter& sexpr;
};

/// Convert the SexprPrinter to a Stringify printer.
inline auto operator<<(SexprPrinter& out, Stringify) -> StringifyPrinter;

/// Print the stringified version of x. Returns the SexprPrinter, so all following operations will
/// operate as normal.
template <typename T>
inline auto operator<<(const StringifyPrinter& out, T&& x) -> SexprPrinter&;

/// A printer that bypasses the SexprPrinter's formatting. To start raw output, stream rawStart to
/// an SexprPrinter. To end raw output, stream rawEnd;
struct RawPrinter {
	SexprPrinter& sexpr;
};

/// Print x without any SexprPrinter-based formatting.
template <typename T>
inline auto operator<<(const RawPrinter& out, T&& x) -> const RawPrinter&;
struct RawStart {};

/// Convert an SexprPrinter to a RawPrinter.
inline auto operator<<(SexprPrinter& out, const RawStart&) -> RawPrinter;

extern const RawStart rawStart;

struct RawEnd {};

extern const RawEnd rawEnd;

/// convert a RawPrinter to a SexprPrinter.
/// The SexprPrinter is dirtied.
inline auto operator<<(const RawPrinter& out, const RawEnd&) -> SexprPrinter&;

}  // namespace Pith

#include <Pith/SexprPrinter.inl.hpp>

#endif  // PITH_SEXPRPRINTERHPP_