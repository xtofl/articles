#pragma once

#include <numeric>

namespace overloading {
    // Based on convention:
    // operation 'mappend' is overloaded for T
    //
    template<typename T> T mempty();
    template<typename T> T mappend(T, T);

    template<>
    int mempty<int>() { return 0; }
    template<>
    int mappend<int>(int a, int b) { return a + b; }

    template<>
    std::string mempty<std::string>() { return ""; }
    template<>
    std::string mappend<std::string>(std::string a, std::string b) { return a + b; }


    template<typename It, typename Monoid = typename It::value_type>
    Monoid mconcat(It b, It e) {
        return std::accumulate(b, e, mempty<Monoid>(), mappend<Monoid>);
    }
}

namespace traits {
    template<typename Monoid, typename It>
    Monoid mconcat(It b, It e) {
        // TODO: return std::accumulate(b, e, Monoid::mempty(), Monoid::mappend);
        Monoid acc = Monoid::mempty();
        while(b != e) {
            acc = Monoid::mappend(acc, {*b});
            ++b;
        }
        return acc;
    }
}

namespace lean {
    template<typename Monoid, typename It>
    auto mconcat(Monoid m, It b, It e);

    template<typename T_, typename Mappend_t>
    struct Monoid {
        using T = T_;
        T mempty;
        Mappend_t mappend;

        template<typename Range>
        auto mconcat(Range r) const {
            return lean::mconcat(*this, begin(r), end(r));
        }
    };

    auto monoid = [](auto e, auto f) {
        return Monoid<decltype(e), decltype(f)>{e, f};
    };

    template<typename Monoid, typename It>
    auto mconcat(Monoid m, It b, It e) {
        return std::accumulate(b, e, m.mempty, m.mappend);
    }
}

template<typename T> struct Sum {
    T value;
    static constexpr T mempty{};
    static Sum mappend(Sum a, Sum b) {
        return {a.value+b.value};
    }
};

template<typename Map, typename Monoid>
struct FSum {
    using K = typename Map::key_type;
    using V = typename Map::value_type;
    Map t;
    static FSum mempty() {
        return {};
    }
    static FSum mappend(FSum a, FSum b) {
        for(const auto& kv: b.t) {
            auto &xa = a.t[kv.first];
            auto xb = kv.second;
            xa = Monoid::mappend(Monoid{xa}, Monoid{xb}).t;
        }
        return a;
    }
};

// just a helper function
#include <vector>

template<typename T, typename F>
auto transform(const std::vector<T> &as, F f) {
    std::vector<decltype(f(as.front()))> result;
    std::transform(begin(as), end(as), std::back_inserter(result), f);
    return result;
}

