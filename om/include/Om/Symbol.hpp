#ifndef OM_SYMBOL_HPP_
#define OM_SYMBOL_HPP_

#include <Om/Cell.hpp>
#include <Om/Ref.hpp>
#include <Om/String.hpp>

namespace Om {

class Symbol : public Cell {
public:
	Ref<const String> string_;
};

}  // namespace Om

#endif  // OM_SYMBOL_HPP_
