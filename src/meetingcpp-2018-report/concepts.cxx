#include <cstdio>
#include <vector>
#include <functional>
#include <memory>

template<typename T> concept bool Monoid =
requires (T a, T b, T c) {
    {T::identity()} -> T; // has identity
    {a + b} -> T; // says nothing about the meaning
    // associativity: {(a + b) + c == a + (b + c)}
};


template<Monoid T>
auto sum(std::vector<T> ms) {
    auto start = T::identity();
    return start;
}

template<class T>
auto nsum(std::vector<T> ts) {
    auto start = T::identity();
    return start;
}

struct Distance {
    int value;

    Distance operator+(Distance other){ return other; }
};

int main() {
    std::vector<Distance> ds;
    auto ns = nsum(ds);
    auto s = sum(ds);
}