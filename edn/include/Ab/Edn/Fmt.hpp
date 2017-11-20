#ifndef AB_EDN_FMT_HPP_
#define AB_EDN_FMT_HPP_

#include <Ab/Edn/Ir.hpp>
#include <fmt/core.h>

namespace Ab::Edn {

/// Configuration for pretty-printing.
struct PPrintCfg {
	/// 
	std::size_t margin = 72;

	/// Width threshold where objects are formatted in the horizontally compact "miser style".
	std::size_t miser_width = 40;

	/// 
	bool print_metadata;
};

namespace Internal {

struct PPrintState {
	PPrintCfg& cfg;
};

} // namespace Internal

void pprint(const Node& node, const PPrintCfg& cfg) {
	if (cfg.print_meta && node->metadata()) {
		pprint_metadata(node);
	}
}

void pprint(const Node& node) {
	pprint(node, PPrintCfg());
}

} // namespace Ab::Edn

#endif // AB_EDN_FMT_HPP_
