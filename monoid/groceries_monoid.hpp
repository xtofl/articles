#pragma once

#include "groceries.h"
#include "monoid.hpp"

const auto intmapmonoid =
    lean::fmonoid<std::map<std::string, int>>(
        lean::monoid(0, std::plus<int>{})
    );

const auto grocery_monoid = lean::monoid(
    GroceryList{intmapmonoid.mempty},
    [](auto a, auto b) -> GroceryList{
        return {intmapmonoid.mappend(a.items, b.items)};
    }
);

template<>
GroceryList overloading::mempty<GroceryList>() { return {}; }
template<>
GroceryList overloading::mappend<GroceryList>(
    GroceryList a, GroceryList b)
{
    for (const auto &ib: b.items) {
        a.items[ib.first] += ib.second;
    }
    return a;
}
