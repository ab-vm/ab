#ifndef AB_BYTEBUFFER_HPP_
#define AB_BYTEBUFFER_HPP_

#include <Ab/Config.hpp>

#include <Ab/Bytes.hpp>

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <string.h>
#include <type_traits>

namespace Ab {

/// Byte buffer builder.
class ByteBuffer {
public:
	ByteBuffer() : ByteBuffer(std::int32_t(64)) {}

	ByteBuffer(std::int32_t capacity) :
		data_(new Byte[capacity]),
		size_(0),
		capa_(capacity) {}

	ByteBuffer(const ByteBuffer& other) :
		data_(new Byte[other.capacity()]),
		size_(other.size()),
		capa_(other.capacity()) {
		if (data_ != nullptr) {
			memcpy(data_, other.data(), size());
		}
	}

	ByteBuffer(ByteBuffer&& other)
		: data_(other.data()), size_(other.size()), capa_(other.capacity()) {
		other.data_ = nullptr;
		other.capa_ = 0;
		other.size_ = 0;
	}

	~ByteBuffer() {
		if (data_ != nullptr) {
			delete[] data_;
		}
		data_ = nullptr;
		size_ = 0;
		capa_ = 0;
	}

	void write(std::size_t offset, const Byte* src, std::int32_t nbytes) {
		assert(begin() + offset < end());
		memcpy(begin() + offset, src, nbytes);
	}

	/// hack: write any type into the byte stream.
	template <typename T>
	void write(std::size_t offset, T value) {
		write(offset, reinterpret_cast<const Byte*>(&value), sizeof(T));
	}

	/// copy bytes to end of stream, resizing if needed.
	void append(const Byte* src, std::size_t nbytes) {
		ensure(nbytes);
		memcpy(end(), src, nbytes);
		size_ += nbytes;
	}

	/// hack: append the bytes of any type to the byte stream.
	template <typename T>
	void append(T value) {
		append(reinterpret_cast<Byte*>(&value), sizeof(T));
	}

	/// Erase the data in this buffer
	void clear() {
		if (data_ != nullptr) {
			delete[] data_;
			data_ = nullptr;
			size_ = 0;
			capa_ = 0;
		}
	}

	/// release ownership of the underlying memory and reset this buffer.
	/// The returned array must be explicitly freed.
	Byte* release() {
		auto result = data_;
		data_       = nullptr;
		size_       = 0;
		capa_       = 0;
		return result;
	}

	Byte* data() { return data_; }

	const Byte* data() const { return data_; }

	Byte* begin() { return data_; }

	Byte* end() { return data_ + size_; }

	const Byte* begin() const { return data_; }

	const Byte* end() const { return data_ + size_; }

	const Byte* cbegin() const { return data_; }

	const Byte* cend() const { return data_ + size_; }

	std::int32_t size() const { return size_; }

	std::int32_t capacity() const { return capa_; }

	Byte& operator[](std::int32_t offset) { return data_[offset]; }

	const Byte& operator[](std::int32_t offset) const { return data_[offset]; }

	/// Convert offset to a T*,
	template <typename T>
	T& reinterpret(std::int32_t offset) {
		return *reinterpret_cast<T*>(data_ + offset);
	}

	/// convert an offset to a T*.
	template <typename T>
	const T& reinterpret(std::int32_t offset) const {
		return *reinterpret_cast<T*>(data_ + offset);
	}

private:
	/// ensure we have room for n more bytes.
	void ensure(std::size_t nbytes) {
		if (capa_ < size_ + nbytes) {
			grow();
		}
	}

	void grow() {
		if (capa_ == 0) {
			assert(data_ == nullptr);
			assert(size_ == 0);
			capa_ = 64;
			data_ = reinterpret_cast<Byte*>(malloc(capa_));
		} else {
			assert(data_ != nullptr);
			assert(size_ <= capa_);
			capa_ *= 2;
			data_ = reinterpret_cast<Byte*>(realloc(data_, capa_));
		}
	}

	Byte* data_;
	std::size_t size_;
	std::size_t capa_;  // capacity
};

}  // namespace Ab

#endif  // AB_BYTEBUFFER_HPP_
