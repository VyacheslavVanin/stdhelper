#include "vvvstdhelper/containerhelper.hpp"
#include "vvvstdhelper/format.hpp"
#include "vvvstdhelper/view.hpp"
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

TEST(format, vector)
{
    const std::vector<int> v = {1, 2, 3, 42, 100500};
    auto ret = vvv::helper::format("@", v);
    EXPECT_EQ(ret, "[1, 2, 3, 42, 100500]");

    const std::vector<std::string> vs = {"aaa", "bbb", "hello"};
    auto rets = vvv::helper::format("@", vs);
    EXPECT_EQ(rets, "[aaa, bbb, hello]");
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

auto is_view_eq = [](const auto& a, const auto& view) {
    ASSERT_EQ(a.size(), view.size());
    ASSERT_EQ(a.begin(), view.begin());
    ASSERT_EQ(a.end(), view.end());
    for (size_t i = 0; i < a.size(); ++i)
        EXPECT_EQ(a[i], view[i]);
};

TEST(view, make_view) {
    using vvv::make_view;
    std::vector<int> a{0,1,2,3};
    const auto& view = make_view(a);

    is_view_eq(a, view);
}

TEST(view, slice) {
    using vvv::make_view;
    using vvv::sslice;
    std::vector<int> a{0,1,2,3,4,5,6,7,8,9};
    const auto& view = make_view(a);
    const auto& whole_s = sslice(view, 0, a.size());
    is_view_eq(a, whole_s);

    const auto& view123 = sslice(view, 1, 4);
    EXPECT_EQ(3, view123.size());
    EXPECT_EQ(1, view123[0]);
    EXPECT_EQ(2, view123[1]);
    EXPECT_EQ(3, view123[2]);

    const auto& view789 = sslice(view, 7, 100500);
    EXPECT_EQ(3, view789.size());
    EXPECT_EQ(7, view789[0]);
    EXPECT_EQ(8, view789[1]);
    EXPECT_EQ(9, view789[2]);
}

TEST(view, split) {
    using vvv::make_view;
    using vvv::split;
    std::vector<int> a{0,1,2,3,4,5,6,7,8,9};
    const auto& view = make_view(a);

    // split view of size 10 to 2 splits by 5 elements
    const auto& s2 = split(view, 2);
    ASSERT_EQ(2, s2.size());
    for (const auto& v: s2) {
        EXPECT_EQ(5, v.size());
    }

    // split view of size 10 to 5 splits by 2 elements
    const auto& s5 = split(view, 5);
    ASSERT_EQ(5, s5.size());
    for (const auto& v: s5) {
        EXPECT_EQ(2, v.size());
    }

    // split view of size 10 to 3 splits by 3 + 3 + 4 elements
    const auto& s3 = split(view, 3);
    ASSERT_EQ(3, s3.size());
    EXPECT_EQ(3, s3[0].size());
    EXPECT_EQ(3, s3[1].size());
    EXPECT_EQ(4, s3[2].size());
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
