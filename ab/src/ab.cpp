#include <Ab.hpp>
#include <cstdint>
#include <iostream>

namespace Ab {

template <>
bool init(Klass* k, Klass* parentKlass) {
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

Shape{

};

BaseShape{

};

}  // namespace Ab