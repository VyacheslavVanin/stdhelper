#pragma once
#include "math_functors.hpp"
#include <cmath>
#include <iostream>
#include <memory>
#include <vvvstdhelper/actionlist.hpp>
#include <vvvstdhelper/data_listener.hpp>

namespace vvv {
template <typename T>
class DynamicData {
private:
    auto& action() const { return data->action(); }

    template <typename U>
    using Listener = vvv::DataListener<U, vvv::ActionList<void(U)>>;

public:
    using value_type = T;

    DynamicData() : DynamicData(T{}) {}
    DynamicData(T val) : data(std::make_shared<Listener<T>>(val)) {}
    DynamicData(const DynamicData& other)
        : data(std::make_shared<Listener<T>>()), binded{other.data},
          dependencies{other.dependencies}
    {
        bind();
    }

    DynamicData(DynamicData&& other)
        : data(std::make_shared<Listener<T>>()), binded{std::move(other.data)},
          dependencies{std::move(other.dependencies)}
    {
        bind();
    }

    DynamicData& operator=(const DynamicData& other)
    {
        binded = other.data;
        dependencies = other.dependencies;
        bind();
        return *this;
    }
    DynamicData& operator=(DynamicData&& other)
    {
        binded = std::move(other.data);
        dependencies = std::move(other.dependencies);
        bind();
        return *this;
    }

    DynamicData& operator=(const value_type& other)
    {
        *data = other;
        return *this;
    }

    DynamicData& operator=(value_type&& other)
    {
        *data = std::move(other);
        return *this;
    }

    auto& action() { return data->action(); }

    // explicit operator const T&() { return data->get(); }
    // explicit operator T() && { return std::move(data->get()); }

    const T& get() const { return data->get(); }
    const T& operator*() const { return get(); }
    const T* operator->() const { return &get(); }
    auto operator->() { return data->operator->(); }
    auto operator*() { return data->operator*(); }

    /// Numeric Unary Operators
    template <typename OP>
    DynamicData func() const
    {
        DynamicData ret;
        auto weak_ret = std::weak_ptr<Listener<T>>(ret.data);
        action().addAction([weak_ret](const value_type& v) mutable {
            auto ret = weak_ret.lock();
            if (!ret) {
                return;
            }
            *ret = OP()(v);
        });

        ret = OP()(get());
        ret.dependencies.push_back(data);
        return ret;
    }

    DynamicData operator-() { return func<std::negate<T>>(); }

    DynamicData operator+() { return *this; }

    /// Numeric Binary Operators
    template <typename OP, typename U>
    DynamicData func(U k) const
    {
        DynamicData ret;
        auto weak_ret = std::weak_ptr<Listener<T>>(ret.data);
        action().addAction([weak_ret, k](const value_type& v) mutable {
            auto ret = weak_ret.lock();
            if (!ret) {
                return;
            }
            *ret = OP()(v, k);
        });

        ret = OP()(get(), k);
        ret.dependencies.push_back(data);
        return ret;
    }

    template <typename U>
    DynamicData operator*(U k) const
    {
        return func<std::multiplies<T>>(k);
    }

    template <typename U>
    DynamicData operator/(U k) const
    {
        return func<std::divides<T>>(k);
    }

    template <typename U>
    DynamicData operator-(U k) const
    {
        return func<std::minus<T>>(k);
    }

    template <typename U>
    DynamicData operator+(U k) const
    {
        return func<std::plus<T>>(k);
    }

    /// Objects Binary Operators
    template <typename OP>
    DynamicData func(const DynamicData<T>& other) const
    {
        DynamicData ret;
        auto weak_ret = std::weak_ptr<Listener<T>>(ret.data);
        auto weak_this = std::weak_ptr<Listener<T>>(data);
        auto weak_other = std::weak_ptr<Listener<T>>(other.data);
        auto lambda = [weak_ret, weak_other](const value_type& v) mutable {
            auto other = weak_other.lock();
            if (!other) {
                std::cout << "!!!!!11111 " << v << "\n";
                return;
            }

            auto ret = weak_ret.lock();
            if (!ret) {
                std::cout << "!!!!!22222 " << v << "\n";
                return;
            }

            *ret = OP()(v, other->get());
        };
        action().addAction(std::move(lambda));

        auto lambda2 = [weak_ret, weak_this](const value_type& v) mutable {
            auto ret = weak_ret.lock();
            if (!ret) {
                std::cout << "!!!!!33333 " << v << "\n";
                return;
            }

            auto a = weak_this.lock();
            if (!a) {
                std::cout << "!!!!!44444 " << v << "\n";
                return;
            }

            *ret = OP()(a->get(), v);
        };
        other.action().addAction(std::move(lambda2));
        ret = OP()(get(), other.get());

        const auto deps_size =
            dependencies.size() + other.dependencies.size() + 2;
        ret.dependencies.reserve(deps_size);
        ret.dependencies.insert(ret.dependencies.end(), dependencies.begin(),
                                dependencies.end());
        ret.dependencies.insert(ret.dependencies.end(),
                                other.dependencies.begin(),
                                other.dependencies.end());
        ret.dependencies.push_back(other.data);
        ret.dependencies.push_back(data);
        return ret;
    }

    DynamicData operator*(const DynamicData& other) const
    {
        return func<std::multiplies<T>>(other);
    }
    DynamicData operator/(const DynamicData& other) const
    {
        return func<std::divides<T>>(other);
    }
    DynamicData operator-(const DynamicData& other) const
    {
        return func<std::minus<T>>(other);
    }
    DynamicData operator+(const DynamicData& other) const
    {
        return func<std::plus<T>>(other);
    }

private:
    void bind()
    {
        auto weak_data = std::weak_ptr<Listener<T>>(data);
        binded->action().addAction([weak_data](const value_type& v) mutable {
            auto ret = weak_data.lock();
            if (!ret) {
                return;
            }
            *ret = v;
        });
        *data = binded->get();
    }

    // "interface" data used to interact with other instances and to store data
    // leave till the end of life
    std::shared_ptr<Listener<T>> data;
    std::shared_ptr<Listener<T>> binded;
    std::vector<std::shared_ptr<Listener<T>>> dependencies;
};

#define VVV_UNARY_OPERATION(x)                                                 \
    template <typename T>                                                      \
    DynamicData<T> x(DynamicData<T>& in)                                       \
    {                                                                          \
        return in.template func<functors::x<T>>();                             \
    }

VVV_UNARY_OPERATION(abs);
VVV_UNARY_OPERATION(acos);
VVV_UNARY_OPERATION(asin);
VVV_UNARY_OPERATION(atan);
VVV_UNARY_OPERATION(cbrt);
VVV_UNARY_OPERATION(ceil);
VVV_UNARY_OPERATION(cos);
VVV_UNARY_OPERATION(erf);
VVV_UNARY_OPERATION(exp);
VVV_UNARY_OPERATION(exp2);
VVV_UNARY_OPERATION(floor);
VVV_UNARY_OPERATION(log);
VVV_UNARY_OPERATION(log10);
VVV_UNARY_OPERATION(log1p);
VVV_UNARY_OPERATION(log2);
VVV_UNARY_OPERATION(round);
VVV_UNARY_OPERATION(sin);
VVV_UNARY_OPERATION(sqrt);
VVV_UNARY_OPERATION(tan);
VVV_UNARY_OPERATION(trunc);

#undef VVV_UNARY_OPERATION

#define VVV_BINARY_OPERATION(x)                                                \
    template <typename T>                                                      \
    DynamicData<T> x(DynamicData<T>& a, DynamicData<T>& b)                     \
    {                                                                          \
        return a.template func<functors::x<T>>(b);                             \
    }

VVV_BINARY_OPERATION(hypot)
VVV_BINARY_OPERATION(pow)
VVV_BINARY_OPERATION(fmod)
VVV_BINARY_OPERATION(remainder)
VVV_BINARY_OPERATION(copysign)
VVV_BINARY_OPERATION(fdim)
VVV_BINARY_OPERATION(max)
VVV_BINARY_OPERATION(min)

#undef VVV_BINARY_OPERATION
} // namespace vvv
