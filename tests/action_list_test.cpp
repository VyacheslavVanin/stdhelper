#include <gtest/gtest.h>
#include <vvvstdhelper/actionlist.hpp>
#include <vvvstdhelper/actionmap.hpp>
#include <vvvstdhelper/shared_function.hpp>

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

TEST(actionlist, aaaa)
{
    using vvv::SharedFunction;
    vvv::ActionList<void(void), SharedFunction<void(void)>> l;
    std::string out1;
    std::string out2;

    auto f = SharedFunction<void(void)>([&out1] { out1 = "hello"; });
    l.addAction(f);
    l.addAction([&out2] { out2 = "world"; });

    l();
    EXPECT_EQ(out1, "hello");
    EXPECT_EQ(out2, "world");

    out1.clear();
    out2.clear();
    l.remove(f);
    l();

    EXPECT_EQ(out1, "");
    EXPECT_EQ(out2, "world");
}
