#include "gthash/gthash.h"
#include <gtest/gtest.h>

#include <tuple>

using namespace gthash;

TEST(low_level, empty_tuple_hashes_to_same_value_everytime) {
    constexpr std::tuple<> empty;
    static_assert(lowlevel::hash(empty) == lowlevel::hash(empty));
    EXPECT_EQ(lowlevel::hash(empty), lowlevel::hash(empty));
}

TEST(low_level, singleton_tuple_hashes_to_element_hash) {
    FAIL();
}