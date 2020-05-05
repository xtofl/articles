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

        return 1;
    }
}
}