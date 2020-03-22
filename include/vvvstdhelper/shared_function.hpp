#pragma once
#include <functional>
#include <memory>

namespace vvv {

/// Same as std::function but copied object is == original
template <typename T>
class SharedFunction {
public:
    using func_type = std::function<T>;

    template<typename U>
    SharedFunction(U&& f)
        : f_ptr(std::make_shared<func_type>(std::forward<U>(f)))
    {
    }

    explicit operator bool() const noexcept
    {
        return f_ptr && *f_ptr;
    }

    template <typename... Args>
    decltype(auto) operator()(Args&&... args) const
    {
        return (*f_ptr)(std::forward<Args>(args)...);
    }

    bool operator==(const SharedFunction<T>& other) const {
        return &other == this || other.f_ptr == f_ptr;
    }

private:
    std::shared_ptr<std::function<T>> f_ptr;
};

}
