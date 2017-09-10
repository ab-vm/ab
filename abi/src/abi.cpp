#include <Ab/Version.hpp>
#include <Ab/Process.hpp>
#include <Pith/Assert.hpp>
#include <iostream>

extern "C" auto main([[gnu::unused]] int argc, [[gnu::unused]] char** argv) -> int {
	PITH_ASSERT(Ab::Process::init() == Ab::ProcessError::SUCCESS);

	std::cout << Ab::Version::ABIGAIL << std::endl;

	PITH_ASSERT(Ab::Process::kill() == Ab::ProcessError::SUCCESS);
	return 0;
};
