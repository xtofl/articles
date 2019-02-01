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

    // TEST(overloaded, monoidalproperties_multiplication) {
    //     std::vector<int> ints{{1, 2, 3, 4}};
    //     EXPECT_EQ(24, overloading::mconcat(begin(ints), end(ints)));
    // }
}

namespace test_traits {
    template<typename Value> struct Sum {
        using T = Value;
        T t;
        static Sum mempty() { return {}; }
        static Sum mappend(Sum a, Sum b) {
            return Sum{a.t + b.t};
        }
    };
    Custom operator+(Custom c, Custom d) {
        return {
            c.s + d.s,
            c.n + d.n};
    }
    TEST(typetraits, monoidalproperties) {
        std::vector<int> ints{{1, 2, 3, 4}};
        EXPECT_EQ(10, traits::mconcat<Sum<int>>(begin(ints), end(ints)).t);

        std::vector<Custom> cs{{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}};
        EXPECT_EQ((Custom{"abcd", 10}), traits::mconcat<Sum<Custom>>(begin(cs), end(cs)).t);
    }

    template<> struct Sum<GroceryList> {
        using T = GroceryList;
        T t;
        static Sum mempty() { return {}; }
        static Sum mappend(Sum a, Sum b) {
            for (const auto &ib: b.t.items) {
                a.t.items[ib.first] += ib.second;
            }
            return {a};
        }
    };
    TEST(traits_grocerylist, has_mempty) {
        const auto empty = Sum<GroceryList>::mempty();
        GroceryList x{{{"x", 1}, {"y", 1}}};
        GroceryList y{{{"y", 2}, {"z", 3}}};
        EXPECT_EQ(x, Sum<GroceryList>::mappend({x}, empty).t);
        EXPECT_EQ(y, Sum<GroceryList>::mappend(empty, {y}).t);
    }
    TEST(traits_grocerylist, has_mappend) {
        GroceryList a, b;
        auto c = Sum<GroceryList>::mappend({a}, {b});
    }
    TEST(traits_grocerylist, can_mappend) {
        GroceryList bacon_eggs{{{"bacon <g>", 100}, {"eggs", 5}}};
        GroceryList sunnysideup{{{"eggs", 5}}};
        GroceryList mashed_potatoes{{{"potatoes <kg>", 2}, {"milk <l>", 1}, {"eggs", 1}}};
        std::vector<GroceryList> lists{bacon_eggs, sunnysideup, mashed_potatoes};
        EXPECT_EQ((GroceryList{{
            {"bacon <g>", 100},
            {"eggs", 11},
            {"milk <l>", 1},
            {"potatoes <kg>", 2}}
        }), traits::mconcat<Sum<GroceryList>>(begin(lists), end(lists)).t);
    }
    template<typename Value> struct Product {
        using T = Value;
        T t;
        static Product mempty() { return {1}; }
        static Product mappend(Product a, Product b) {
            return {a.t * b.t};
        }
    };
    TEST(traits, monoidalproperties_multiplication) {
        std::vector<int> ints{{1, 2, 3, 4}};
        EXPECT_EQ(24, traits::mconcat<Product<int>>(begin(ints), end(ints)).t);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}