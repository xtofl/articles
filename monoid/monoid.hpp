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
    template<typename T, T Mempty, typename FMappend>//typename MAppend, MAppend _mappend = MAppend{}>
    struct Monoid {
        static constexpr T mempty = Mempty;
        FMappend mappend;
    };
    template<typename T, T mempty, typename FMappend>//typename MAppend, MAppend _mappend = MAppend{}>
    constexpr auto monoid(FMappend mappend) {
        return Monoid<T, mempty, FMappend>{mappend};
    }

    template<typename Monoid, typename It>
    auto mconcat(Monoid m, It b, It e) {
        return std::accumulate(b, e, Monoid::mempty, m.mappend);
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

