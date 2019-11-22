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

namespace std {
OSTREAM_OPERATOR_DEF(std::vector);
OSTREAM_OPERATOR_DEF(std::unordered_set);
OSTREAM_OPERATOR_DEF(std::unordered_multiset);
OSTREAM_OPERATOR_2_DEF(std::vector);
OSTREAM_OPERATOR_2_DEF(std::unordered_map);
OSTREAM_OPERATOR_2_DEF(std::unordered_multimap);
OSTREAM_OPERATOR_DEF(std::set);
OSTREAM_OPERATOR_DEF(std::multiset);
OSTREAM_OPERATOR_2_DEF(std::map);
OSTREAM_OPERATOR_2_DEF(std::multimap);

template <class T1, class T2>
inline std::ostream& operator<<(std::ostream& str, const std::pair<T1, T2>& p)
{
    str << "(";
    str << p.first << ", ";
    str << p.second;
    str << ")";
    return str;
}
} // namespace std

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

namespace std {
OSTREAM_OPERATOR(std::vector);
OSTREAM_OPERATOR_2(std::vector);
OSTREAM_OPERATOR(std::unordered_set);
OSTREAM_OPERATOR(std::unordered_multiset);
OSTREAM_OPERATOR_2(std::unordered_map);
OSTREAM_OPERATOR_2(std::unordered_multimap);
OSTREAM_OPERATOR(std::set);
OSTREAM_OPERATOR(std::multiset);
OSTREAM_OPERATOR_2(std::map);
OSTREAM_OPERATOR_2(std::multimap);

template <typename T>
std::vector<T> operator+(const std::vector<T>& l, const std::vector<T>& r)
{
    std::vector<T> ret = l;
    std::copy(r.begin(), r.end(), std::back_inserter(ret));
    return ret;
}
} // namespace std

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

template <typename C, typename C2, typename P,
          typename T = typename C::value_type,
          typename T2 = typename C2::value_type,
          typename R = std::result_of_t<P && (T&&, T2&&)>,
          typename RC = std::vector<R>>
inline RC map(const P& p, const C& xs, const C2& ys)
{
    // TODO: reimplement via some sort of zip iterator
    // TODO: support more types
    RC ret;
    const auto ret_size = std::min(xs.size(), ys.size());
    ret.reserve(ret_size);
    for (size_t i = 0; i < ret_size; ++i)
        ret.push_back(p(xs[i], ys[i]));

    return ret;
}

template <typename RC, typename C, typename P,
          typename T = typename C::value_type,
          typename RT = typename RC::value_type,
          typename PCALLABLE = std::result_of_t<P && (T &&)>>
inline void map(RC& ret, const P& p, const C& xs)
{
    const auto ret_size = std::min({xs.size(), ret.size()});
    for (size_t i = 0; i < ret_size; ++i)
        ret[i] = p(xs[i]);
}

template <typename RC, typename C, typename C2, typename P,
          typename T = typename C::value_type,
          typename T2 = typename C2::value_type,
          typename RT = typename RC::value_type,
          typename PCALLABLE = std::result_of_t<P && (T&&, T2&&)>>
inline void map(RC& ret, const P& p, const C& xs, const C2& ys)
{
    const auto ret_size = std::min({xs.size(), ys.size(), ret.size()});
    for (size_t i = 0; i < ret_size; ++i)
        ret[i] = p(xs[i], ys[i]);
}

template <typename RC, typename C, typename C2, typename C3, typename P,
          typename T = typename C::value_type,
          typename T2 = typename C2::value_type,
          typename T3 = typename C3::value_type,
          typename RT = typename RC::value_type,
          typename PCALLABLE = std::result_of_t<P && (T&&, T2&&, T3&&)>>
inline void map(RC& ret, const P& p, const C& xs, const C2& ys, const C3& zs)
{
    const auto ret_size =
        std::min({xs.size(), ys.size(), zs.size(), ret.size()});
    for (size_t i = 0; i < ret_size; ++i)
        ret[i] = p(xs[i], ys[i], zs[i]);
}

template <typename T, template <typename> class A,
          template <typename, typename> class C, typename P,
          typename R = std::result_of_t<P && (T &&)>>
inline C<R, A<R>> map(const P& p, const C<T, A<T>>& xs)
{
    C<R, A<R>> ret;
    ret.reserve(xs.size());
    for (const auto& x : xs)
        ret.push_back(p(x));

    return ret;
}

static auto getFirst = [](const auto& p) -> const auto& { return p.first; };
static auto getSecond = [](const auto& p) -> const auto& { return p.second; };

} // namespace helpers
} // namespace vvv
