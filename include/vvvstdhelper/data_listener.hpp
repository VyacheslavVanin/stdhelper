#pragma once
#include <ostream>

namespace vvv {
template <typename T>
struct NoOp {
    void operator()(const T&) {}
};

/// Data that triggers action each time stored data modified
/// T - stored data type<br>
/// A - functor to be called on data modified
template <typename T, typename A = NoOp<T>>
class DataListener : protected A {
private:
public:
    using value_type = T;
    using actions_type = A;

    struct MutableProxy {
        using actions_type = A;

        ~MutableProxy() { actions(ref); }

        T& operator*() && { return ref; }
        T* operator->() && { return &ref; }

        bool operator==(const T& other) const { return ref == other; }

    private:
        MutableProxy(T& ref, actions_type& actions) : ref(ref), actions(actions)
        {
        }

        friend class DataListener<T, A>;
        T& ref;
        actions_type& actions;
    };

    DataListener() = default;
    DataListener(T value) : data(std::move(value)) {}

    template <typename A2>
    DataListener(const DataListener<T, A2>& other)
    {
        data = other.data;
    }

    template <typename A2>
    DataListener(DataListener<T, A2>&& other)
    {
        data = std::move(other.data);
    }

    DataListener& operator=(const T& other)
    {
        data = other;
        invoke(data);
        return *this;
    }

    DataListener& operator=(T&& other)
    {
        data = std::move(other);
        invoke(data);
        return *this;
    }

    template <typename A2>
    DataListener& operator=(const DataListener<T, A2>& other)
    {
        data = other.data;
        invoke(data);
        return *this;
    }

    template <typename A2>
    DataListener& operator=(DataListener<T, A2>&& other)
    {
        data = std::move(other.data);
        invoke(data);
        return *this;
    }

    template <typename A2>
    bool operator==(DataListener<T, A2>&& other) const
    {
        return data == std::move(other.data);
    }

    operator const T&() { return data; }
    operator T() && { return std::move(data); }

    const T& get() const { return data; }
    const T& operator*() const { return get(); }
    const T* operator->() const { return &get(); }
    MutableProxy operator->() { return MutableProxy(data, action()); }
    MutableProxy operator*() { return MutableProxy(data, action()); }

    void invoke() { invoke(data); }

    A& action() { return static_cast<A&>(*this); }

private:
    void invoke(const T& v) { action()(v); }

    T data;
};

template <typename T>
std::ostream& operator<<(std::ostream& str, const DataListener<T>& v)
{
    return str << *v;
}

template <typename T>
std::ostream& operator<<(std::ostream& str,
                         typename DataListener<T>::MutableProxy&& v)
{
    return str << *std::move(v);
}
} // namespace vvv
