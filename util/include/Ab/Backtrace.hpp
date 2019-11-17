#ifndef AB_BACKTRACE_HPP_
#define AB_BACKTRACE_HPP_

namespace Ab {

/// TODO: Implement native backtraces

#if 0

char* abi::__cxa_demangle	(	const char * 	mangled_name,
char * 	output_buffer,
size_t * 	length,
int * 	status	 
)

#endif // 0

demangle(const char* mangled_name) {
	return abi::__cxa_demangle(mangled_name, output_buffer, length, status);
}

} // namespace Ab

#endif // AB_BACKTRACE_HPP_
