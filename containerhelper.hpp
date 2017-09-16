#ifndef VVVSTLHELPER_H
#define VVVSTLHELPER_H
#include <algorithm>
#include <string>
#include <vector>
#include <stack>
#include <queue>

namespace vvv {
namespace helpers {

template <
    typename C, class P,
    typename = typename std::enable_if_t<std::is_rvalue_reference<C&&>::value>>
inline decltype(auto) filter(C&& v, P p)
{
    const auto n = std::copy_if(v.begin(), v.end(), v.begin(), p);
    v.resize(std::distance(v.begin(), n));
    return std::move(v);
}

template <class C, class P>
inline C filter(const C& v, P p)
{
    C ret;
    std::copy_if(v.begin(), v.end(), std::inserter(ret, ret.end()), p);
    return ret;
}

template <class C, class P>
inline bool any_of(const C& c, const P& p)
{
    return std::any_of(c.begin(), c.end(), p);
}

template <class C, class V>
inline bool contain(const C& c, const V& v)
{
    return any_of(c, [&v](const auto& e) { return e == v; });
}

template <typename T>
T extract(std::stack<T>& c)
{
    T ret = std::move(c.top());
    c.pop();
    return ret;
}

template <typename T>
T extract(std::queue<T>& c)
{
    T ret = std::move(c.front());
    c.pop();
    return ret;
}

}
}

#endif
