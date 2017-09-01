#ifndef AB_MEMORY_HPP_
#define AB_MEMORY_HPP_

#include <Pith/Address.hpp>
#include <Pith/Assert.hpp>
#include <Pith/Page.hpp>
#include <Pith/Result.hpp>
#include <cstdint>

namespace Ab {

struct MemoryConfig {
	Pith::Span<Pith::Page> reservedPages_;
	std::size_t initialPageCount_;

	inline auto verify() const -> void {
		PITH_ASSERT(reservedPages_.length() >= initialPageCount_);
	}
};

enum class MemoryState { DEAD, ACTIVE };

enum class MemoryError { SUCCESS, ERROR };

class Memory {
public:
	static constexpr inline auto defaultConfig() -> const MemoryConfig& {
		return defaultConfig_;
	}

	inline constexpr Memory()
		: reservedPages_{nullptr, 0}, activePageCount_{0}, state_{MemoryState::DEAD} {};

	~Memory();

	inline auto init(const MemoryConfig& config) -> MemoryError;

	inline auto kill() -> void;

	inline auto grow(std::size_t n = 1) -> MemoryError;

	inline auto activePages() const -> Pith::Span<Pith::Page> {
		return Pith::Span<Pith::Page>{reservedPages().value(), activePageCount_};
	}

	inline auto reservedPages() const -> const Pith::Span<Pith::Page>& {
		return reservedPages_;
	}

private:
	static constexpr const MemoryConfig defaultConfig_{
		{nullptr, Pith::mebibytes(1)},  // reservedPages_
		1  // initialPageCount_
	};

	Pith::Span<Pith::Page> reservedPages_;
	std::size_t activePageCount_;
	MemoryState state_;
};

inline Memory::~Memory() {
	PITH_ASSERT(state_ == MemoryState::DEAD);
}

inline auto Memory::init(const MemoryConfig& config) -> MemoryError {
	PITH_ASSERT(state_ == MemoryState::DEAD);
	config.verify();

	auto result = Pith::Page::map(config.reservedPages_);

	if (!result) {
		return MemoryError::ERROR;
	}

	reservedPages_.value(result());
	reservedPages_.length(config.reservedPages_.length());

	int error = Pith::Page::setPermissions(
		activePages(), Pith::Page::Permission::READ | Pith::Page::Permission::WRITE);

	if (error != 0) {
		PITH_ASSERT(Pith::Page::unmap(reservedPages_) == 0);
		return MemoryError::ERROR;
	}

	state_ = MemoryState::ACTIVE;
	return MemoryError::SUCCESS;
}

inline auto Memory::kill() -> void {
	PITH_ASSERT(state_ == MemoryState::ACTIVE);
	state_ = MemoryState::DEAD;
	PITH_ASSERT(Pith::Page::unmap(reservedPages_) == 0);
}

inline auto Memory::grow(std::size_t n) -> MemoryError {
	PITH_ASSERT(state_ == MemoryState::ACTIVE);

	if (activePageCount_ + n > reservedPages().length()) {
		return MemoryError::ERROR;
	}

	Pith::Span<Pith::Page> newPages{activePages().end(), n};
	int perm = Pith::Page::Permission::READ | Pith::Page::Permission::WRITE;
	auto result = Pith::Page::map(newPages, perm);
	PITH_ASSERT(result);

	activePageCount_ += n;

	return MemoryError::SUCCESS;
}

}  // namespace Ab

#endif  // AB_MEMORY_HPP_
