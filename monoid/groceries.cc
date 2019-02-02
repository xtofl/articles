#include <cstdio>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <string>
#include <vector>
#include <tuple>
#include <optional>
#include <cassert>

namespace overloading {
    // Based on convention:
    // operation 'mappend' is overloaded for T
    //
    template<typename T> T mempty();
    template<typename T> T mappend(T, T);

    template<>
    int mempty<int>() { return 0; }
    template<>
    int mappend<int>(int a, int b) { return a + b; }

    template<>
    std::string mempty<std::string>() { return ""; }
    template<>
    std::string mappend<std::string>(std::string a, std::string b) { return a + b; }


    template<typename It, typename Monoid = typename It::value_type>
    Monoid mconcat(It b, It e) {
        return std::accumulate(b, e, mempty<Monoid>(), mappend<Monoid>);
    }
}

namespace traits {
    template<typename Monoid, typename It>
    Monoid mconcat(It b, It e) {
        // TODO: return std::accumulate(b, e, Monoid::mempty(), Monoid::mappend);
        Monoid acc = Monoid::mempty();
        while(b != e) {
            acc = Monoid::mappend(acc, {*b});
            ++b;
        }
        return acc;
    }
}

using Name = std::string;
using Quantity = int;
struct GroceryList {
    std::map<Name, Quantity> items;
};
bool operator==(const GroceryList& a, const GroceryList& b) {
    return a.items == b.items;
}

template<>
GroceryList overloading::mempty<GroceryList>() { return {}; }
template<>
GroceryList overloading::mappend<GroceryList>(
    GroceryList a, GroceryList b)
{
    for (const auto &ib: b.items) {
        a.items[ib.first] += ib.second;
    }
    return a;
}

template<typename It>
GroceryList join_grocerylists(It b, It e) {
    static_assert(std::is_same_v<typename It::value_type, GroceryList>);
    GroceryList result{};
    for( ; b!=e ; ++b) {
        for(const auto &ingredient: b->items) {
            result.items[ingredient.first] += ingredient.second;
        }
    }
    return result;
}

template<typename T> struct Sum {
    T value;
    static constexpr T mempty{};
    static Sum mappend(Sum a, Sum b) {
        return {a.value+b.value};
    }
};

template<typename Map, typename Monoid>
struct FSum {
    using K = typename Map::key_type;
    using V = typename Map::value_type;
    Map t;
    static FSum mempty() {
        return {};
    }
    static FSum mappend(FSum a, FSum b) {
        for(const auto& kv: b.t) {
            auto &xa = a.t[kv.first];
            auto xb = kv.second;
            xa = Monoid::mappend(Monoid{xa}, Monoid{xb}).t;
        }
        return a;
    }
};

// just a helper function
template<typename T, typename F>
auto transform(const std::vector<T> &as, F f) {
    std::vector<decltype(f(as.front()))> result;
    std::transform(begin(as), end(as), std::back_inserter(result), f);
    return result;
}

