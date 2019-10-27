#pragma once
#include <cassert>
#include <vector>

namespace vvv {
/// Mutable version of View class
template <typename Iterator>
class VarView {
public:
    using value_type = typename std::iterator_traits<Iterator>::value_type;
    VarView(Iterator begin, Iterator end) : begin_(begin), end_(end) {}

    const Iterator begin() const { return begin_; }
    const Iterator end() const { return end_; }
    Iterator begin() { return begin_; }
    Iterator end() { return end_; }
    size_t size() const { return std::distance(begin(), end()); }
    bool empty() const { return end_ == begin_; }
    const auto& operator[](size_t i) const { return *(begin() + i); }
    auto& operator[](size_t i) { return *(begin() + i); }

    bool operator==(const VarView& other) const
    {
        const auto& This = *this;
        const auto& s = size();
        if (s != other.size())
            return false;

        for (size_t i = 0; i < s; ++i)
            if (This[i] != other[i])
                return false;

        return true;
    }

    bool operator!=(const VarView& other) const { return !operator==(other); }

private:
    Iterator begin_;
    Iterator end_;
};

template <typename Iterator>
class View {
public:
    using value_type = typename std::iterator_traits<Iterator>::value_type;
    View(Iterator begin, Iterator end) : begin_(begin), end_(end) {}
    View(const VarView<Iterator>& vv) : begin_(vv.begin()), end_(vv.end()) {}

    Iterator begin() const { return begin_; }
    Iterator end() const { return end_; }
    size_t size() const { return std::distance(begin(), end()); }
    bool empty() const { return end_ == begin_; }
    auto operator[](size_t i) const { return *(begin() + i); }

    bool operator==(const View& other) const
    {
        const auto& This = *this;
        const auto& s = size();
        if (s != other.size())
            return false;

        for (size_t i = 0; i < s; ++i)
            if (This[i] != other[i])
                return false;

        return true;
    }

    bool operator!=(const View& other) const { return !operator==(other); }

private:
    Iterator begin_;
    Iterator end_;
};

template <typename T>
inline auto make_view(const T& t)
{
    return View<decltype(t.begin())>(t.begin(), t.end());
}
template <typename T>
inline auto make_view(T& t)
{
    return VarView<decltype(t.begin())>(t.begin(), t.end());
}

/// Fast slice no runtime checks performed
/// @param begin starting index
/// @param end ending index
/// @return view containing elements with indices within this [begin, end) range
template <template <typename> class V, typename T>
inline View<T> slice(const V<T>& t, size_t begin, size_t end)
{
    const auto a = t.begin() + begin;
    const auto b = t.begin() + end;
    assert(std::distance(t.begin(), b) <= t.size());
    return View<T>(a, b);
}
template <typename T>
inline VarView<T> slice(VarView<T>& t, size_t begin, size_t end)
{
    const auto a = t.begin() + begin;
    const auto b = t.begin() + end;
    assert(std::distance(t.begin(), b) <= t.size());
    return VarView<T>(a, b);
}

template <template <typename> class V, typename T>
inline bool is_valid_index(const V<T>& t, size_t index)
{
    return index < t.size();
}

/// Safe slice. Same as slise but if begin or end greater than t.end() then
/// parameters truncated to valid index
template <template <typename> class V, typename T>
inline View<T> sslice(const V<T>& t, size_t begin, size_t end)
{
    const auto tend = t.end();
    const auto a = is_valid_index(t, begin) ? t.begin() + begin : tend;
    const auto b = is_valid_index(t, end) ? t.begin() + end : tend;
    return View<T>(a, b);
}
template <typename T>
inline VarView<T> sslice(VarView<T>& t, size_t begin, size_t end)
{
    const auto tend = t.end();
    const auto a = is_valid_index(t, begin) ? t.begin() + begin : tend;
    const auto b = is_valid_index(t, end) ? t.begin() + end : tend;
    return VarView<T>(a, b);
}

/// Split view into p\ splits views. If view \v cannot be splitted to equal
/// splits then reminded will be added to last split
template <template <typename> class V, typename T>
inline std::vector<View<T>> split(const V<T>& v, size_t splits = 2)
{
    const auto size = v.size();
    const auto split_size = size / splits;
    const auto split_rem = size % splits;

    std::vector<View<T>> ret;
    ret.reserve(splits);
    for (size_t i = 0; i < splits; ++i)
        ret.push_back(slice(v, i * split_size, (i + 1) * split_size));

    if (split_rem) {
        const auto old_back = ret.back();
        ret.back() = View<T>(old_back.begin(), old_back.end() + split_rem);
    }

    return ret;
}
template <typename T>
inline std::vector<VarView<T>> split(VarView<T>& v, size_t splits = 2)
{
    const auto size = v.size();
    const auto split_size = size / splits;
    const auto split_rem = size % splits;

    std::vector<VarView<T>> ret;
    ret.reserve(splits);
    for (size_t i = 0; i < splits; ++i)
        ret.push_back(slice(v, i * split_size, (i + 1) * split_size));

    if (split_rem) {
        const auto old_back = ret.back();
        ret.back() = VarView<T>(old_back.begin(), old_back.end() + split_rem);
    }

    return ret;
}
} // namespace vvv
