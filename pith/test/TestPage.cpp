#include <Pith/Config.hpp>
#include <Pith/Page.hpp>
#include <gtest/gtest.h>

using namespace Pith;

TEST(TestPage, StackAllocate) {
	Page p;
}

TEST(TestPage, mapOnePage) {
	Span<Page> pages{nullptr, 1};
	auto result = Page::map(pages);
	EXPECT_TRUE(result);
	EXPECT_NE(result(), nullptr);
	pages.value(result());
	EXPECT_EQ(Page::unmap(pages), 0);
}

TEST(TestPage, setPermissions) {
	Span<Page> pages{nullptr, 1};
}

TEST(TestPage, setPermissionsOnUnmappedPage) {
	Span<Page> pages{nullptr, 1};

	// Get a fresh page.
	{
		auto r = Page::map(pages);
		ASSERT_TRUE(r);
		pages.value(r());
	}

	// Explicitly unmap it.
	{
		auto e = Page::unmap(pages);
		ASSERT_EQ(e, 0);
	}

	// Fail to set permissions on unmapped page.
	{
		auto e = Page::setPermissions(pages, Page::Permissions::READ);
		ASSERT_NE(e, 0);
	}
}

#if 0
TEST(PageSpan, mapAndUnmap) {
	PageSpan pages{nullptr, 1};
	auto map = pages.map();
	map.unmap();
}
#endif  // 0