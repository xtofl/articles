#include <cstdio>
#include <vector>
#include <functional>
#include <memory>

template<typename T> concept bool Monoid =
requires (T a, T b, T c) {
    {T::identity}; // has identity
    {a + b} -> T; // says nothing about the meaning
    {(a + b) + c == a + (b + c)}
};


template<Monoid T, Monoid ...Ts>
auto sum(Ts... ms) {
    auto start = T::zero;
    return start + ms...;
}

int main() {
    auto as = std::vector<A>{1, 2, 3};
    auto as2 = as;
}