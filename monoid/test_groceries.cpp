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
std::ostream &operator<<(std::ostream &o, const GroceryList& g) {
    o << "{";
    for(const auto &it: g.items) {
        o << it.first << ": " << it.second << ", ";
    }
    return o << "}";
}

namespace test_overloading {
    TEST(overloaded, monoidalproperties) {
        std::vector<int> ints{{1, 2, 3, 4}};
        EXPECT_EQ(10, overloading::mconcat(begin(ints), end(ints)));

        std::vector<Custom> cs{{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}};
        EXPECT_EQ((Custom{"abcd", 10}), overloading::mconcat(begin(cs), end(cs)));
    }

    TEST(grocerylist, has_mempty) {
        const auto empty = overloading::mempty<GroceryList>();
        GroceryList x{{{"x", 1}, {"y", 1}}};
        GroceryList y{{{"y", 2}, {"z", 3}}};
        EXPECT_EQ(x, overloading::mappend(x, empty));
        EXPECT_EQ(y, overloading::mappend(empty, y));
    }
    TEST(grocerylist, has_mappend) {
        GroceryList a, b;
        auto c = overloading::mappend(a, b);
    }
    TEST(grocerylist, can_mappend) {
        GroceryList bacon_eggs{{{"bacon <g>", 100}, {"eggs", 5}}};
        GroceryList sunnysideup{{{"eggs", 5}}};
        GroceryList mashed_potatoes{{{"potatoes <kg>", 2}, {"milk <l>", 1}, {"eggs", 1}}};
        std::vector<GroceryList> lists{bacon_eggs, sunnysideup, mashed_potatoes};
        EXPECT_EQ((GroceryList{{
            {"bacon <g>", 100},
            {"eggs", 11},
            {"milk <l>", 1},
            {"potatoes <kg>", 2}}
        }), overloading::mconcat(begin(lists), end(lists)));
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}