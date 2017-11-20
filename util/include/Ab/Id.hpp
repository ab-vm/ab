#ifndef AB_ID_HPP_
#define AB_ID_HPP_

#include <Ab/Mixin.hpp>
#include <cstdint>

namespace Ab {

/// A mixin for defining IDs.
/// An ID is a  typesafe integer values with equality operators.
///
template <typename D, typename V = std::uintptr_t>
class IdMixin : public Mixin<IdMixin<D, V>, D> {
public:
	bool operator==(const D& rhs) const { return value_ == rhs.value_; }

	bool operator!=(const D& rhs) const { return value_ != rhs.value_; }

	V value() const { return value_; }

protected:
	IdMixin() : value_() {}

	explicit IdMixin(V value) : value_(value) {}

private:
	V value_;
};

/// General ID type.
///
class Id : public IDMixin<ID> {
public:
	Id() : IDMixin() {}

	explicit Id(std::size_t value) : IdMixin(value) {}
};

/// Small ID type. General purpose.
///
class Sid : public IdMixin<Sid, std::uint32_t> {
public:
	Sid() : IdMixin() {}

	explicit Sid(std::uint32_t value) : IdMixin(value) {}
};

} // namespace Ab

#endif // AB_ID_HPP_
