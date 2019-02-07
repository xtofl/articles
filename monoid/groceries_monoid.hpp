#pragma once

#include "groceries.h"
#include "monoid.hpp"

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
