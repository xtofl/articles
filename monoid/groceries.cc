#include <cstdio>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <tuple>
#include <cassert>

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

template<typename T, typename F>
auto transform(const std::vector<T> &as, F f) {
    std::vector<decltype(f(as.front()))> result;
    std::transform(begin(as), end(as), std::back_inserter(result), f);
    return result;
}

auto operator-(const IngredientList &a) {
    return transform(a, [](auto x) { x.amount.n *= -1; return x; });
}

auto operator+(const IngredientList &a, const IngredientList &b) {
    IngredientList result = a;
    const auto bkeys = transform(a, [](const auto &ingredient) { return ingredient.name; });
    for (const auto &bx: b) {
        const auto it = std::find_if(
            begin(result), end(result),
            [=](auto x){ return x.name == bx.name; });
        if (it != end(result)) {
            it->amount.n += bx.amount.n;
        } else {
            const auto where = std::lower_bound(
                    begin(result), end(result),
                    bx,
                    [](auto a, auto b) { return a.name < b.name; }
                );
            result.insert(
                where,
                bx);
        }
    }
    return result;
}

auto operator-(const IngredientList& a, const IngredientList &b) {
    return a + (-b);
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
TEST(ingredient_lists_subtract, per_ingredient) {
    const IngredientList
        list1 = {{ {"kaas"}, {300, gram}}, {"pasta", {1, pak}}},
        list2 = {{ {"kaas"}, {100, gram}}, {"zakdoekjes", {2, pak}}}
        ;
    EXPECT_EQ((IngredientList{{
            {"kaas", {200, gram}},
            {"pasta", {1, pak}},
            {"zakdoekjes", {-2, pak}}
        }}),
        list1 - list2);
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