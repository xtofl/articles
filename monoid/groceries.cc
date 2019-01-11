#include <cstdio>
#include <algorithm>
#include <string>
#include <vector>

struct Unit {
    std::string label;
};
struct Amount {
    int n;
    Unit unit;
};
auto to_string(const Amount &a) {
    return std::to_string(a.n) + a.unit.label;
}
struct Ingredient {
    std::string name;
    Amount amount;
};
auto to_string(const Ingredient &i) {
    return i.name + " " + to_string(i.amount);
}
struct Dish {
    std::vector<Ingredient> ingredients;
};
struct Menu {
    std::vector<Dish> dishes;
};
auto menu() {
    return Menu{};
}

auto ingredients(const Menu & menu) {
    return std::vector<Ingredient>{};
}

template<typename T>
std::vector<T> operator+(const std::vector<T>& a, const std::vector<T> &b) {
    auto result = a;
    result.insert(end(result), begin(b), end(b));
    return result;
}

template<typename T>
std::vector<T> operator-(const std::vector<T>& a, const std::vector<T> &b) {
    auto result = a;
    return a;
}

const Unit gram{"g"};
const Unit pak{"pak"};
 
auto extras() {
    return std::vector<Ingredient>{{
        {"kaas", {100, gram}},
        {"chipolata", {400, gram}},
        {"spaghetti", {2, pak}},
    }};
}
auto pantry() {
    return std::vector<Ingredient>{{
        {"chipolata", {200, gram}},
        {"spaghetti", {4, pak}},
    }};
}

int main() {
    auto list =
          ingredients(menu())
        + extras()
        - pantry();
    for(auto i: list) {
        printf("o %s\n", to_string(i).c_str());
    }
}