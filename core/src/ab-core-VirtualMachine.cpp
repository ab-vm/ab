#include <Ab/Config.hpp>
#include <Ab/Assert.hpp>
#include <Ab/VirtualMachine.hpp>

namespace Ab {

Module* load_module(Context& cx, const char* filename) {
	(void)cx;
	(void)filename;
	AB_ASSERT_UNIMPLEMENTED();
}

Module* load_module(Context& cx, const std::string& filename) {
	(void)cx;
	(void)filename;
	AB_ASSERT_UNIMPLEMENTED();
}

void link_module(Context& cx, Module* module) {
	(void)cx;
	(void)module;
	AB_ASSERT_UNIMPLEMENTED();
}

ModuleInst* instantiate_file(Context& cx, const std::string& filename) {
	(void)cx;
	(void)filename;
	AB_ASSERT_UNIMPLEMENTED();
}

void run_func(Context& cx, FuncInst* func) {
	(void)cx;
	(void)func;
	AB_ASSERT_UNIMPLEMENTED();
	interpret(&cx.exec_state(), func);
}

}  // namespace Ab
