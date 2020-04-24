#pragma once
#include <cmath>
#include <algorithm>

namespace vvv {
namespace functors {

#define VVV_UNARY_FUNCTOR(x)                                                   \
    template <typename T>                                                      \
    struct x {                                                                 \
        T operator()(const T& t) const { return std::x(t); }                   \
    };

VVV_UNARY_FUNCTOR(abs);
VVV_UNARY_FUNCTOR(acos);
VVV_UNARY_FUNCTOR(asin);
VVV_UNARY_FUNCTOR(atan);
VVV_UNARY_FUNCTOR(cbrt);
VVV_UNARY_FUNCTOR(ceil);
VVV_UNARY_FUNCTOR(cos);
VVV_UNARY_FUNCTOR(erf);
VVV_UNARY_FUNCTOR(exp);
VVV_UNARY_FUNCTOR(exp2);
VVV_UNARY_FUNCTOR(floor);
VVV_UNARY_FUNCTOR(log);
VVV_UNARY_FUNCTOR(log10);
VVV_UNARY_FUNCTOR(log1p);
VVV_UNARY_FUNCTOR(log2);
VVV_UNARY_FUNCTOR(round);
VVV_UNARY_FUNCTOR(sin);
VVV_UNARY_FUNCTOR(sqrt);
VVV_UNARY_FUNCTOR(tan);
VVV_UNARY_FUNCTOR(trunc);

#undef VVV_UNARY_FUNCTOR

#define VVV_BINARY_FUNCTOR(x)                                                  \
    template <typename T>                                                      \
    struct x {                                                                 \
        T operator()(const T& a, const T& b) const { return std::x(a, b); }    \
    };

VVV_BINARY_FUNCTOR(hypot)
VVV_BINARY_FUNCTOR(pow)
VVV_BINARY_FUNCTOR(fmod)
VVV_BINARY_FUNCTOR(remainder)
VVV_BINARY_FUNCTOR(copysign)
VVV_BINARY_FUNCTOR(fdim)
VVV_BINARY_FUNCTOR(max)
VVV_BINARY_FUNCTOR(min)

#undef VVV_BINARY_FUNCTOR

} // namespace functors
} // namespace vvv
