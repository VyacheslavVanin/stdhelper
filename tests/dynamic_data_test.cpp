#include <functional>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vvvstdhelper/containerhelper.hpp>
#include <vvvstdhelper/shared_function.hpp>

#include <gtest/gtest.h>

#include <vvvstdhelper/dynamic_data.hpp>

template <typename T>
using DD = vvv::DynamicData<T>;

TEST(DynData, base)
{
    DD<double> a = 10;
    DD<double> b = 100;
    auto c = a + b;
    ASSERT_EQ(10, a.get());
    ASSERT_EQ(100, b.get());
    ASSERT_EQ(110, c.get());
}

TEST(DynData, reassign)
{
    DD<double> a = 10;
    DD<double> b = 100;
    auto c = a + b;
    a = 20;
    ASSERT_EQ(20, a.get());
    ASSERT_EQ(100, b.get());
    ASSERT_EQ(120, c.get());

    a = 30;
    ASSERT_EQ(30, a.get());
    ASSERT_EQ(100, b.get());
    ASSERT_EQ(130, c.get());

    b = 2000;
    ASSERT_EQ(30, a.get());
    ASSERT_EQ(2000, b.get());
    ASSERT_EQ(2030, c.get());
}

TEST(DynData, temporary_vars)
{
    DD<double> a = 10;
    DD<double> b = 100;
    DD<double> c = 1000;
    auto d = (a + b) + 7 + (c + b);
    ASSERT_EQ(10, a.get());
    ASSERT_EQ(100, b.get());
    ASSERT_EQ(1000, c.get());
    ASSERT_EQ(1217, d.get());

    a = 30;
    ASSERT_EQ(1237, d.get());

    b = 2000;
    ASSERT_EQ(5037, d.get());
}

TEST(DynData, reassing_expr)
{
    DD<double> a = 10;
    DD<double> b = 100;
    auto c = a + b;
    b = a + 20;
    ASSERT_EQ(10, a.get());
    ASSERT_EQ(30, b.get());
    ASSERT_EQ(40, c.get());
}

TEST(DynData, unary)
{
    DD<double> a = 10;
    DD<double> b = 100;
    auto c = -(a + b);
    ASSERT_EQ(-110, c.get());
    b = a + 20;
    ASSERT_EQ(10, a.get());
    ASSERT_EQ(30, b.get());
    ASSERT_EQ(-40, c.get());
}

TEST(DynData, unary_math)
{
    double a_ref = M_PI / 4;
    double b_ref = sin(a_ref);
    DD<double> a = a_ref;
    auto b = sin(a);
    ASSERT_EQ(b_ref, b.get());
    ASSERT_EQ(a_ref, a.get());
}

TEST(DynData, binary_math)
{
    double a_ref = 3;
    double b_ref = 4;
    double c_ref = std::hypot(a_ref, b_ref);
    DD<double> a = a_ref;
    DD<double> b = b_ref;
    auto c = hypot(a, b);
    ASSERT_EQ(b_ref, b.get());
    ASSERT_EQ(a_ref, a.get());
    ASSERT_EQ(c_ref, c.get());
}

TEST(DynData, max_min)
{
    double a_ref = 3;
    double b_ref = 4;
    DD<double> a = a_ref;
    DD<double> b = b_ref;
    auto c = max(a, b);
    ASSERT_EQ(b_ref, b.get());
    ASSERT_EQ(a_ref, a.get());
    ASSERT_EQ(b.get(), c.get());

    a = 5;
    ASSERT_EQ(c.get(), a.get());
}

TEST(DynData, const_base)
{
    const DD<double> a = 10;
    const DD<double> b = 100;
    const auto c = a + b;

    ASSERT_EQ(10, a.get());
    ASSERT_EQ(100, b.get());
    ASSERT_EQ(110, c.get());
}

TEST(DynData, const_assign)
{
    const DD<double> a = 10;
    DD<double> b = 100;
    const auto c = a + b;
    auto d = c;

    ASSERT_EQ(10, a.get());
    ASSERT_EQ(100, b.get());
    ASSERT_EQ(110, c.get());
    ASSERT_EQ(110, d.get());

    b = 200;
    ASSERT_EQ(210, c.get());
    ASSERT_EQ(210, d.get());
}

using DDD = DD<double>;
using DDD_pair = std::pair<DDD, DDD>;

void Set(DDD_pair& p, DDD a, DDD b)
{
    p.first = std::move(a);
    p.second = p.first + std::move(b);
}

TEST(DynData, preserve_dependency)
{
    DDD_pair a;
    DDD_pair b;
    Set(a, 10, 20);
    Set(b, a.second + 10, 30);

    ASSERT_EQ(a.first.get(), 10);
    ASSERT_EQ(a.second.get(), 30);
    ASSERT_EQ(b.first.get(), 40);
    ASSERT_EQ(b.second.get(), 70);

    Set(a, 10, 40);
    ASSERT_EQ(a.first.get(), 10);
    ASSERT_EQ(a.second.get(), 50);
    ASSERT_EQ(b.first.get(), 60);
    ASSERT_EQ(b.second.get(), 90);
}
