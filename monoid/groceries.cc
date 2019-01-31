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

template<typename T> struct Sum {
    T value;
    static constexpr T mempty{};
    static Sum mappend(Sum a, Sum b) {
        return {a.value+b.value};
    }
};

// just a helper function
template<typename T, typename F>
auto transform(const std::vector<T> &as, F f) {
    std::vector<decltype(f(as.front()))> result;
    std::transform(begin(as), end(as), std::back_inserter(result), f);
    return result;
}

