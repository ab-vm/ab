#if !defined AB_VEC_HPP_
#define AB_VEC_HPP_

#include <Ab/Rc.hpp>
#include <Ab/SizeP2.hpp>

#include <initializer_list>
#include <cstdio>
#include <fmt/format.h>

namespace Ab {

// todo: tail optimization: https://github.com/clojure/clojure/blob/master/src/jvm/clojure/lang/PersistentVector.java#L175

// Persistent Vector
template <typename T>
class Vec {
private:
	// Branch factor, power of two.
	static constexpr SizeP2 RADIX = SIZEP2_4;

	class Node : public RefCounted {};

	struct Leaf final : public Node {
		~Leaf() {
			fprintf(stderr, "delete leaf %p\n", this);
		}

		T values[std::size_t(RADIX)];
	};

	struct Branch final : public Node {
		~Branch() {
			fprintf(stderr, "delete branch %p\n", this);
		}

		Node* children[std::size_t(RADIX)] = {0};
	};

public:
	friend class Iter;

	class Iter {
	public:
		Iter(const Iter&) = default;

		Iter& operator++() noexcept {
			subindex_ += 1;

			if (subindex_ == RADIX.get()) {
				current_ = vec_.leafFor(subindex_ + offset_);
				subindex_ = 0;
				offset_ += RADIX.get();
			}
			return *this;
		}

		const T& operator*() const noexcept {
			assert(subindex_ + offset_ < vec_.size());
			return current_->values[subindex_];
		}

		bool operator==(const Iter& rhs) const noexcept {
			return index() == rhs.index();
		}

		bool operator!=(const Iter& rhs) const noexcept {
			return !(*this == rhs);
		}

		Iter& operator=(const Iter& rhs) noexcept = default;

	private:
		friend class Vec<T>;

		Iter(const Vec<T>& vec, Leaf* leaf, std::size_t offset, std::size_t subindex)
			: vec_(vec), current_(leaf), offset_(offset), subindex_(subindex) {}

		Iter(const Vec<T>& vec, std::size_t offset, std::size_t subindex)
			: Iter(vec, vec.leafFor(offset + subindex), offset, subindex) {}

		Iter(const Vec<T>& vec, std::size_t index)
			: vec_(vec),
			  current_(vec.leafFor(index)),
			  offset_((index / RADIX) * RADIX),
			  subindex_(index % RADIX) {}

		std::size_t index() const noexcept { return offset_ + subindex_; }

		const Vec& vec_;
		const Leaf* current_;
		std::size_t offset_;
		std::size_t subindex_;
	};

	Vec() : root_(nullptr), size_(0), level_(0) {
		root_ = new Leaf();
		root_->refcounter().inc();
	}

	Vec(const Vec<T>& other) : root_(other.root_), size_(other.size_), level_(other.level_)  {
		fprintf("copying %p to %p\n", &other, this);
		root_->refcounter().inc();
	}

	Vec(Vec<T>&& other) : root_(other.root_), size_(other.size_), level_(other.level_) {
		fprintf("moving %p to %p\n", &other, this);
	}

	~Vec() noexcept {
		release(root_, level_);
	};

	constexpr const T& at(std::size_t i) const {
		const Leaf* leaf = leafFor(i);
		return leaf->values[i % RADIX];
	}

	const T& operator[](std::size_t i) const {
		return at(i);
	}

	/// Associate i with x.
	Vec<T> assoc(std::size_t i, const T& x) const {
		assert(size_ <= i);
		return Vec<T>(size_, level_, update(level_, root_, i, x));
	}

	Iter begin() const {
		return Iter(*this, 0);
	}

	Iter end() const {
		return Iter(*this, size());
	}

	/// Append x.
	Vec<T> conj(Vec<T>& x) const;

	/// Append x.
	Vec<T> cons(const T& x) const {

		const auto i = size_; // pushing on the end
		const auto size2 = size_ + 1;

		if (i < capacity()) {
			auto root2 = update(level_, root_, i, x);
			return Vec<T>(root2, size2, level_);
		}

		// must grow

		fprintf(stderr, "growing\n");

		const auto root2 = new Branch();
		const auto level2 = level_ + 1;

		root2->children[0] = root_;
		root_->refcounter().inc();

		auto si = subindex(level2, i);

		assert(root2->children[si] == nullptr);

		root2->children[si] = update(level2 - 1, root2->children[si], i, x);
		root2->children[si]->refcounter().inc();
		return Vec<T>(root2, size2, level2);
	}

	constexpr std::size_t capacity() const noexcept {
		return  1 << ((level_ + 1) * RADIX.log2());
	}

	constexpr std::size_t size() const noexcept { return size_; }

	template <typename F>
	constexpr void foreach(F&& f) const { foreach(root_, level_, f); }

	/// MUTATION
	Vec<T>& operator=(const Vec<T>& rhs) noexcept {
		fprintf(stderr, "assiging\n");
		release(root_, level_);
		root_ = rhs.root_;
		root_->refcounter().inc();
		size_ = rhs.size_;
		level_ = rhs.level_;
		return *this;
	}

	void dump() const { dump(level_, root_); }

private:
	// return the branch index.
	static std::size_t subindex(std::size_t level, std::size_t i) noexcept {
		const auto shift = level * RADIX.log2();
		return (i >> shift) % RADIX;
	}

	/// release the node from this vector.
	static void release(const Node* node, std::size_t level) {
		if (node == nullptr) {
			return;
		}

		if (node->refcounter().dec() == 0) {
			if (level == 0) {
				auto leaf = static_cast<const Leaf*>(node);
				delete leaf;
			} else {
				auto branch = static_cast<const Branch*>(node);
				for (auto child : branch->children) {
					release(child, level - 1);
				}
				delete branch;
			}
		}
	}

	// Copy a leaf. new src has a refcount of zero.
	static Leaf* copy(const Leaf* src) {
		if (src == nullptr) {
			return new Leaf();
		}

		return new Leaf(*src);
	}

	static Branch* copy(const Branch* src) {
		if (src == nullptr) {
			auto dst = new Branch();
			return dst;
		}

		auto dst = new Branch(*src);

		for (auto c : dst->children) {
			if (c != nullptr) {
				c->refcounter().inc();
			}
		}

		return dst;
	}

	static Node* update(std::size_t level, const Node* node, std::size_t i, const T& x) {
		if (level == 0) {
			auto leaf = copy(static_cast<const Leaf*>(node));
			leaf->values[i % RADIX] = x;
			return leaf;
		}

		auto clone = copy(static_cast<const Branch*>(node));
		auto si = subindex(level, i);
		auto shift = level * RADIX.log2();
		auto subindex = (i >> shift) % RADIX;

		release(clone->children[si], level - 1);
		clone->children[si] = update(level - 1, clone->children[subindex], i, x);
		clone->children[subindex]->refcounter().inc();
		return clone;
	}

	template <typename F>
	static constexpr void foreach(const Node* node, int level, F&& f) {
		if (node == nullptr) {
			return;
		}
		if (level == 0) {
			for (const T& e : static_cast<const Leaf*>(node)->values) {
				f(e);
			}
		} else {
			for (const Node* c : static_cast<const Branch*>(node)->children) {
				foreach(c, level - 1, f);
			}
		}
	}

	static Node* nodefor(const Node* node, std::size_t level, std::size_t i) {
		assert(0 < level);
		auto branch = static_cast<const Branch*>(node);
		auto si = subindex(level, i);
		return branch->children[si];
	}

	Vec(Node* root, std::size_t size, std::uint8_t level)
		: root_(root), size_(size), level_(level) {
		root->refcounter().inc();
	}


	/// height of the b-tree. a level of 0 indicates no branch nodes.
	constexpr std::size_t level() const noexcept { return level_; }

	/// leaf corresponding to index I.
	const Leaf* leafFor(const std::size_t i) const {
		const Node* node = root_;
		for (auto level = level_; 0 != level; --level) {
			node = nodefor(node, level, i);
			assert(node != nullptr);
		}
		return static_cast<const Leaf*>(node);
	}

	void dump(std::size_t level, const Node* node) const {
		if (node == nullptr) {
			fprintf(stderr, "\n(nil (level %zu))", level);
		}
		else if (level == 0) {
			auto leaf = static_cast<const Leaf*>(node);
			fprintf(stderr, "\n(leaf %p (rc %zu)", leaf, leaf->refcounter().value());
			for (const T& e : leaf->values) {
				fprintf(stderr, "\n  (element %d)", e);
			}
			fprintf(stderr, ")");
		}
		else {
			auto branch = static_cast<const Branch*>(node);
			fprintf(stderr, "\n(branch %p (level %zu) (rc %zu)", branch, level, branch->refcounter().value());
			for (auto c : branch->children) {
				dump(level - 1, c);
			}
			fprintf(stderr, ")");
		}
	}

	Node* root_;         // 
	std::size_t size_;   // the number of elements in the vector. May be less than the capacity.
	std::uint8_t level_; // the number of branch-layers. A level-0 vector contains only one leaf.
};

}  // namespace Ab

#endif // AB_VEC_HPP_
