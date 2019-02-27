#include "../src/containerhelper.hpp"
#include "../src/format.hpp"
#include <gtest/gtest.h>

TEST(format, single)
{
    auto ret = vvv::helper::format("hello @", "world");
    EXPECT_EQ(ret, "hello world");
}

TEST(format, multi)
{
    auto ret = vvv::helper::format("@ @", "hello", "world");
    EXPECT_EQ(ret, "hello world");
}

TEST(format, none_string)
{
    auto ret = vvv::helper::format("@ @", "hello", 42);
    EXPECT_EQ(ret, "hello 42");
}

TEST(format, no_placeholder)
{
    auto ret = vvv::helper::format("foo bar", "hello", 42);
    EXPECT_EQ(ret, "foo bar");
}

TEST(format, too_many_placeholders)
{
    auto ret = vvv::helper::format("@ @ @", "hello", 42);
    EXPECT_EQ(ret, "hello 42 @");
}

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

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
