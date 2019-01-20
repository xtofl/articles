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

TEST(Amount, is_associative) {
    Amount<Gram> a{1}, b{5}, c{10};
    ASSERT_EQ(a + (b+c), (a+b) + c);
}

TEST(Amount, has_neutral_element) {
    Amount<Gram> a{1};
    ASSERT_EQ( zero(a) + a, a + zero(a) );
    ASSERT_EQ( zero(a) + a, a           );
    ASSERT_EQ(           a, a + zero(a) );
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}