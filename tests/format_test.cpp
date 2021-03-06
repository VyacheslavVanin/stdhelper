#include "vvvstdhelper/containerhelper.hpp"
#include "vvvstdhelper/format.hpp"
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

TEST(format, map)
{
    const std::unordered_map<int, int> v = {
        {1, 100},
    };
    auto ret = vvv::helper::format("@", v);
    EXPECT_EQ(ret, "[(1, 100)]");
}

TEST(format, multimap)
{
    const std::unordered_multimap<int, int> v = {{1, 100}, {1, 200}};
    auto ret = vvv::helper::format("@", v);
    EXPECT_EQ(ret, "[(1, 200), (1, 100)]");
}

TEST(format, set)
{
    const std::unordered_set<int> v = {
        1,
    };
    auto ret = vvv::helper::format("@", v);
    EXPECT_EQ(ret, "[1]");
}

TEST(format, multiset)
{
    const std::unordered_multiset<int> v = {1, 1};
    auto ret = vvv::helper::format("@", v);
    EXPECT_EQ(ret, "[1, 1]");
}
