#include <Ab/Parse/LineInfo.hpp>

#include <gtest/gtest.h>

namespace Ab::Test {

TEST(Location, ConstructsOk) {
	Parse::LineInfo table;
}

TEST(Location, Empty) {
	Parse::LineInfo table;
	EXPECT_EQ(table.line(0), 0);
}

TEST(Location, LineAtOne) {
	Parse::LineInfo table;

	table.store(1);

	EXPECT_EQ(table.line(0), 0);
	EXPECT_EQ(table.line(1), 1);
	EXPECT_EQ(table.line(2), 1);
}

TEST(TestLocation, LineAtTwo) {
	Parse::LineInfo table;

	table.store(2); // 1 - eof

	EXPECT_EQ(table.line(1), 0);
	EXPECT_EQ(table.line(1), 0);
	EXPECT_EQ(table.line(2), 1);
	EXPECT_EQ(table.line(3), 1);
}

TEST(TestLocation, LinesAtTwoAndFour) {
	Parse::LineInfo table;

	table.store(2); // 1
	table.store(4); // 2 - eof

	EXPECT_EQ(table.line(0), 0);
	EXPECT_EQ(table.line(1), 0);
	EXPECT_EQ(table.line(2), 1);
	EXPECT_EQ(table.line(3), 1);
	EXPECT_EQ(table.line(4), 2);
}

TEST(Location, EmptyLines) {
	Parse::LineInfo table;

	table.store(1); // 1
	table.store(2); // 2 - eof

	EXPECT_EQ(table.line(0), 0);
	EXPECT_EQ(table.line(1), 1);
	EXPECT_EQ(table.line(2), 2);
}

TEST(Location, Columns) {
	Parse::LineInfo table;

	table.store(1); // 1
	table.store(3); // 2 - eof

	EXPECT_EQ(table.column(0), 0);
	EXPECT_EQ(table.column(1), 0);
	EXPECT_EQ(table.column(2), 1);
	EXPECT_EQ(table.column(3), 0);
}

} // namespace Ab::Test
