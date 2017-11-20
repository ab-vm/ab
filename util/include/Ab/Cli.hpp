#ifndef AB_CLI_HPP_
#define AB_CLI_HPP_

#include <Ab/Config.hpp>
#include <Ab/StringSpan.hpp>

namespace Ab {
namespace Cli {

struct ReadState {
  char** argv;
  int argc;
  int index;
};
       
template <typename T>
ArgTraits {
  bind
};

struct NoArg {
  
};

struct StringArg {
  template <typename F>
  void bind(F& f) {}
  
  using ValueType = std::string_view;
};

class OptionBase {
  /// returns true if successful
  virtual bool handle(const char* p) = 0;
};

                      
template <typename ArgType>
class Option {
  Option(std::function<ArgTraits<ArgType>::value_type)
};

template <LongOption>
OptionParser {
  
};

template <typename... Parsers>
struct Any {
};

template <

} // namespace Cli
} // namespace Ab

using OptionParser = Cli::Any<
  Cli::Any<Cli::Flag<

std::string_view()

ShortOptionReader<>

LongOptionReader<>

template <typename T>
struct Reader<LongOption O<T>> {

};

template <typename T>
struct Reader<ShortOption O<T>> {
	operator()()
};

template s
template <typename... Options>
struct LongOptionReader<OptionSet<Options>> {
	AnyOf<Options...>
}

template

struct {
	Read(ReadState& in, )
}


Error: while parsing arg 3: unrecognized short option "p", in
  b9run -1 -2 -qpp
              ~~~^
