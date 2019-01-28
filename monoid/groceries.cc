#include <cstdio>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <tuple>
#include <optional>
#include <cassert>


template<typename U>
Amount<U> zero() { return {0};}
template<typename U>
Amount<U> zero(Amount<U>) { return {0};}

template<typename U>
auto to_string(const Amount<U> &a) {
    return std::to_string(a.n) + label<Amount>;
}


template<typename Unit>
struct AmountList {
    std::vector<Amount<Unit>> amounts;

    AmountList operator+(const AmountList &other) const {
        auto result = other;
        result.amounts.insert(
            end(result.amounts),
            begin(amounts), end(amounts));
        return result;
    }
    bool operator==(const AmountList &other) const {
        return amounts == other.amounts;
    }
};

template<typename U>
AmountList<U> zero(AmountList<U> i) { return {{}};}


template<typename T> struct Monoid {};

template<typename It, typename Monoid = typename It::value_type>
Monoid mconcat(It b, It e) {
    Monoid acc{};
    while(b != e) { acc = Monoid::mappend(acc, *b); ++b; }
    return acc;
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

