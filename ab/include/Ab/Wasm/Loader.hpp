#ifndef AB_WASM_LOADER_HPP_
#define AB_WASM_LOADER_HPP_

#include <Ab/Config.hpp>
#include <Ab/Context.hpp>
#include <Ab/Module.hpp>
#include <Om/Ref.hpp>
#include <Pith/Result.hpp>

namespace Ab {
namespace Wasm {

enum class LoaderError { UNKNOWN_ERROR };

class Loader {
public:
	/// Load a module from a binary stream
	auto operator()(ActiveContext& cx, std::istream& in)
		-> Pith::Result<Om::Ref<Module>, LoaderError>;
};

}  // namespace Wasm
}  // namespace Ab

#endif  // AB_WASM_LOADER_HPP_
