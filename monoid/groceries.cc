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
};

template<typename U>
auto operator+(Amount<U> a, Amount<U> b) {
    return Amount<U>{a.n + b.n};
}

template<typename U>
auto to_string(const Amount<U> &a) {
    return std::to_string(a.n) + label<Amount>;
}

// just a helper function
template<typename T, typename F>
auto transform(const std::vector<T> &as, F f) {
    std::vector<decltype(f(as.front()))> result;
    std::transform(begin(as), end(as), std::back_inserter(result), f);
    return result;
}

