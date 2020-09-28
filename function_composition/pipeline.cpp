#include <tuple>
constexpr auto inc = [](const auto i){ return i+1;};
constexpr auto twice = [](const auto i){ return i*2;};

template<typename ...Fs>
struct Pipeline {
    const std::tuple<Fs ...> fs;
    template<typename G>
    constexpr auto operator|(const G &g) const {
        return Pipeline{std::tuple_cat(fs, g)};
    }
};
constexpr auto id_ = [](const auto i){ return i;};
constexpr auto ID = Pipeline<decltype(id_)>{std::make_tuple(id_)};
template<typename Arg, typename Pipe>
constexpr auto operator|(Arg a, Pipe p){
    return 22;
}
static_assert( (10 | ID | inc | twice) == 22);