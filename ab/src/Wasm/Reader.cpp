#include <Ab/Wasm/Binary/Reader.hpp>

namespace Ab {
namespace Wasm {
namespace Binary {

auto NoOpVisitor::moduleStart() -> void {
}

auto NoOpVisitor::moduleEnd() -> void {
}

auto NoOpVisitor::header(std::uint32_t magic, std::uint32_t version) -> void {
}

auto NoOpVisitor::sectionStart(const Section& section) -> void {
}

auto NoOpVisitor::sectionEnd(const Section& section) -> void {
}

auto NoOpVisitor::typeSection(std::size_t count) -> void {
}

auto NoOpVisitor::functionType(const FunctionType& type) -> void {
}

auto NoOpVisitor::importSection(std::size_t count) -> void {
}

auto NoOpVisitor::importEntry(const ImportEntry& entry) -> void {
}

auto NoOpVisitor::functionSection(std::size_t count) -> void {
}

auto NoOpVisitor::functionEntry(std::size_t i, std::uint32_t type) -> void {
}

}  // namespace Binary
}  // namespace Wasm
}  // namespace Ab
