#ifndef VIRTUALMACHINESTATE_INL_HPP_
#define VIRTUALMACHINESTATE_INL_HPP_

#include <Ab/VirtualMachineState.hpp>

namespace Ab {

auto VirtualMachineState Commit(TR::IlBuilder* b) -> void {
	PITH_TRACE();
}

inline virtual auto Reload(TR::IlBuilder* b) override -> void {
	PITH_TRACE();
}

inline virtual auto MakeCopy() override -> OMR::VirtualMachineState* {
	PITH_TRACE();
}

inline virtual auto MergeInto(OMR::VirtualMachineState* other, TR::IlBuilder* b) -> void {
	PITH_TRACE();
}

}  // namespace Ab

#endif  // VIRTUALMACHINESTATE_INL_HPP_
