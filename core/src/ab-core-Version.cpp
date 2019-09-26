#include <Ab/Config.hpp>
#include <Ab/Version.hpp>

namespace Ab {

const unsigned int Version::MAJOR = AB_VERSION_MAJOR;
const unsigned int Version::MINOR = AB_VERSION_MINOR;
const unsigned int Version::PATCH = AB_VERSION_PATCH;
const char Version::STRING[]      = AB_VERSION;
const char Version::COMMIT[]      = AB_COMMIT;
const char Version::ABIGAIL[] =
	"  /__/'\n"
	">|'.' <\n"
	" `---'";

}  // namespace Ab
