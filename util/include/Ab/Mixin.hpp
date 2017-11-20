#ifndef AB_MIXIN_HPP_
#define AB_MIXIN_HPP_

#include <Ab/StaticPoly.hpp>

namespace Ab {

/// A mixin adds behaviour or functionality to another type. Mixins are injected
/// into a class through inheritance. The target class gaining functionality is
/// called the derived class, D. The derived class is always the first template
/// parameter of a mixin. A mixin uses static polymorphism to access the derived
/// class.
///
/// Mixin is itself a mixin, adding static-polymorphism functionality to other
/// mixin types.
///
/// Mixin has no API requirements on D.
///
/// Mixin defines the following API:
///   private:
///     friend class D;
///     U* self();
///     const U* self() const;
///     const U* cself() const;
///
/// @tparam D The deriving type, a kind of mixin.
/// @tparam U The ultimately deriving type, the deriving type of the mixin.
///
template <typename D, typename U>
class Mixin : public StaticPoly<D, U> {};

}  // namespace Ab

#endif  // AB_MIXIN_HPP_
