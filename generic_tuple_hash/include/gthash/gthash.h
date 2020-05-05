#pragma once

#include <tuple>
#include <functional>
#include <cstddef>
#include <tuple>

namespace gthash {
namespace lowlevel {

    template<typename T>
    constexpr size_t hash(T t){ return std::hash<T>{}(t); }

    template<typename T>
    constexpr size_t hash(const T *t){ return std::hash<const T*>{}(t); }

    namespace detail {
        template<typename ...Ts>
        constexpr auto variadic_xor(Ts... ts)
        {
            return (ts ^ ... ^ 1);
        }
    }

    template<typename ...Ts>
    constexpr size_t variadic_hash(Ts... ts) {
        // I would have liked to write
        // return (hash(ts)... ^ 1);
        return detail::variadic_xor(hash(ts)...);
    }
        
    template<typename ...Ts>
    constexpr size_t hash(const std::tuple<Ts...> &t) {
        return std::apply(&variadic_hash<Ts...>, t);
    }
}
}

namespace std {
    template<typename ...Ts>
    struct hash<std::tuple<Ts...>> {
        constexpr size_t operator()(const std::tuple<Ts...> &t) const {
            return gthash::lowlevel::hash(t);
        }
    };
}