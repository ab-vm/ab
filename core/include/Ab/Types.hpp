#ifndef AB_TYPES_HPP_
#define AB_TYPES_HPP_

#include <Ab/HashUtilities.hpp>
#include <Ab/ModuleConstants.hpp>
#include <array>
#include <vector>

namespace Ab {

template <ValType V>
struct ValTypeConst : public std::integral_constant<ValType, V> {};

template <typename T>
struct ToValType {};

template <typename T>
struct ToValType<const T> : public ToValType<T> {};

template <>
struct ToValType<std::uint64_t> : public ValTypeConst<ValType::I64> {};

template <>
struct ToValType<std::int64_t> : public ValTypeConst<ValType::I64> {};

template <>
struct ToValType<std::uint32_t> : public ValTypeConst<ValType::I32> {};

template <>
struct ToValType<std::int32_t> : public ValTypeConst<ValType::I32> {};

template <>
struct ToValType<float> : public ValTypeConst<ValType::F32> {};

template <>
struct ToValType<double> : public ValTypeConst<ValType::F64> {};

/// Convert a C-Type to an equivalent ValType.
///
template <typename T>
constexpr ValType VAL_TYPE = ToValType<T>::value;

/// convert a static sequence of c-types to a static array of vm-types.
///
template <typename... Ts>
constexpr std::array<ValType, sizeof...(Ts)> VAL_TYPE_ARRAY{VAL_TYPE<Ts>...};

/// Check that a static sequence of c-types matches a dynamic vector of vm-types.
/// The C-types are specified as type parameter pack, while the vm-types are
/// specified by a vector of ValTypes.
///
template <typename... Ts>
bool types_match(const std::vector<ValType>& vm_types) {
	constexpr auto C_TYPES = VAL_TYPE_ARRAY<Ts...>;
	if (C_TYPES.size() != vm_types.size()) {
		return false;
	}
	for (std::size_t i = 0; i < C_TYPES.size(); ++i) {
		if (C_TYPES[i] != vm_types[i]) {
			return false;
		}
	}
	return true;
}

struct Limits {
public:
	std::size_t hash() const noexcept {
		std::size_t h = Ab::hash(min);
		if (has_max) {
			h = Ab::mix_hash(min, max);
		}
		return h;
	}

	bool operator==(const Limits& rhs) const noexcept {
		if (min != rhs.min) {
			return false;
		}
		if (has_max != rhs.has_max) {
			return false;
		}
		if (has_max && max != rhs.max) {
			return false;
		}
		return true;
	}

	bool operator!=(const Limits& rhs) const noexcept { return !(*this == rhs); }

	std::uint32_t min;
	std::uint32_t max;
	bool has_max;
};

struct FuncType final : public Type {
public:
	virtual ~FuncType() noexcept = default;

	virtual TypeKind kind() const noexcept override { return TypeKind::FUNC; }

	virtual std::size_t hash() const noexcept override {
		std::size_t h = Ab::hash(args.size(), rets.size());
		for (auto ty : args) {
			h = mix_hash(h, ty);
		}
		for (auto ty : rets) {
			h = mix_hash(h, ty);
		}
		return h;
	}

	/// Deep comparison against an unknown kind of type.
	///
	virtual bool operator==(const Type& rhs) const noexcept override {
		if (rhs.kind() != TypeKind::FUNC) {
			return false;
		}
		return *this == rhs.as_func();
	}

	virtual bool operator!=(const Type& rhs) const noexcept override { return !(*this == rhs); }

	/// Deep comparison of two func types.
	///
	bool operator==(const FuncType& rhs) const noexcept {
		if (args.size() != rhs.args.size()) {
			return false;
		}
		if (rets.size() != rhs.rets.size()) {
			return false;
		}
		for (std::size_t i = 0; i < args.size(); ++i) {
			if (args[i] != rhs.args[i]) {
				return false;
			}
		}
		for (std::size_t i = 0; i < rets.size(); ++i) {
			if (rets[i] != rhs.rets[i]) {
				return false;
			}
		}
		return true;
	}

	bool operator!=(const FuncType& rhs) const noexcept { return !(*this == rhs); }

	/// The types of the arguments.
	///
	std::vector<ValType> args;

	/// The types of the return values. 1 or 0 values.
	///
	std::vector<ValType> rets;
};

inline FuncType& Type::as_func() noexcept {
	AB_ASSERT(kind() == TypeKind::FUNC);
	return *static_cast<FuncType*>(this);
}

inline const FuncType& Type::as_func() const noexcept {
	AB_ASSERT(kind() == TypeKind::FUNC);
	return *static_cast<const FuncType*>(this);
}

}  // namespace Ab

template <>
struct std::hash<Ab::Type> {
	std::size_t operator()(const Ab::Type& type) const noexcept { return type.hash(); }
};

template <>
struct std::hash<Ab::FuncType> {
	std::size_t operator()(const Ab::FuncType& type) const noexcept { return type.hash(); }
};

#endif  // AB_TYPES_HPP_
