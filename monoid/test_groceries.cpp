#include <gtest/gtest.h>
#include <functional>
#include "groceries.h"
#include "groceries_monoid.hpp"

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
        mappend(c.s, d.s),
        mappend(c.n, d.n)
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
    using namespace overloading;

    TEST(overloaded, monoidalproperties) {
        std::vector<int> ints{{1, 2, 3, 4}};
        EXPECT_EQ(10, mconcat(begin(ints), end(ints)));

        std::vector<Custom> cs{{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}};
        EXPECT_EQ((Custom{"abcd", 10}), mconcat(begin(cs), end(cs)));
    }

    TEST(grocerylist, has_mempty) {
        const auto empty = mempty<GroceryList>();
        GroceryList x{{{"x", 1}, {"y", 1}}};
        GroceryList y{{{"y", 2}, {"z", 3}}};
        EXPECT_EQ(x, mappend(x, empty));
        EXPECT_EQ(y, mappend(empty, y));
    }
    TEST(grocerylist, has_mappend) {
        GroceryList a, b;
        auto c = mappend(a, b);
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
        }), mconcat(begin(lists), end(lists)));
    }

    // TEST(overloaded, monoidalproperties_multiplication) {
    //     std::vector<int> ints{{1, 2, 3, 4}};
    //     EXPECT_EQ(24, mconcat(begin(ints), end(ints)));
    // }
}

namespace traits {

    template<typename T> struct Sum {
        T t;
        static Sum mempty() { return {T{0}}; }
        static Sum mappend(Sum a, Sum b) {
            return Sum{a.t + b.t};
        }
    };
    template<>
    Sum<std::string> Sum<std::string>::mempty() {
        return {std::string{}};
    }
    template<>
    Sum<std::string> Sum<std::string>::mappend(Sum<std::string> a, Sum<std::string> b) {
        return {{a.t + b.t}};
    }

    template<> Sum<Custom> Sum<Custom>::mempty(){
        return {{}};
    }

    template<> Sum<Custom> Sum<Custom>::mappend(
            Sum<Custom> c,
            Sum<Custom> d) {
        return {Custom{
            Sum<std::string>::mappend({c.t.s}, {d.t.s}).t,
            Sum<int>::mappend({c.t.n}, {d.t.n}).t
        }};
    }
    TEST(typetraits, monoidalproperties) {
        std::vector<int> ints{{1, 2, 3, 4}};
        EXPECT_EQ(10, mconcat<Sum<int>>(begin(ints), end(ints)).t);

        std::vector<Custom> cs{{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}};
        EXPECT_EQ((Custom{"abcd", 10}), mconcat<Sum<Custom>>(begin(cs), end(cs)).t);
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
        const auto expected = (GroceryList{{
            {"bacon <g>", 100},
            {"eggs", 11},
            {"milk <l>", 1},
            {"potatoes <kg>", 2}}
        });
        EXPECT_EQ(expected, mconcat<Sum<GroceryList>>(begin(lists), end(lists)).t);
        EXPECT_EQ(expected, join_grocerylists(begin(lists), end(lists)));
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
        EXPECT_EQ(24, mconcat<Product<int>>(begin(ints), end(ints)).t);
    }

    TEST(traits, map_to_monoids_is_a_monoid) {
        using IntMap = std::map<int, int>;
        const IntMap a{{ {1, 1}, {2, 4}, {3, 9} }};
        const IntMap b{{ {1, 2}, {2, 3}, {3, 4} }};
        std::vector<IntMap> intmaps{a, b};

        const IntMap expected{{{1, 3}, {2, 7}, {3, 13}}};
        EXPECT_EQ(expected,
            (mconcat<FSum<IntMap, Sum<int>>>(
                begin(intmaps),
                end(intmaps)).t));
    }
}

namespace lean {
    TEST(typetraits, monoidalproperties) {
        std::vector<int> ints{{1, 2, 3, 4}};
        auto intsum = monoid(0, std::plus<int>{});
        EXPECT_EQ(10, mconcat(intsum, begin(ints), end(ints)));

        std::vector<Custom> cs{{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}};
        EXPECT_EQ((Custom{"abcd", 10}), mconcat(
            monoid(
                Custom{},
                [](auto a, auto b){
                    return Custom{a.s + b.s, a.n + b.n};
                }),
            begin(cs), end(cs)));
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}