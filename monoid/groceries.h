#pragma once

#include <map>
#include <string>

using Name = std::string;
using Quantity = int;
struct GroceryList {
    std::map<Name, Quantity> items;
};
inline bool operator==(const GroceryList& a, const GroceryList& b) {
    return a.items == b.items;
}

template<typename It>
GroceryList join_grocerylists(It b, It e) {
    static_assert(std::is_same_v<typename It::value_type, GroceryList>);
    GroceryList result{};
    for( ; b!=e ; ++b) {
        for(const auto &ingredient: b->items) {
            result.items[ingredient.first] += ingredient.second;
        }
    }
    return result;
}
