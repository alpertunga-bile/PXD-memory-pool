#include <gtest/gtest.h>

TEST(HelloTest, Basic) {
    EXPECT_STRNE("hello", "world");

    EXPECT_EQ(2 * 2, 4);
}
