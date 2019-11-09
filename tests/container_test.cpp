#include "vvvstdhelper/containerhelper.hpp"
#include <gtest/gtest.h>
#include <type_traits>

TEST(container, filter_rvalue)
{
    using vvv::helpers::filter;
    std::vector<int> vv{1, 2, 3, 4, 5, 6, 7, 8};

    const auto result = filter(std::move(vv), [](auto v) { return v > 4; });

    const auto& expected = std::vector<int>{5, 6, 7, 8};
    EXPECT_EQ(expected, result);
}

TEST(container, filter)
{
    using vvv::helpers::filter;
    std::vector<int> vv{1, 2, 3, 4, 5, 6, 7, 8};

    const auto result = filter(vv, [](const auto& v) { return v > 4; });

    const auto& expected = std::vector<int>{5, 6, 7, 8};
    EXPECT_EQ(expected, result);
}

TEST(container, contain)
{
    using vvv::helpers::contain;
    std::vector<int> vv{1, 3, 5, 7};

    EXPECT_TRUE(contain(vv, 1));
    EXPECT_TRUE(contain(vv, 3));
    EXPECT_TRUE(contain(vv, 7));
    EXPECT_FALSE(contain(vv, 0));
    EXPECT_FALSE(contain(vv, 2));
    EXPECT_FALSE(contain(vv, -7));

    EXPECT_TRUE(contain({1, 2, 3}, 2));
    EXPECT_TRUE(contain({1, 2, 42}, 42));
    EXPECT_FALSE(contain({1, 2, 42}, 3));
}

TEST(container, map)
{
    using vvv::helpers::map;
    std::vector<int> vv{1, 3, 5, 7};

    const auto& lambda = [](const auto& v) { return v * 2; };
    const auto& double_vv = map(lambda, vv);

    const std::vector<int> expected_double{2, 6, 10, 14};
    EXPECT_EQ(expected_double, double_vv);

    // auto types
    const auto& to_string = [](const auto& v) { return std::to_string(v); };
    const auto& string_vv = map(to_string, vv);

    const std::vector<std::string> expected_string_vv = {"1", "3", "5", "7"};
    EXPECT_EQ(expected_string_vv, string_vv);
}

TEST(container, map2)
{
    using vvv::helpers::map;
    std::vector<int> vv{1, 3, 5, 7};
    std::vector<float> vv2{2, 5, 8, 10};

    const auto& lambda = [](const auto& v, const auto& v2) { return v * v2; };
    const auto& double_vv = map(lambda, vv, vv2);

    const std::vector<float> expected_double{2, 15, 40, 70};
    EXPECT_EQ(expected_double, double_vv);

    std::vector<float> out(4);
    map(out, lambda, vv, vv2);
    EXPECT_EQ(expected_double, out);
}
