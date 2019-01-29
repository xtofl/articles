#include <gtest/gtest.h>
#include "groceries.cc"

TEST(overloaded, monoidalproperties) {
    std::vector<int> ints{{1, 2, 3, 4}};
    EXPECT_EQ(10, overloading::mconcat(begin(ints), end(ints)));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}