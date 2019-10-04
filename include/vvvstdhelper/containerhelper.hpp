#pragma once
#include <algorithm>
#include <map>
#include <ostream>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define OSTREAM_OPERATOR_DEF(TYPE)                                             \
    template <typename T>                                                      \
    inline std::ostream& operator<<(std::ostream& str, const TYPE<T>& v);

#define OSTREAM_OPERATOR_2_DEF(TYPE)                                           \
    template <typename K, typename V>                                          \
    inline std::ostream& operator<<(std::ostream& str, const TYPE<K, V>& v);

OSTREAM_OPERATOR_DEF(std::vector);
OSTREAM_OPERATOR_DEF(std::unordered_set);
OSTREAM_OPERATOR_2_DEF(std::vector);
OSTREAM_OPERATOR_2_DEF(std::unordered_map);
OSTREAM_OPERATOR_DEF(std::set);
OSTREAM_OPERATOR_2_DEF(std::map);

template <class T1, class T2>
inline std::ostream& operator<<(std::ostream& str, const std::pair<T1, T2>& p)
{
    str << "(";
    str << p.first << ", ";
    str << p.second;
    str << ")";
    return str;
}

template <typename T>
inline std::ostream& pprint_impl(const T& v, std::ostream& str)
{
    if (v.empty()) {
        str << "[]";
        return str;
    }

    str << "[";
    for (auto i = std::begin(v); i != std::end(v); ++i) {
        str << *i;
        if (std::next(i) != std::end(v))
            str << ", ";
    }
    str << "]";

    return str;
}

#define OSTREAM_OPERATOR(TYPE)                                                 \
    template <typename T>                                                      \
    inline std::ostream& operator<<(std::ostream& str, const TYPE<T>& v)       \
    {                                                                          \
        return pprint_impl(v, str);                                            \
    }

#define OSTREAM_OPERATOR_2(TYPE)                                               \
    template <typename K, typename V>                                          \
    inline std::ostream& operator<<(std::ostream& str, const TYPE<K, V>& v)    \
    {                                                                          \
        return pprint_impl(v, str);                                            \
    }

OSTREAM_OPERATOR(std::vector);
OSTREAM_OPERATOR_2(std::vector);
OSTREAM_OPERATOR(std::unordered_set);
OSTREAM_OPERATOR_2(std::unordered_map);
OSTREAM_OPERATOR(std::set);
OSTREAM_OPERATOR_2(std::map);

template <typename T>
std::vector<T> operator+(const std::vector<T>& l, const std::vector<T>& r)
{
    std::vector<T> ret = l;
    std::copy(r.begin(), r.end(), std::back_inserter(ret));
    return ret;
}

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
inline bool contain(const std::initializer_list<C>& c, const V& v)
{
    return any_of(c, [&v](const auto& e) { return e == v; });
}

template <class C, class V>
inline bool contain(const C& c, const V& v)
{
    return any_of(c, [&v](const auto& e) { return e == v; });
}

template <typename T>
inline T extract(std::stack<T>& c)
{
    T ret = std::move(c.top());
    c.pop();
    return ret;
}

template <typename T>
inline T extract(std::queue<T>& c)
{
    T ret = std::move(c.front());
    c.pop();
    return ret;
}

template <typename T, typename P>
inline std::vector<std::result_of_t<P && (T &&)>>
map(const P& p, const std::initializer_list<T>& xs)
{
    using U = std::result_of_t<P && (T &&)>;
    std::vector<U> ret;
    ret.reserve(xs.size());
    for (const auto& x : xs)
        ret.push_back(p(x));

    return ret;
}

template <typename T, template <typename> class C, typename P>
inline C<std::result_of_t<P && (T &&)>> map(const P& p, const C<T>& xs)
{
    using U = std::result_of_t<P && (T &&)>;
    C<U> ret;
    ret.reserve(xs.size());
    for (const auto& x : xs)
        ret.push_back(p(x));

    return ret;
}

template <typename T, template <typename> class A,
          template <typename, typename> class C, typename P>
inline C<std::result_of_t<P && (T &&)>, A<std::result_of_t<P && (T &&)>>>
map(const P& p, const C<T, A<T>>& xs)
{
    using U = std::result_of_t<P && (T &&)>;
    C<U, A<U>> ret;
    ret.reserve(xs.size());
    for (const auto& x : xs)
        ret.push_back(p(x));

    return ret;
}

static auto getFirst = [](const auto& p) -> const auto& { return p.first; };
static auto getSecond = [](const auto& p) -> const auto& { return p.second; };

} // namespace helpers
} // namespace vvv
