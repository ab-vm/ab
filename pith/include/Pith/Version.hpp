#ifndef PITH_VERSION_HPP_
#define PITH_VERSION_HPP_

#include <Pith/Config.hpp>

namespace Pith {

class Version {
public:
	static const unsigned int MAJOR;
	static const unsigned int MINOR;
	static const unsigned int PATCH;
	static const char STRING[];
	static const char COMMIT[];
};

} // namespace Pith

#endif // PITH_VERSION_HPP_
