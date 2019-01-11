#include <cstdio>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <tuple>

struct Unit {
    std::string label;
};
auto tie(const Unit &a) { return std::tie(a.label); }
bool operator < (const Unit &a, const Unit &b) { return tie(a) < tie(b); }
bool operator == (const Unit &a, const Unit &b) { return tie(a) == tie(b); }
struct Amount {
    int n;
    Unit unit;
};
auto tie(const Amount &a) { return std::tie(a.n, a.unit); }
bool operator < (const Amount &a, const Amount &b) { return tie(a) < tie(b); }
bool operator == (const Amount &a, const Amount &b) { return tie(a) == tie(b); }
auto to_string(const Amount &a) {
    return std::to_string(a.n) + a.unit.label;
}
struct Ingredient {
    std::string name;
    Amount amount;
};
auto tie(const Ingredient &a) { return std::tie(a.name, a.amount); }
bool operator < (const Ingredient &a, const Ingredient &b) { return tie(a) < tie(b); }
bool operator == (const Ingredient &a, const Ingredient &b) { return tie(a) == tie(b); }
auto to_string(const Ingredient &i) {
    return i.name + " " + to_string(i.amount);
}
using IngredientList = std::vector<Ingredient>;
struct Dish {
    IngredientList ingredients;
};

struct Menu {
    std::vector<Dish> dishes;
};
auto menu() {
    return Menu{};
}

auto ingredients(const Menu & menu) {
    return IngredientList{};
}

auto operator+(const IngredientList& a, const IngredientList &b) {
    IngredientList result;
    result.insert(end(result), begin(b), end(b));
    return result;
}

template<typename T, typename F>
auto transform(const std::vector<T> &as, F f) {
    std::vector<decltype(f(as.front()))> result;
    std::transform(begin(as), end(as), std::back_inserter(result), f);
    return result;
}

auto operator-(const IngredientList &a, const IngredientList &b) {
    IngredientList result;
    const auto keys = transform(a, [](const auto &ingredient) { return ingredient.name; });
    for (const auto &key: keys) {
        auto va = std::find_if(begin(a), end(a), [=](auto x){ return x.name == key; });
        auto vb = std::find_if(begin(b), end(b), [=](auto x){ return x.name == key; });
        result.push_back(*va);
        if (vb != end(b))
            result.back().amount.n -= vb->amount.n;
    }
    return result;
}

const Unit gram{"g"};
const Unit pak{"pak"};
 
auto extras() {
    return IngredientList{{
        {"kaas", {100, gram}},
        {"chipolata", {400, gram}},
        {"spaghetti", {2, pak}},
    }};
}
auto pantry() {
    return IngredientList{{
        {"chipolata", {200, gram}},
        {"spaghetti", {4, pak}},
    }};
}

std::ostream &operator<< (std::ostream &out, const Ingredient &ingredient) {
    out << "[" << to_string(ingredient) << "]";
    return out;
}

#include <gtest/gtest.h>
TEST(ingredient_lists, can_be_subtracted) {
    const IngredientList
        list1 = {{ {"kaas"}, {100, gram}}},
        list2 = {{ {"kaas"}, {100, gram}}}
        ;
    EXPECT_EQ((IngredientList{{{"kaas", {0, gram}} }}), list1 - list2);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
    auto list =
          ingredients(menu())
        + extras()
        - pantry();
    for(auto i: list) {
        printf("o %s\n", to_string(i).c_str());
    }
}