#include <gtest/gtest.h>
#include <vvvstdhelper/has_operator.hpp>

struct foo {
    foo operator+(const foo&) const { return {}; }
};

TEST(has_operator, base)
{
    ASSERT_TRUE(vvv::has_eq<int>());
    ASSERT_FALSE(vvv::has_eq<foo>());
    ASSERT_TRUE(vvv::has_binary_plus<int>());
    ASSERT_TRUE(vvv::has_binary_plus<foo>());
}
