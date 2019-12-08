#pragma once
#include <algorithm>
#include <stdint.h>

namespace vvv {
/// FIXME: add endianess check :-)

template <size_t S>
inline void swapbytes(uint8_t (&arr)[S])
{
    const size_t halfs = S / 2;
    for (size_t i = 0; i < halfs; ++i) {
        std::swap(arr[i], arr[S - i - 1]);
    }
}

template <typename T, size_t S = sizeof(T)>
inline T hton(T val)
{
    T ret = val;
    swapbytes(reinterpret_cast<uint8_t(&)[S]>(ret));
    return ret;
}

template <typename T, size_t S = sizeof(T)>
inline T ntoh(T val)
{
    T ret = val;
    swapbytes(reinterpret_cast<uint8_t(&)[S]>(ret));
    return ret;
}
} // namespace vvv
