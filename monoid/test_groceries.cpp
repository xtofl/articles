#include <gtest/gtest.h>
#include "groceries.cc"

struct Gram : public Unit {};
template<> const char *label<Gram> = "g";

/////////////////////////////////////////////////
// Amount, +, 0 is a monoid
TEST(Amount, is_internal) {
    Amount<Gram> a, b;
    static_assert(std::is_same_v<Amount<Gram>, decltype(a + b)>);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}