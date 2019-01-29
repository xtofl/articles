#include <gtest/gtest.h>
#include "groceries.cc"

struct Custom {
    std::string s;
    int n;
    bool operator==(const Custom &c) const {
        return s == c.s && n == c.n;
    }
};

template<>
Custom overloading::mempty<Custom>() { return {}; }
template<>
Custom overloading::mappend<Custom>(Custom c, Custom d) {
    return {
        c.s + d.s,
        c.n + d.n
    };
}

std::ostream &operator<<(std::ostream &o, const Custom &c) {
    return o << "[" << c.s << ", " << c.n << "]";
}
TEST(overloaded, monoidalproperties) {
    std::vector<int> ints{{1, 2, 3, 4}};
    EXPECT_EQ(10, overloading::mconcat(begin(ints), end(ints)));

    std::vector<Custom> cs{{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}};
    EXPECT_EQ((Custom{"abcd", 10}), overloading::mconcat(begin(cs), end(cs)));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}