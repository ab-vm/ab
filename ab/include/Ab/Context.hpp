#ifndef AB_CONTEXT_HPP_
#define AB_CONTEXT_HPP_

#include <Ab/Config.hpp>
#include <Om/ActiveContext.hpp>
#include <Om/Context.hpp>

namespace Ab {

class Context : public Om::Context {};

class ActiveContext : public Om::ActiveContext {};

} // namespace Ab

#endif // AB_CONTEXT_HPP_
