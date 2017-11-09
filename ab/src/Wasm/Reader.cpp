#include <Ab/Wasm/Binary/Reader.hpp>

namespace Ab {
namespace Wasm {
namespace Binary {

auto NoOpVisitor::moduleStart() -> void {
}

auto NoOpVisitor::moduleEnd() -> void {
}

auto NoOpVisitor::header() -> void {
}

auto NoOpVisitor::sectionStart(SectionCode code) -> void {
}

auto NoOpVisitor::sectionEnd(SectionCode code) -> void {
}

auto NoOpVisitor::typeSection(std::size_t count) -> void {
}

auto NoOpVisitor::typeEntry() -> void {
}

auto NoOpVisitor::importSection(std::size_t count) -> void {
}

auto NoOpVisitor::importEntry() -> void {
}

}  // namespace Binary
}  // namespace Wasm
}  // namespace Ab
