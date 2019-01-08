// compile with
// g++  -std=c++17 truth.cxx
#include <tuple>
#include <cstdio>

template<class F> auto fib(int a, int b, F yield) {
    if(yield(a)) return;
    if(yield(b)) return;
    while(true) {
        auto next = a + b;
        if(yield(next)) return;
        std::tie(a, b) = std::tie(b, next);
    }
}

namespace checked {
    template<class F> auto fib(int a, int b, F yield) {
        /// prologue(
        ///     a > 0,
        ///     b > 0,
        ///     (x > maxint/2) => !yield(x)
        /// );
        ::fib(a, b, yield);
        /// epilogue();
    }
}
int main() {
    auto consume = [](auto f) { printf("%d ", f); return f > 20; };
    checked::fib(1, 1, consume);
    printf("\n");
}