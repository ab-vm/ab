#ifndef AB_TYPETRAITS_HPP_
#define AB_TYPETRAITS_HPP_

#include <type_traits>

namespace Ab {

template <std::size_t v>
using size_constant = std::integral_constant<std::size_t, v>;

template <typename T>
struct type_alias {
	using type = T;
};

template <typename T, T... Xs>
struct sum_impl {
	static constexpr T value = (0 + ... + Xs);
};

/// Given a static sequence of integers, define the sum as a static constant.
///
template <typename T, T... Xs>
struct sum : public std::integral_constant<T, sum_impl<T, Xs...>::value> {};

template <typename T, T... Xs>
constexpr T sum_v = sum<T, Xs...>::value;

/// Given a static sequence of sizes, define the sum as a static constant.
///
template <std::size_t... Xs>
using sum_sizes = sum<std::size_t, Xs...>;

template <std::size_t... Xs>
constexpr std::size_t sum_sizes_v = sum_sizes<Xs...>::value;

/// If N and I match, define the type T.
///
template <std::size_t N, std::size_t I, typename T>
struct match_nth_type {};

template <std::size_t N, typename T>
struct match_nth_type<N, N, T> : public type_alias<T> {};

template <std::size_t N, std::size_t I, typename T>
using match_nth_type_t = typename match_nth_type<N, I, T>::type;

template <std::size_t N, typename I, typename... Ts>
struct nth_type_impl;

template <std::size_t N, std::size_t... Is, typename... Ts>
struct nth_type_impl<N, std::index_sequence<Is...>, Ts...> : public match_nth_type<N, Is, Ts>... {};

/// Extract the Nth type from the type sequence Ts.
///
template <std::size_t N, typename... Ts>
struct nth_type : public nth_type_impl<N, std::index_sequence_for<Ts...>, Ts...> {};

template <std::size_t N, typename... Ts>
using nth_type_t = typename nth_type<N, Ts...>::type;

template <typename... Ts>
struct type_sequence {};

template <std::size_t N, typename T>
struct element_type;

template <std::size_t N, typename... Ts>
struct element_type<N, type_sequence<Ts...>> : nth_type<N, Ts...> {};

template <std::size_t N, typename T>
using element_type_t = typename element_type<N, T>::type;

}  // namespace Ab

#endif  // AB_TYPETRAITS_HPP_
