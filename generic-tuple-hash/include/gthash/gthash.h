#pragma once

#include <tuple>

namespace gthash {
namespace lowlevel {
    constexpr int hash(std::tuple<>) {
        return 1;
    }
}
}