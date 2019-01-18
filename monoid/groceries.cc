#include <cstdio>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <tuple>
#include <optional>
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

auto get_keys(const IngredientList &a) {
    return transform(a, [](auto i){ return i.name; });
}

template<typename T> auto set_union(const std::vector<T> &ts, const std::vector<T> &t2s) {
    std::vector<T> result;
    std::set_union(
        begin(ts), end(ts),
        begin(t2s), end(t2s),
        std::back_inserter(result));
    return result;
}

std::optional<Ingredient> ingredient(const IngredientList &i, std::string name) {
    auto it = std::find_if(
        begin(i), end(i),
        [&](auto i){ return i.name == name; });
    if(it != end(i))
        return {*it};
    else
        return {};
}

struct Monoid {
    IngredientList value;
    static const Monoid zero;
    Monoid(IngredientList a): value(std::move(a)) {}
    static Monoid append(Monoid a, const Monoid &b) {
        const auto keys = set_union(get_keys(a.value), get_keys(b.value));
        return {transform(keys, [&](auto key){
            auto ia = ingredient(a.value, key);
            auto ib = ingredient(b.value, key);
            auto unit = ia.value_or(*ib).amount.unit;
            auto amount = (ia ? ia->amount.n : 0) + (ib? ib->amount.n : 0);
            return Ingredient{key, {amount, unit}};
        })};
    }
};

#include <gtest/gtest.h>
TEST(ingredient_lists_subtract, per_ingredient) {
    const IngredientList
        list1 = {{ {"kaas"}, {300, gram}}, {"pasta", {1, pak}}},
        list2 = {{ {"kaas"}, {100, gram}}, {"zakdoekjes", {2, pak}}}
        ;
    const IngredientList combined{{
            {"kaas", {200, gram}},
            {"pasta", {1, pak}},
            {"zakdoekjes", {2, pak}}
        }};
    EXPECT_EQ( combined, Monoid::append({list1}, {list2}).value );
}

enum class Proceed { yes, no; };

template<typename It>
struct merger {
    Proceed a(It);
    Proceed b(It);
    Proceed ab(It);
    Proceed end_of_a();
    Proceed end_of_b();
};

auto make_merger = [](auto a, auto b, auto ab, auto eoa, auto eob)
{
    return merger
}

auto merged

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
    std::vector<int> a, b;

    std::vector<int> intersection;
    merge_visit(
        begin(a), end(a),
        begin(b), end(b),
        merger{
            .ab = std::back_inserter(intersection)
        }
    );
}

