#ifndef AB_VERSION_HPP_
#define AB_VERSION_HPP_

#include <Ab/Config.hpp>

namespace Ab {

class Version {
public:
	static const unsigned int MAJOR;
	static const unsigned int MINOR;
	static const unsigned int PATCH;
	static const char STRING[];
	static const char COMMIT[];
	static const char ABIGAIL[];
};

} // namespace Ab

#endif // AB_VERSION_HPP_
