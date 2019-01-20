#include <cstdio>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <tuple>
#include <optional>
#include <cassert>

struct Unit {
};
template<typename U> const char * label;

template<typename U>
struct Amount {
    int n;
    using unit_t = U;
    bool operator==(const Amount &other) const {
        return n == other.n;
    }
};

template<typename U>
auto operator+(Amount<U> a, Amount<U> b) {
    return Amount<U>{a.n + b.n};
}
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

// just a helper function
template<typename T, typename F>
auto transform(const std::vector<T> &as, F f) {
    std::vector<decltype(f(as.front()))> result;
    std::transform(begin(as), end(as), std::back_inserter(result), f);
    return result;
}

