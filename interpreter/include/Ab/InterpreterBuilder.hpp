
#if !defined(AB_INTERPRETER_INTERPRETERBUILDER_HPP_)
#define AB_INTERPRETER_INTERPRETERBUILDER_HPP_

#include <MethodBuilder.hpp>

namespace Ab {

class InterpreterBuilder : public TR::MethodBuilder {
private:
};


class WasmInterpreterBuilder : public InterpreterBuilder {

};

}  // namespace InterpreterBuilder

#endif // AB_INTERPRETER_INTERPRETERBUILDER_HPP_
