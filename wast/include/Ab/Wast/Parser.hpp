#if !defined(AB_WAST_PARSER_HPP_)
#define AB_WAST_PARSER_HPP_

#include <cstddef>

namespace Ab {

int parse(const char* p, std::size_t n, bool eof = true);

class Parser {
public:
	Parser();

	void operator()(const char* p, std::size_t n, bool isEof = true);

private:
};

}  // namespace Ab

#endif // AB_WAST_PARSER_HPP_

