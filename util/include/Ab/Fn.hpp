#ifndef AB_FN_HPP_
#define AB_FN_HPP_

namespace Ab {

#include <cstddef>
#include <memory>
#include <new>
#include <type_traits>
#include <utility>

template <typename R, typename... Args>
using InvokeFn = R (*)(const void* fn, Args... args);

using DestructFn = void (*)(void* interface) noexcept;

template <typename F>
class FnRef;

template <typename R, typename... Args>
class FnRef<R(Args...)> {
public:
	FnRef(void* data, InvokeFn<R, Args...> invoke) : data_(data), invoke_(invoke) {}

	R invoke(Args... args) const { return invoke_(data_, std::forward<Args>(args)...); }

	R operator()(Args... args) const { return invoke_(data_, std::forward<Args>(args)...); }

private:
	void* data_;
	InvokeFn<R, Args...> invoke_;
};

template <typename R, typename... Args>
struct FnTable {
	InvokeFn<R, Args...> invoke;
	DestructFn destruct;
};

template <typename R, typename... Args>
class FnInterface {
public:
	~FnInterface() noexcept { table_->destruct(this); }

	R invoke(Args... args) const { return table_->invoke(this, std::forward<Args>(args)...); }

protected:
	constexpr FnInterface(const FnTable<R, Args...>* table) : table_(table) {}

private:
	const FnTable<R, Args...>* table_;
};

template <typename T, typename R, typename... Args>
class FnStorage : public FnInterface<R, Args...> {
public:
	FnStorage(const T& value) : FnInterface<R, Args...>(&TABLE), value_(value) {}

	FnStorage(T&& value) : FnInterface<R, Args...>(&TABLE), value_(value) {}

	~FnStorage() = default;

private:
	static R do_invoke(const void* interface, Args... args) {
		return static_cast<const FnStorage*>(interface)->value_(
			std::forward<Args>(args)...);
	}

	static void do_destruct(void* interface) {
		return static_cast<FnStorage*>(interface)->~FnStorage();
	}

	static constexpr FnTable<R, Args...> TABLE = {&do_invoke, &do_destruct};

	T value_;
};

template <typename F>
class Fn;

template <typename R, typename... Args>
class Fn<R(Args...)> {
public:
	template <typename T>
	Fn(const T& value) : interface_(new FnStorage<std::decay_t<T>, R, Args...>(value)) {}

	template <typename T>
	Fn(T&& value) : interface_(new FnStorage<std::decay_t<T>, R, Args...>(std::move(value))) {}

	~Fn() = default;

	R invoke(Args... args) const { return interface_->invoke(std::forward<Args>(args)...); }

	R operator()(Args... args) const { return interface_->invoke(std::forward<Args>(args)...); }

private:
	std::unique_ptr<FnInterface<R, Args...>> interface_;
};

template <typename F>
class SmallFn;

template <typename R, typename... Args>
class SmallFn<R(Args...)> {
public:
	static constexpr std::size_t STORAGE_SIZE = 4 * sizeof(void*);

	template <typename T>
	SmallFn(const T& value) {
		static_assert(sizeof(FnStorage<T, R, Args...>) <= STORAGE_SIZE);
		new (cast_storage<void>()) FnStorage<T, R(Args...)>(value);
	}

	template <typename T>
	SmallFn(T&& value) {
		static_assert(sizeof(FnStorage<T, R, Args...>) <= STORAGE_SIZE);
		new (cast_storage<void>()) FnStorage<T, R, Args...>(std::move(value));
	}

	~SmallFn() = default;

	R invoke(Args... args) const {
		return get_interface()->invoke(std::forward<Args>(args)...);
	}

	R operator()(Args... args) const {
		return get_interface()->invoke(std::forward<Args>(args)...);
	}

private:
	template <typename T>
	T* cast_storage() noexcept {
		return reinterpret_cast<T*>(storage_);
	}

	template <typename T>
	const T* cast_storage() const noexcept {
		return reinterpret_cast<const T*>(storage_);
	}

	const FnInterface<R, Args...>* get_interface() const noexcept {
		return cast_storage<FnInterface<R, Args...>>();
	}

	char storage_[STORAGE_SIZE];
};

}  // namespace Ab

#endif  // AB_FN_HPP_
