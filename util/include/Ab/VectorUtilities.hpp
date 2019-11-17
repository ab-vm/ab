#ifndef AB_VECTORUTILITIES_HPP_
#define AB_VECTORUTILITIES_HPP_

#include <vector>

namespace Ab {

/// Emplace an element onto the back of the vector, returning a reference to the new element.
///
template <typename T, typename A, typename... Args>
T& push(std::vector<T, A>& vector, Args&&... args) {
	vector.emplace_back(std::forward<Args>(args)...);
	return vector[vector.size() - 1];
}

} // namespace Ab

#endif // AB_VECTORUTILITIES_HPP_
