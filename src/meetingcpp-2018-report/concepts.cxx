#include <cstdio>

template<typename T>
concept bool Monoid = requires(T a, T b) {
    {T::empty()} -> T
};

auto start(Monoid m) {
    return Monoid::empty()
}

auto acc(Monoid m...) {
    auto zero = Monoid::empty();
    return zero.append(m)...;
}

struct X {
    static X empty() { return X{}; }
};

int main() {
    auto x = start(X{});
    // FAILS... auto y = start(1);
}