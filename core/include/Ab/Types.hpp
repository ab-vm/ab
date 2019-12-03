#ifndef AB_TYPES_HPP_
#define AB_TYPES_HPP_

#include <Ab/HashUtilities.hpp>
#include <Ab/ModuleConstants.hpp>
#include <Ab/TypeTraits.hpp>
#include <array>
#include <vector>

namespace Ab {

/// The size of a register, or stack slot, in byte. Stack slots are 32-bits.
///
constexpr std::size_t SIZEOF_SLOT = 4;

template <ValType V>
struct val_type_constant : public std::integral_constant<ValType, V> {};

/// Given a type T, convert it to it's equivalent ValType.
///
template <typename T>
struct to_val_type;

template <typename T>
struct to_val_type<const T> : public to_val_type<T> {};

template <>
struct to_val_type<std::uint64_t> : public val_type_constant<ValType::I64> {};

template <>
struct to_val_type<std::int64_t> : public val_type_constant<ValType::I64> {};

template <>
struct to_val_type<std::uint32_t> : public val_type_constant<ValType::I32> {};

template <>
struct to_val_type<std::int32_t> : public val_type_constant<ValType::I32> {};

template <>
struct to_val_type<float> : public val_type_constant<ValType::F32> {};

template <>
struct to_val_type<double> : public val_type_constant<ValType::F64> {};

template <typename T>
constexpr ValType to_val_type_v = to_val_type<T>::value;

template <typename... Ts>
constexpr std::array<ValType, sizeof...(Ts)> to_val_type_array_v = {to_val_type_v<Ts>...};

/// Given a ValType V, get the slots v consumes.
///
template <ValType V>
struct slots_taken;

template <>
struct slots_taken<ValType::I32> : public size_constant<1> {};

template <>
struct slots_taken<ValType::I64> : public size_constant<2> {};

template <>
struct slots_taken<ValType::F32> : public size_constant<1> {};

template <>
struct slots_taken<ValType::F64> : public size_constant<2> {};

template <ValType X>
constexpr std::size_t slots_taken_v = slots_taken<X>::value;

/// The number of slots consumed on the vm stack by the c-type T.
/// Given a type T, get the slots consumed by it's equivalent ValType.
///
template <typename T>
struct slot_sizeof : public slots_taken<to_val_type_v<T>> {};

template <typename T>
constexpr auto slot_sizeof_v = slot_sizeof<T>::value;

/// Given a sequence of types and a sequence of indices, count the stack slots consumed by the types
/// at the specified indices.
template <typename I, typename... Ts>
struct sum_slots_for;

template <std::size_t... Is, typename... Ts>
struct sum_slots_for<std::index_sequence<Is...>, Ts...>
	: public sum_sizes<slot_sizeof_v<nth_type_t<Is, Ts>>...> {};

template <typename I, typename... Ts>
constexpr std::size_t sum_slots_for_v = sum_slots_for<I, Ts...>::value;

/// Given a sequence of types Ts, count the stack slots consumed by the first N types.
///
template <std::size_t N, typename... Ts>
struct sum_slots_up_to : public sum_slots_for<std::make_index_sequence<N>, Ts...> {};

template <std::size_t N, typename... Ts>
constexpr auto sum_slots_up_to_v = sum_slots_up_to<N, Ts...>::value;

/// Given a sequence of types Ts, get the stack position of the Nth element.
///
template <std::size_t N, typename... Ts>
struct nth_slot_index : public sum_slots_up_to<N - 1, Ts...> {};

template <typename... Ts>
struct nth_slot_index<0, Ts...> : public size_constant<0> {};

template <std::size_t N, typename... Ts>
constexpr std::size_t nth_slot_index_v = nth_slot_index<N, Ts...>::value;

/// Given a tuple of type T, get the slots consumed by the Nth element.
///
template <std::size_t N, typename T>
constexpr std::size_t slots_taken_by_element_v = slot_sizeof_v<std::tuple_element_t<N, T>>;

/// Given a tuple of type T, get the stack position (in slots) for the Nth element of the tuple.
///
template <std::size_t N, typename T>
struct element_slot_index;

template <std::size_t N, typename... Ts>
struct element_slot_index<N, std::tuple<Ts...>> : public sum_slots_up_to<N - 1, Ts...> {};

template <typename... Ts>
struct element_slot_index<0, std::tuple<Ts...>> : public size_constant<0> {};

template <std::size_t N, typename T>
constexpr std::size_t element_slot_index_v = element_slot_index<N, T>::value;

/// The number of stack slots consumed by a sequence of vm-types.
///
template <ValType... Vs>
constexpr std::size_t slots_taken_by = sum_sizes_v<slots_taken_v<Vs>...>;

/// The number of slots consumed on the VM stack for a sequence of c-types.
///
template <typename... Ts>
constexpr std::size_t slots_taken_by_type = sum_sizes_v<slot_sizeof_v<Ts>...>;

/// Get the number of bytes consumed on the VM stack for the valtype sequence Vs.
///
template <ValType... Vs>
constexpr std::size_t bytes_taken_by = slots_taken_by<Vs...>* SIZEOF_SLOT;

/// Get the number of bytes consumed on the VM stack for the c-types sequence Ts.
///
template <typename... Ts>
constexpr std::size_t bytes_taken_by_type = slots_taken_by_type<Ts...>* SIZEOF_SLOT;

/// Number of registers taken by the value-type type.
///
constexpr std::size_t nreg(ValType type) noexcept {
	switch (type) {
	case ValType::I32:
		return slots_taken_by<ValType::I32>;
	case ValType::I64:
		return slots_taken_by<ValType::I64>;
	case ValType::F32:
		return slots_taken_by<ValType::F32>;
	case ValType::F64:
		return slots_taken_by<ValType::F64>;
	default:
		return SIZE_MAX;
	}
}

/// Check that a static sequence of c-types matches a dynamic vector of vm-types.
/// The C-types are specified as type parameter pack, while the vm-types are
/// specified by a vector of ValTypes.
///
template <typename... Ts>
bool types_match(const std::vector<ValType>& dyn_types) {
	constexpr auto fix_types = to_val_type_array_v<Ts...>;
	if (fix_types.size() != dyn_types.size()) {
		return false;
	}
	for (std::size_t i = 0; i < fix_types.size(); ++i) {
		if (fix_types[i] != dyn_types[i]) {
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

struct FuncType final {
public:
	std::size_t hash() const noexcept {
		std::size_t h = Ab::hash(args.size(), rets.size());
		for (auto ty : args) {
			h = mix_hash(h, ty);
		}
		for (auto ty : rets) {
			h = mix_hash(h, ty);
		}
		return h;
	}

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

}  // namespace Ab

template <>
struct std::hash<Ab::FuncType> {
	std::size_t operator()(const Ab::FuncType& type) const noexcept { return type.hash(); }
};

#endif  // AB_TYPES_HPP_
