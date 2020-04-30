#include <vector>
#include <numeric>
#include <algorithm>
#include <random>

std::random_device rd;
std::mt19937 g(rd());


    template<typename T_, typename Mappend_t>
    struct Monoid {
        using T = T_;
        T mempty;
        Mappend_t mappend;
    };

    auto monoid = [](auto e, auto f) {
        return Monoid<decltype(e), decltype(f)>{e, f};
    };

    template<typename Monoid, typename It>
    auto mconcat(Monoid m, It b, It e) {
        return std::accumulate(b, e, m.mempty, m.mappend);
    }

    template<typename Monoid, typename Range>
    auto mconcat(Monoid m, Range &&r) {
        return mconcat(m, begin(r), end(r));
    }

constexpr auto N = 1000;
auto input_data() {
  std::vector<int> ints(N);
  std::iota(begin(ints), end(ints), 1);
  std::shuffle(begin(ints), end(ints), g);
  return ints;
};

const auto ints = input_data();

int add(int a, int b) { return a+b; }
static void main_bare(benchmark::State& state) {

  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
        auto a = std::accumulate(begin(ints), end(ints), 0, add);
        // Make sure the variable is not optimized away by compiler
        benchmark::DoNotOptimize(a);
  }
}
// Register the function as a benchmark
BENCHMARK(main_bare);

static void main_monoid(benchmark::State& state) {
  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
        auto intsum = monoid(0, add);//Monoid<int, add, 0>{};

        auto a = mconcat(intsum, ints);
        // Make sure the variable is not optimized away by compiler
        benchmark::DoNotOptimize(a);
  }
}
 
// Register the function as a benchmark
BENCHMARK(main_monoid);
