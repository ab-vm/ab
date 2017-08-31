#include <Ab/Config.hpp>
#include <Ab/Context.hpp>
#include <Ab/Wasm/Loader.hpp>
#include <Pith/Assert.hpp>
#include <istream>

namespace Ab {
namespace Wasm {

auto Loader::operator()(ActiveContext& cx, std::istream& in)
	-> Pith::Result<Om::Ref<Module>, LoaderError> {
	PITH_ASSERT_UNREACHABLE();
	return Pith::err(LoaderError{});
}

}  // namespace Wasm
}  // namespace Ab
