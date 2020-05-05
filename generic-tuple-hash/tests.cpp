#include "gthash/gthash.h"
#include <gtest/gtest.h>

#include <tuple>

using namespace gthash;

TEST(low_level, same_value_results_in_same_hash) {
    static_assert(lowlevel::hash(2) == lowlevel::hash(2));
}

TEST(low_level, different_value_results_in_different_hash) {
    static_assert(lowlevel::hash(1) != lowlevel::hash(2));
}

TEST(low_level, empty_tuple_hashes_to_same_value_everytime) {
    constexpr std::tuple<> empty;
    static_assert(lowlevel::hash(empty) == lowlevel::hash(empty));
    EXPECT_EQ(lowlevel::hash(empty), lowlevel::hash(empty));
}

TEST(low_level, singleton_tuple_hashes_to_element_hash) {
    constexpr auto value = 1;
    static_assert(lowlevel::hash(value) == lowlevel::hash(std::make_tuple(1)));
    static_assert(lowlevel::hash(value + 1) != lowlevel::hash(std::make_tuple(1)));
}