#include <iostream>
#include <gtest/gtest.h>
#include <gfx_lib/matrix4X4.h>

TEST(simple_tests, suite1) {
    EXPECT_EQ(2, 2);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
