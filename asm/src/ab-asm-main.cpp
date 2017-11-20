#include <Ab/Commands.hpp>

static const char* USAGE = 
  "Usage: ab asm <input> -o <output>";

extern "C" int main(int argc, char** argv) {
	return Ab::cmd_asm(argc, argv);
}
