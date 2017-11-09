#include <Om/Config.hpp>
#include <Om/Version.hpp>

namespace Om {

const unsigned int Version::MAJOR = OM_VERSION_MAJOR;
const unsigned int Version::MINOR = OM_VERSION_MINOR;
const unsigned int Version::PATCH = OM_VERSION_PATCH;
const char Version::STRING[]      = OM_VERSION;
const char Version::COMMIT[]      = OM_COMMIT;

}  // namespace Om
