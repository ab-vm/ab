#include <Pith/Config.hpp>
#include <Pith/Version.hpp>

namespace Pith {

const unsigned int Version::MAJOR = PITH_VERSION_MAJOR;
const unsigned int Version::MINOR = PITH_VERSION_MINOR;
const unsigned int Version::PATCH = PITH_VERSION_PATCH;
const char Version::STRING[] = PITH_VERSION;
const char Version::COMMIT[] = PITH_COMMIT;

}  // namespace Pith
