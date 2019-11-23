#ifndef AB_BYTEBUFFER_HPP_
#define AB_BYTEBUFFER_HPP_

#include <Ab/Config.hpp>
#include <Ab/Assert.hpp>
#include <Ab/Bytes.hpp>
#include <cstddef>
#include <cstring>
#include <type_traits>

namespace Ab {

/// Container holding a flat array of bytes.
/// A vector-like container that provides extra convieniences for copying
/// trivially copyable data, such as ints or structs, into the buffer.
///
class ByteBuffer {
public:
	static constexpr std::size_t DEFAULT_CAPACITY = 64;

	/// Construct a bytebuffer with a default capacity.
	///
	ByteBuffer() : ByteBuffer(DEFAULT_CAPACITY) {}

	/// Construct a bytebuffer with a known capacity.
	///
	ByteBuffer(std::size_t capacity) : data_(nullptr), size_(0), capa_(0) {
		data_ = reinterpret_cast<Byte*>(std::malloc(capacity));
		if (!data_) {
			throw std::bad_alloc();
		}
		capa_ = capacity;
	}

	ByteBuffer(const ByteBuffer& other) : data_(nullptr), size_(0), capa_(0) {
		data_ = reinterpret_cast<Byte*>(std::malloc(other.capacity()));
		if (!data_) {
			throw std::bad_alloc();
		}
		size_ = other.size();
		capa_ = other.capacity();
		std::memcpy(data_, other.data(), size_);
	}

	ByteBuffer(ByteBuffer&& other) : data_(nullptr), size_(0), capa_(0) {
		data_       = other.data();
		size_       = other.size();
		capa_       = other.capacity();
		other.data_ = nullptr;
		other.capa_ = 0;
		other.size_ = 0;
	}

	~ByteBuffer() noexcept {
		free(data_);
		data_ = nullptr;
		size_ = 0;
		capa_ = 0;
	}

	/// Write a range of bytes into the stream. Will not resize the buffer.
	///
	void write(std::size_t offset, const Byte* src, std::int32_t nbytes) noexcept {
		AB_ASSERT(offset + nbytes <= size_);
		std::memcpy(begin() + offset, src, nbytes);
	}

	/// Write any type into the byte stream. Will not resize the buffer.
	///
	template <typename T, typename = std::enable_if_t<std::is_trivial_v<T>>>
	void write(std::size_t offset, T value) noexcept {
		AB_ASSERT(offset + sizeof(T) <= size_);
		write(offset, reinterpret_cast<const Byte*>(&value), sizeof(T));
	}

	/// copy bytes to end of stream, resizing if needed.
	///
	void append(const Byte* src, std::size_t nbytes) {
		ensure_space(nbytes);
		std::memcpy(end(), src, nbytes);
		size_ += nbytes;
	}

	/// Append the bytes of any trivial type to the byte stream.
	///
	template <typename T, typename = std::enable_if_t<std::is_trivial_v<T>>>
	void append(T value) {
		append(reinterpret_cast<Byte*>(&value), sizeof(T));
	}

	/// Append the bytes of another buffer to the byte stream.
	///
	void append(const ByteBuffer& other) { append(other.data(), other.size()); }

	/// Erase the data in this buffer.
	///
	void clear() noexcept {
		std::free(data_);
		data_ = nullptr;
		size_ = 0;
		capa_ = 0;
	}

	/// Reinterpret the bytes at offset, as a value of type T.
	/// Returns a reference to the value.
	///
	template <typename T>
	T& reinterpret(std::size_t offset = 0) noexcept {
		static_assert(std::is_trivial_v<T>);
		AB_ASSERT(offset + sizeof(T) <= size_);
		return *reinterpret_cast<T*>(data_ + offset);
	}

	template <typename T>
	const T& reinterpret(std::size_t offset = 0) const noexcept {
		static_assert(std::is_trivial_v<T>);
		AB_ASSERT(offset + sizeof(T) <= size_);
		return *reinterpret_cast<T*>(data_ + offset);
	}

	/// Read a value out of the byte buffer.
	///
	template <typename T>
	const T read(std::size_t offset = 0) const noexcept {
		static_assert(std::is_trivial_v<T>);
		AB_ASSERT(offset + sizeof(T) <= size_);
		return reinterpret<T>(offset);
	}

	/// release ownership of the underlying memory and reset this buffer.
	/// The returned array must be explicitly freed.
	///
	Byte* release() noexcept {
		Byte* result = data_;
		data_        = nullptr;
		size_        = 0;
		capa_        = 0;
		return result;
	}

	std::span<Byte> release_span() noexcept {
		shrink_to_fit();

		std::span<Byte> result(data_, size_);

		data_ = nullptr;
		size_ = 0;
		capa_ = 0;

		return result;
	}

	Byte* data() noexcept { return data_; }

	const Byte* data() const noexcept { return data_; }

	Byte* begin() noexcept { return data_; }

	Byte* end() noexcept { return data_ + size_; }

	const Byte* begin() const noexcept { return data_; }

	const Byte* end() const noexcept { return data_ + size_; }

	const Byte* cbegin() const noexcept { return data_; }

	const Byte* cend() const noexcept { return data_ + size_; }

	std::size_t size() const noexcept { return size_; }

	std::size_t capacity() const noexcept { return capa_; }

	Byte& operator[](std::size_t offset) noexcept { return data_[offset]; }

	const Byte& operator[](std::size_t offset) const noexcept { return data_[offset]; }

	/// Resize the byte buffer. If growing, new memory is uninitialized.
	///
	void resize(std::size_t size) {
		resize_capacity(size);
		size_ = size;
	}

	/// Allocate more capacity.
	/// If the given capacity is smaller than the current capacity, do nothing.
	///
	void reserve(std::size_t capacity) {
		if (capa_ < capacity) {
			resize_capacity(capacity);
		}
	}

	/// Resize the capacity of the buffer to match the size.
	///
	void shrink_to_fit() { resize_capacity(size_); }

private:
	/// Ensure we have capacity for appending n more bytes.
	///
	void ensure_space(std::size_t nbytes) {
		std::size_t min_capacity = size_ + nbytes;
		if (capa_ < min_capacity) {
			grow_capacity(min_capacity);
		}
	}

	/// Grow the capacity, without adjusting size.
	/// Either doubles the capacity, or grows to min_capacity, whichever is larger.
	///
	void grow_capacity(std::size_t min_capacity) {
		std::size_t capacity;
		if (capa_ == 0) {
			AB_ASSERT(data_ == nullptr);
			AB_ASSERT(size_ == 0);
			capacity = std::max(min_capacity, DEFAULT_CAPACITY);
		} else {
			AB_ASSERT(data_ != nullptr);
			AB_ASSERT(size_ <= capa_);
			capacity = std::max(min_capacity, capa_ * 2);
		}
		resize_capacity(capacity);
	}

	/// Resize the data buffer, without adjusting size.
	/// If the new capacity is zero, free the data buffer.
	///
	void resize_capacity(std::size_t capacity) {
		if (capacity == capa_) {
			return;
		}
		if (capacity == 0) {
			std::free(data_);
			capa_ = 0;
			return;
		}
		auto d = reinterpret_cast<Byte*>(std::realloc(data_, capacity));
		if (!d) {
			throw std::bad_alloc();
		}
		data_ = d;
		capa_ = capacity;
	}

	Byte* data_;
	std::size_t size_;
	std::size_t capa_;
};

template <typename T>
ByteBuffer& operator<<(ByteBuffer& buffer, T value) {
	buffer.append(value);
	return buffer;
}

}  // namespace Ab

#endif  // AB_BYTEBUFFER_HPP_
