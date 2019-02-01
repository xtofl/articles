// http://quick-bench.com/mEJDA6p6zEdR_Wg-Yo_PkXXYwto
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>

std::random_device rd;
std::mt19937 g(rd());

template<typename Value> struct Sum {
    using T = Value;
    T t;
    static Sum mempty() { return {}; }
    static Sum mappend(Sum a, Sum b) {
        return Sum{a.t + b.t};
    }
};


template<typename Monoid, typename It>
Monoid mconcat(It b, It e) {
    Monoid acc = Monoid::mempty();
    while(b != e) {
        acc = Monoid::mappend(acc, {*b});
        ++b;
    }
    return acc;
}
constexpr auto N = 1000;

static void main_bare(benchmark::State& state) {
  std::vector<int> ints(N);
  std::iota(begin(ints), end(ints), 1);
  std::shuffle(begin(ints), end(ints), g);
  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
        auto a = std::accumulate(begin(ints), end(ints), 0);
        // Make sure the variable is not optimized away by compiler
        benchmark::DoNotOptimize(a);
  }
}
// Register the function as a benchmark
BENCHMARK(main_bare);

static void main_monoid(benchmark::State& state) {
  std::vector<int> ints(N);
  std::iota(begin(ints), end(ints), 1);
  std::shuffle(begin(ints), end(ints), g);
  // Code inside this loop is measured repeatedly
  for (auto _ : state) {

        auto a = mconcat<Sum<int>>(begin(ints), end(ints)).t;
        // Make sure the variable is not optimized away by compiler
        benchmark::DoNotOptimize(a);
  }
}
 
// Register the function as a benchmark
BENCHMARK(main_monoid);
