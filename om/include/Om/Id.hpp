#ifndef OM_ID_HPP_
#define OM_ID_HPP_

#include <Om/Config.hpp>
#include <Om/Cell.hpp>
#include <cstdint>

namespace Om {

/// A unique identifier for any GC-managed Cell object.
/// Safe to hold across GCs. Not safe to hold across some Map mutations which change the Id.

using RawId = std::uintptr_t;

class Id {
public:

	inline constexpr auto value() const -> RawId {
		return value_;
	}

	inline constexpr bool operator==(const Id& other) const;
	inline constexpr bool operator!=(const Id& other) const;

protected:
	inline constexpr explicit Id(RawId value);

	friend class Cell;

private:
	RawId value_;
};

//
// Implementation
//

inline constexpr bool Id::operator==(const Id& other) const {
	return value_ == other.value_;
}

inline constexpr bool Id::operator!=(const Id& other) const {
	return value_ != other.value_;
}

inline constexpr Id::Id(RawId value) : value_{value} {
}

}  // namespace Om

#endif  // OM_ID_HPP_