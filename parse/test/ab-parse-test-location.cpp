#include <Ab/Parse/LineInfo.hpp>

#include <gtest/gtest.h>

namespace Ab::Test {

TEST(Location, ConstructsOk) {
	Parse::LineInfo table;
}

TEST(Location, Empty) {
	Parse::LineInfo table;
	EXPECT_THROW(table.line(0), std::runtime_error);
}

TEST(Location, TooFewEntries) {
	Parse::LineInfo table;
	table.store(0);
	EXPECT_THROW(table.line(0), std::runtime_error);
}

TEST(TestLocation, OneLine) {
	Parse::LineInfo table;

	table.store(0); // 0
	table.store(2); // 1 - eof

	EXPECT_EQ(table.line(0), 0);
	EXPECT_EQ(table.line(1), 0);
}

TEST(TestLocation, TwoLines) {
	Parse::LineInfo table;

	table.store(0); // 0
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
	table.store(0); // 0π
	table.store(1); // 1
	table.store(2); // 2 - eof

	EXPECT_EQ(table.line(0), 0);
	EXPECT_EQ(table.line(1), 1);
	EXPECT_EQ(table.line(2), 2);
}

TEST(Location, Columns) {
	Parse::LineInfo table;
	table.store(0); // 0
	table.store(1); // 1
	table.store(3); // 2 - eof

	EXPECT_EQ(table.column(0), 0);
	EXPECT_EQ(table.column(1), 0);
	EXPECT_EQ(table.column(2), 1);
	EXPECT_EQ(table.column(3), 0);
}

} // namespace Ab::Test
