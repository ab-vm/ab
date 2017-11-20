#if !defined(AB_STATICPOLY_HPP_)
#define AB_STATICPOLY_HPP_

namespace Ab {

/// A class that can convert itself to an underlying type.
///
/// The template paramter D ensures no diamond inheritance problems occur.
///
/// @tparam D derived type. Only D may cast itself to U, using the private
///           self() call.
/// @tparam U underlying type. The type this object converts to.
///
template <typename D, typename U>
class StaticPoly {
private:
	friend D;

	/// Cast _this_ to the underlying type U.
	///
	U* self() noexcept { return static_cast<U*>(this); }

	const U* self() const noexcept { return static_cast<const U*>(this); }

	/// Cast _this_ to the underlying type const U.
	///
	const U* cself() const noexcept { return static_cast<const U*>(this); }

	U& self_ref() noexcept { return *self(); }

	const U& self_ref() const noexcept { return *self(); }

	const U& cself_ref() const noexcept { return *cself(); }
};

}  // namespace Ab

#endif // AB_STATICPOLY_HPP_
