#include <Pith/Config.hpp>
#include <Pith/Page.hpp>
#include <gtest/gtest.h>

using namespace Pith;

TEST(TestPage, pageSize) {
	Process::init();
	EXPECT_NE(Page::size(), std::size_t{0});
	Process::kill();
}

TEST(TestPage, mapOnePage) {
	Process::init();
	auto size = Page::size();
	auto addr = Page::map(size);
	Page::unmap(addr, size);
	Process::kill();
}
