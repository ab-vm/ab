#ifndef OM_ZEROOBJECTSLOTS_HPP_
#define OM_ZEROOBJECTSLOTS_HPP_

#include <Om/Config.hpp>
#include <Om/Object.hpp>

namespace Om {

class ZeroObjectSlots {
public:
	auto inline operator()(Ref<Object> object) -> void {
		for (auto && slot : object->slots()) {
			slot = nullptr;
		}
	}
};

} // namespace Om

#endif // OM_ZEROOBJECTSLOTS_HPP_
