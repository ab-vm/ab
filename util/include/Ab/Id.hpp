#ifndef AB_ID_HPP_
#define AB_ID_HPP_

#include <Ab/Mixin.hpp>
#include <Ab/HashUtilities.hpp>
#include <cstdint>

namespace Ab {

/// A mixin for defining IDs.
/// An ID is a  typesafe integer values with equality operators.
/// For sorted sets, ids are ordered. \
///
template <typename D, typename V = std::uintptr_t>
class IdMixin : public Mixin<IdMixin<D, V>, D> {
public:
	constexpr bool operator==(const D& rhs) const { return value_ == rhs.value_; }

	constexpr bool operator!=(const D& rhs) const { return value_ != rhs.value_; }

	constexpr bool operator<(const D& rhs) const { return value_ < rhs.value_; }

	constexpr bool operator>(const D& rhs) const { return value_ > rhs.value_; }

	constexpr bool operator<=(const D& rhs) const { return value_ <= rhs.value_; }

	constexpr bool operator>=(const D& rhs) const { return value_ >= rhs.value_; }

	constexpr V value() const { return value_; }

	constexpr std::size_t hash() const noexcept { return Ab::hash(value_); }

protected:
	IdMixin() noexcept = default;

	constexpr IdMixin(const IdMixin&) noexcept = default;

	constexpr explicit IdMixin(V value) : value_(value) {}

private:
	V value_;
};

/// General ID type.
///
class Id : public IDMixin<ID> {
public:
	Id() : IDMixin() {}

	constexpr explicit Id(std::size_t value) : IdMixin(value) {}
};

/// Small ID type. General purpose.
///
class Sid : public IdMixin<Sid, std::uint32_t> {
public:
	Sid() : IdMixin() {}

	constexpr explicit Sid(std::uint32_t value) : IdMixin(value) {}
};

} // namespace Ab

#endif // AB_ID_HPP_
