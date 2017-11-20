#ifndef AB_COMBINATORS_HPP_
#define AB_COMBINATORS_HPP_

namespace Ab {

template <typename... Fs>
struct Sequence {
	template <typename... Args>
	operator(Args&& args) {
		bool ok = true;
		Fs... f;
		f(std::forward<Args>(args)...)...;
	};
};

template <typename T>
struct Many {
	bool ok 
	do {

	}
};

template <typename... Fs>
struct Any {
	std::tuple<Fs> fs;
	template <typename... Args>
	operator()(Args&&... args) {
		if (std::get<Fs>(fs)(std::forward<Args>(args)...)) {
			return true;
		}
		
	}
};

} // namespace Ab

#endif // AB_COMBINATORS_HPP_

˜
