#include "gthash/gthash.h"
#include <gtest/gtest.h>

#include <tuple>

using namespace gthash;

TEST(low_level, same_value_results_in_same_hash) {
    EXPECT_EQ(lowlevel::hash(2), lowlevel::hash(2));
    EXPECT_EQ(lowlevel::hash('2'), lowlevel::hash('2'));
}

TEST(low_level, different_value_results_in_different_hash) {
    EXPECT_NE(lowlevel::hash(1), lowlevel::hash(2));
    EXPECT_NE(lowlevel::hash('1'), lowlevel::hash('2'));
}

TEST(low_level, empty_tuple_hashes_to_same_value_everytime) {
    constexpr std::tuple<> empty;
    EXPECT_EQ(lowlevel::hash(empty), lowlevel::hash(empty));
    EXPECT_EQ(lowlevel::hash(empty), lowlevel::hash(empty));
}

TEST(composed_free_function, same_value_results_in_same_hash) {
    constexpr auto value = std::make_tuple(1, true);
    EXPECT_EQ(lowlevel::hash(value), lowlevel::hash(value));
}

TEST(composed_free_function, different_value_results_in_different_hash) {
    constexpr auto value = std::make_tuple(1, true);
    EXPECT_NE(
        lowlevel::hash(std::make_tuple(1, true)),
        lowlevel::hash(std::make_tuple(1, false)));
}