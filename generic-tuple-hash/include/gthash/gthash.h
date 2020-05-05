#pragma once

#include <tuple>
#include <functional>
#include <cstddef>

namespace gthash {
namespace lowlevel {

    template<typename T>
    constexpr size_t hash(T t){ return std::hash<T>{}(t); }

    template<typename T>
    constexpr size_t hash(const T *t){ return std::hash<const T*>{}(t); }

    constexpr size_t hash(std::tuple<>) {
        return 1;
    }

    template<typename ...Ts>
    constexpr size_t hash(std::tuple<Ts...> &&) {
        return 1;
    }
}
}