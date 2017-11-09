#ifndef AB_WASM_BINARY_VISITOR_HPP_
#define AB_WASM_BINARY_VISITOR_HPP_

#include <Ab/Wasm/Binary/SectionCode.hpp>
#include <cstdint>

namespace Ab {
namespace Wasm {
namespace Binary {

///  The Pure-virtual base class. See also: the No-Op base visitor.
class Visitor {
public:
	virtual ~Visitor() noexcept {};

	// Initialization

	virtual auto moduleStart() -> void = 0;

	virtual auto moduleEnd() -> void = 0;

	virtual auto header() -> void = 0;

	/// Finalization

	virtual auto sectionStart(SectionCode code) -> void = 0;

	virtual auto sectionEnd(SectionCode code) -> void = 0;

	/// Type Section

	virtual auto typeSection(std::size_t count) -> void = 0;

	virtual auto typeEntry() -> void = 0;

	/// Import Section

	virtual auto importSection(std::size_t count) -> void = 0;

	virtual auto importEntry() -> void = 0;
};

///  The Pure-virtual base class. See also: the No-Op base visitor.
class NoOpVisitor : public Visitor {
public:
	/// Initialization

	virtual auto moduleStart() -> void override;

	virtual auto moduleEnd() -> void override;

	virtual auto header() -> void override;

	/// Finalization

	virtual auto sectionStart(SectionCode code) -> void override;

	virtual auto sectionEnd(SectionCode code) -> void override;

	/// Type Section

	virtual auto typeSection(std::size_t count) -> void override;

	virtual auto typeEntry() -> void override;

	/// Import Section

	virtual auto importSection(std::size_t count) -> void override;

	virtual auto importEntry() -> void override;
};

}  // namespace Binary
}  // namespace Wasm
}  // namespace Ab

#endif  // AB_WASM_BINARY_VISITOR_HPP_
