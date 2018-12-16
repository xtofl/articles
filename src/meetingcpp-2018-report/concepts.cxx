#include <cstdio>
#include <vector>
#include <functional>
#include <memory>

template<typename T>
concept Monoid = requires (T a, T b, T c) {
    {T::identity}; // has identity
    {a + b} -> T; // says nothing about the meaning
    {(a + b) + c == a + (b + c)}
};


template<Monoid T>
auto sum(T... ms) {
    return Monoid::zero + ms ...;
}

int main() {
    auto as = std::vector<A>{1, 2, 3};
    auto as2 = as;
}