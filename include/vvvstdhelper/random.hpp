#pragma once
#include <random>

namespace vvv {

template <typename T, typename = std::enable_if_t<std::is_scalar<T>::value>>
inline T random(T min, T max)
{
    static std::random_device r;
    static std::default_random_engine e1(r());
    std::uniform_int_distribution<T> uniform_dist(min, max);
    return uniform_dist(e1);
}

template <typename T,
          typename = std::enable_if_t<std::is_floating_point<T>::value>>
inline T random_real(T min, T max)
{
    static std::random_device r;
    static std::default_random_engine e1(r());
    std::uniform_real_distribution<T> uniform_dist(min, max);
    return uniform_dist(e1);
}

template <>
inline float random<float>(float min, float max)
{
    return random_real<float>(min, max);
}
template <>
inline double random<double>(double min, double max)
{
    return random_real<double>(min, max);
}

template <typename T>
struct Random {
    Random(T min, T max) : min(min), max(max) {}
    T operator()() { return vvv::random(min, max); }
    T min;
    T max;
};
} // namespace vvv
