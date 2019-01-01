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

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
