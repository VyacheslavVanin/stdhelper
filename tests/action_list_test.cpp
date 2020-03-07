#include "vvvstdhelper/actionlist.hpp"
#include "vvvstdhelper/actionmap.hpp"
#include <gtest/gtest.h>

TEST(actionlist, actionlist)
{
    vvv::ActionList<void(void)> l;
    std::string out1;
    std::string out2;

    l.addAction([&out1] { out1 = "hello"; });
    l.addAction([&out2] { out2 = "world"; });
    l();

    EXPECT_EQ(out1, "hello");
    EXPECT_EQ(out2, "world");
}

TEST(actionmap, actionmap)
{
    vvv::ActionMap<std::string, void(void)> m;
    std::string out1;
    std::string out2;

    m.addAction("action1", [&out1] { out1 = "hello"; });
    m.addAction("action2", [&out2] { out2 = "world"; });
    m.invoke("action1");

    EXPECT_EQ(out1, "hello");
    EXPECT_EQ(out2, "");

    m.invoke("action2");
    EXPECT_EQ(out2, "world");
}
