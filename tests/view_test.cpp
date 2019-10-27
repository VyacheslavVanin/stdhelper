#include "vvvstdhelper/view.hpp"
#include <gtest/gtest.h>

auto is_view_eq = [](const auto& a, const auto& view) {
    ASSERT_EQ(a.size(), view.size());
    ASSERT_EQ(a.begin(), view.begin());
    ASSERT_EQ(a.end(), view.end());
    for (size_t i = 0; i < a.size(); ++i)
        EXPECT_EQ(a[i], view[i]);
};

TEST(view, make_view)
{
    using vvv::make_view;
    const std::vector<int> a{0, 1, 2, 3};
    const auto& view = make_view(a);

    is_view_eq(a, view);
}

TEST(view, slice)
{
    using vvv::make_view;
    using vvv::sslice;
    const std::vector<int> a{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
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

TEST(view, split)
{
    using vvv::make_view;
    using vvv::split;
    const std::vector<int> a{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const auto& view = make_view(a);

    // split view of size 10 to 2 splits by 5 elements
    const auto& s2 = split(view, 2);
    ASSERT_EQ(2, s2.size());
    for (const auto& v : s2) {
        EXPECT_EQ(5, v.size());
    }

    // split view of size 10 to 5 splits by 2 elements
    const auto& s5 = split(view, 5);
    ASSERT_EQ(5, s5.size());
    for (const auto& v : s5) {
        EXPECT_EQ(2, v.size());
    }

    // split view of size 10 to 3 splits by 3 + 3 + 4 elements
    const auto& s3 = split(view, 3);
    ASSERT_EQ(3, s3.size());
    EXPECT_EQ(3, s3[0].size());
    EXPECT_EQ(3, s3[1].size());
    EXPECT_EQ(4, s3[2].size());
}

TEST(view, mutable_view)
{
    using vvv::make_view;
    using vvv::split;
    std::vector<int> a{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    auto view = make_view(a);

    // split view of size 10 to 3 splits by 3 + 3 + 4 elements
    auto ss = split(view, 3);

    // each split fill with it's number
    for (size_t i = 0; i < 3; ++i) {
        auto& s = ss[i];
        std::fill(s.begin(), s.end(), i);
    }

    const std::vector<int> expected{0, 0, 0, 1, 1, 1, 2, 2, 2, 2};
    ASSERT_EQ(expected, a);
}
