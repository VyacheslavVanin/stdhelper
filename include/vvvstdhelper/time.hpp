#pragma once
#include <chrono>
#include <ostream>

namespace vvv {
using TimePoint = std::chrono::system_clock::time_point;

inline TimePoint now() { return std::chrono::system_clock::now(); }

template <typename D>
inline const char* duration_postfix()
{
    return "??";
}

template <>
inline const char* duration_postfix<std::chrono::seconds>()
{
    return "s";
}

template <>
inline const char* duration_postfix<std::chrono::milliseconds>()
{
    return "ms";
}

template <>
inline const char* duration_postfix<std::chrono::microseconds>()
{
    return "us";
}

template <>
inline const char* duration_postfix<std::chrono::nanoseconds>()
{
    return "ns";
}

template <>
inline const char* duration_postfix<std::chrono::minutes>()
{
    return "min";
}

template <>
inline const char* duration_postfix<std::chrono::hours>()
{
    return "h";
}

template <typename D = std::chrono::milliseconds>
inline void print_dt(std::ostream& str, TimePoint t0, TimePoint t1)
{
    const auto dt = std::chrono::duration_cast<D>(t1 - t0);
    str << dt.count() << duration_postfix<D>();
}
} // namespace vvv
