#include <gtest/gtest.h>
#include <vvvstdhelper/data_listener.hpp>

TEST(data_listener, Static)
{
    using T = std::vector<size_t>;
    struct A {
        void operator()(const T&) { calls++; }
        int calls = 0;
    };

    vvv::DataListener<T, A> s;
    const auto& const_view = s;

    s = {42};
    ASSERT_EQ(1, s.action().calls);
    ASSERT_EQ(*const_view, std::vector<size_t>{42});

    s = {100500};
    ASSERT_EQ(2, s.action().calls);
    ASSERT_EQ(*const_view, std::vector<size_t>{100500});

    s->push_back(1321);
    ASSERT_EQ(3, s.action().calls);

    s->push_back(42);
    ASSERT_EQ(4, s.action().calls);

    s->push_back(22);
    ASSERT_EQ(5, s.action().calls);

    s->operator[](2) = 10000;
    ASSERT_EQ(6, s.action().calls);

    {
        const auto expected = std::vector<size_t>{100500, 1321, 10000, 22};
        ASSERT_EQ(*const_view, expected);
    }

    s->clear();
    ASSERT_EQ(7, s.action().calls);
    ASSERT_TRUE(const_view->empty());
    ASSERT_EQ(7, s.action().calls);

    T s2 = s;
    ASSERT_EQ(7, s.action().calls);
    ASSERT_EQ(*const_view, s2);
}
