#include <gtest/gtest.h>
#include "groceries.cc"

struct Gram : public Unit {};
template<> const char *label<Gram> = "g";

constexpr Amount<Gram> operator "" _g(unsigned long long n) {
    return {static_cast<int>(n)};
}

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

//////////////////////////////////////////////////
// AmountList; a structure containing a Monoid
TEST(AmountList, is_internal) {
    AmountList<Gram> a{{100_g}}, b{{200_g}};
    static_assert(std::is_same_v<AmountList<Gram>, decltype(a+b)>);
}

TEST(AmountList, is_associative) {
    AmountList<Gram> a{{100_g}}, b{{200_g}}, c{{300_g}};
    ASSERT_EQ(a + (b+c), (a+b) + c);
}

TEST(AmountList, has_neutral_element) {
    AmountList<Gram> a{{1_g}};
    ASSERT_EQ( zero(a) + a, a + zero(a) );
    ASSERT_EQ( zero(a) + a, a           );
    ASSERT_EQ(           a, a + zero(a) );
}

////////////////////////////////////////////////
// A generic view on Monoids

template<typename U>
std::ostream &operator<<(std::ostream &out, const Amount<U> &a) {
    return out << a.n << label<U>;
}

#include <numeric>
TEST(Monoid, can_sum_any_monoid) {
    std::vector<Sum<int>> ints;
    ASSERT_EQ(0, mconcat(begin(ints), end(ints)).value);
    ints.push_back(Sum<int>{10});
    ints.push_back(Sum<int>{20});
    ASSERT_EQ(30, mconcat(begin(ints), end(ints)).value);

    std::vector<Sum<Amount<Gram>>> weights;
    ASSERT_EQ(0_g, mconcat(begin(weights), end(weights)).value);
    weights.push_back(Sum<Amount<Gram>>{10_g});
    weights.push_back(Sum<Amount<Gram>>{20_g});
    ASSERT_EQ(30_g, mconcat(begin(weights), end(weights)).value);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}