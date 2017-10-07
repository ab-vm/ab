#if !defined(OM_VALUE_HPP_)
#define OM_VALUE_HPP_

#if 0

#include <Om/Config.hpp>
#include <Pith/Address.hpp>
#include <cstdint>

namespace Om {

/// A single-slot value. Pointer width.
class Value {
public:
	constexpr inline Value();

	constexpr inline Value(Pith::Address addr);

	/// Cast this to Type.
	template <typename Type>
	constexpr auto to() const -> Type const;

private:
	std::uintptr_t value_{0};
};

static_assert(sizeof(Value) == sizeof(Pith::Address), "Value must be pointer-width");

}  // namespace Om

#include <Om/Value.inl.hpp>

#endif  // 0

#endif  // OM_VALUE_HPP_