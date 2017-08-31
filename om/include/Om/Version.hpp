#ifndef OM_VERSION_HPP_
#define OM_VERSION_HPP_

#include <Om/Config.hpp>

namespace Om {

class Version {
public:
	static const unsigned int MAJOR;
	static const unsigned int MINOR;
	static const unsigned int PATCH;
	static const char STRING[];
	static const char COMMIT[];
};

}  // namespace Om

#endif  // OM_VERSION_HPP_
