#include <Pith/Config.hpp>
#include <Pith/Page.hpp>
#include <gtest/gtest.h>

using namespace Pith;

TEST(TestPage, StackAllocate) {
	Page p;
}

TEST(TestPage, map) {
	Span<Page> pages{nullptr, 1};
	auto result = Page::map(pages);
	EXPECT_TRUE(result.isOk());
	pages = result.value();
	auto e = Page::setPermissions(p, PagePermissions{PagePermissions::READ});
}

#if 0
TEST(PageSpan, mapAndUnmap) {
	PageSpan pages{nullptr, 1};
	auto map = pages.map();
	map.unmap();
}
#endif  // 0