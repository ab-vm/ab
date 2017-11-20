#include <Ab/Config.hpp>
#include <Ab/Page.hpp>
#include <gtest/gtest.h>

using namespace Ab;

TEST(page, page_size_not_zero) {
	EXPECT_NE(Page::size(), std::size_t{0});
}

TEST(page, map_one_page) {
	auto size = Page::size();
	auto addr = Page::map(size);
	EXPECT_NE(addr, nullptr);
	Page::unmap(addr, size);
}
