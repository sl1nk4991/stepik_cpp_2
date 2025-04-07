#include "constexpr.cpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(TestGroupName, Subtest_1) {
    ASSERT_TRUE(1 == 1);
}

TEST(TestGroupName, Subtest_2) {
    Point p1 = {1, 0};
    Point p2 = {0, 1};

    Point pt;

    pt = p1 + p2;
    ASSERT_TRUE(pt.x == 1);
    ASSERT_TRUE(pt.y == 1);

    pt = p1 - p2;
    ASSERT_TRUE(pt.x == 1);
    ASSERT_TRUE(pt.y == -1);

    ASSERT_TRUE(p1 * p2 == 0);
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);

    return RUN_ALL_TESTS();
}