#ifndef AB_VECTORUTILITIES_HPP_
#define AB_VECTORUTILITIES_HPP_

#include <vector>

namespace Ab {

/// Push an element onto the back of the vector.
/// @returns a reference to the new element.
///
template <typename T, typename A, typename... Args>
T& push(std::vector<T, A>& vector, Args&&... args) {
	vector.emplace_back(std::forward<Args>(args)...);
	return vector.back();
}

/// Pop a value from the back of a vector, and return it.
///
template <typename T, typename A>
T pop(std::vector<T, A>& vector) {
	T value(std::move(vector.back()));
	vector.pop_back();
	return value;
}

} // namespace Ab

#endif // AB_VECTORUTILITIES_HPP_
