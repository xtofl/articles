#pragma once

#include <tuple>
#include <cstddef>

namespace gthash {
namespace lowlevel {
    template<typename T>
    constexpr size_t hash(T &&t){ return t; }

    constexpr size_t hash(std::tuple<>) {
        return 1;
    }

    template<typename ...Ts>
    constexpr size_t hash(std::tuple<Ts...> &&) {
        return 1;
    }
}
}