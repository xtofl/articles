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

    template<typename Map, typename Key, typename Value>
    auto &find_or_create(Map &m, const Key &k, Value v) {
        auto it = m.find(k);
        if (it == end(m)) {
            return m.insert({k, v}).first->second;
        } else {
            return it->second;
        }
    }

    template<typename Map, typename Monoid>
    auto fmonoid(Monoid m) {
        auto mappend = [=](Map a, Map b) {
            for(const auto& kv: b) {
                auto &xa = find_or_create(a, kv.first, m.mempty);
                auto xb = kv.second;
                xa = m.mappend(xa, xb);
            }
            return a;
        };
        return monoid(
            Map{},
            mappend
        );
    }

}

template<typename T> struct Sum {
    T value;
    static constexpr T mempty{};
    static Sum mappend(Sum a, Sum b) {
        return {a.value+b.value};
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

