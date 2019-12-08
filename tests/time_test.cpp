#include "vvvstdhelper/time.hpp"
#include <gtest/gtest.h>
#include <sstream>

TEST(time, print)
{
    vvv::TimePoint t0 = vvv::now();
    auto t1 = t0 + std::chrono::seconds(1);

    {
        std::stringstream ss;
        vvv::print_dt(ss, t0, t1);
        const auto str = ss.str();
        ASSERT_EQ("1000ms", str);
    }

    {
        std::stringstream ss;
        vvv::print_dt<std::chrono::seconds>(ss, t0, t1);
        const auto str = ss.str();
        ASSERT_EQ("1s", str);
    }

    {
        std::stringstream ss;
        vvv::print_dt<std::chrono::microseconds>(ss, t0, t1);
        const auto str = ss.str();
        ASSERT_EQ("1000000us", str);
    }

    {
        std::stringstream ss;
        vvv::print_dt<std::chrono::nanoseconds>(ss, t0, t1);
        const auto str = ss.str();
        ASSERT_EQ("1000000000ns", str);
    }
}
