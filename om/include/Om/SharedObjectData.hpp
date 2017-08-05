#ifndef OM_SHARED_OBJECT_DATA_HPP_
#define OM_SHARED_OBJECT_DATA_HPP_

namespace Om {

/// Common data shared by Objects and DataObjects
class SharedObjectData : Cell {
public:
	ObjectHeader header_;
	Klass* klass_;
	Prototype* prototype_;
};

}  // namespace Om

#endif  // OM_SHARED_OBJECT_DATA_HPP_