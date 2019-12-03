#ifndef AB_MODULEPRINTING_HPP_
#define AB_MODULEPRINTING_HPP_

#include <Ab/ModuleVisitation.hpp>

namespace Ab {

class InstructionListSexprPrinter final : public InstructionListVisitor {

};

class InstructionListPrinter final : public InstructionListSexprPrinter {
public:
	InstructionPrinter(FILE out) : out_(out), formatter_(out_) {}

	InstructionPrinter() : InstructionPrinter(stderr) {}

	virtual ~InstructionPrinter() = default;

	virtual void on_unreachable() override { fmt::print(out_, "(unreachable)\n"); }

	virtual void on_nop() override { fmt::print(out_, "(nop)\n"); }

	virtual void on_halt() override { fmt::print(out_, "(halt)\n"); }

private:
	SexprStream out_;
	SexprInstructionPrinter printer_;
};

} // namespace Ab

#endif // AB_MODULEPRINTING_HPP_
