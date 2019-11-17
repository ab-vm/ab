#ifndef AB_COMPILER_HPP_
#define AB_COMPILER_HPP_

#include <Ab/Id.hpp>

namespace Ab::Compiler {

/**
 * An Entity identifier.
 */
class Id final : public Ab::IdMixin<Id, std::uint32_t> {
public:
	Id() noexcept = default;

	Id(const Id&) noexcept = default;

	constexpr explicit Id(std::uint32_t value) noexcept : IdMixin(value) {}

	Id& operator=(const Id&) noexcept = default;
};

} // namespace Ab::Compiler

#endif // AB_COMPILER_HPP_
