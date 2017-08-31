#include <Ab/Version.hpp>
#include <iostream>

#if 0
template <> bool init(Klass* k, Klass* parentKlass) {
	VirtualMachine vm;

	heap->allocate<Klass>();
	std::cout << "initializing klass" << std::endl;
	k->klass = parentKlass;
	return true;
}

bootstrap(VirtualMachine& vm) {
	auto cx = vm.access();
	auto mx = vm.memory.access();

	auto klassKlass = allocate<Klass>(mx);
	klass->klass = klass;
};

Shape {

};

BaseShape {

};


thread_entry() {
	Ab::Context cx;
	cx.init();

	cx.kill();
}

#endif

extern "C" auto main(int argc, char** argv) -> int {
	// Process wide init
	PITH_ASSERT_SUCCESS(Ab::Process::init());

	// Bring up our interpreter system
	Ab::System system;
	PITH_ASSERT_SUCCESS(system.init());

	// Create a thread context
	Ab::ContextState context(system);
	PITH_ASSERT_SUCCESS(context.init());

	Ab::ActiveContext cx{context};

	std::cout << Ab::Version::ABIGAIL << std::endl;

	// tear down in reverse order.
	PITH_ASSERT_SUCCESS(context.kill());
	PITH_ASSERT_SUCCESS(system.kill());
	PITH_ASSERT_SUCCESS(Ab::Process::kill());
	return 0;
};
