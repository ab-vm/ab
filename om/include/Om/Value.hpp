#if !defined(OM_VALUE_HPP_)
#define OM_VALUE_HPP_

#include <cstdint>

namespace Om {

/// A single-slot value. Pointer width.
class Value {
public:
	constexpr inline Value();

	/// Construct
	template <typename Type> constexpr inline Value(const Type& x);

	/// Cast this to Type.
	template <typename Type> constexpr Type to() const;

private:
	std::uintptr_t value_{0};
};

static_assert(sizeof Value == sizeof Pith::Address, "Value must be pointer-width");

//
// Implementation
//

constexpr inline Value::Value() : value_{0} {
}

/// TODO: Assert Type is convertible to uintptr_t
template <typename Type>
constexpr inline Value::Value(const Type& x)
	: value_{(std::uintptr_t)x} {
		  // static_assert(sizeof(Type) == sizeof(std::uintptr_t), "Values must be
		  // pointer-width.");
	  };

/// TODO: Assert uintptr_t is convertible to Type
template <typename Type> constexpr inline Type Value::to() const {
	// static_assert(sizeof(Type) == sizeof(std::uintptr_t), "Values are pointer-width.");
	return static_cast<Type>(value_);
}

}  // namespace Om

#endif  // OM_VALUE_HPP_