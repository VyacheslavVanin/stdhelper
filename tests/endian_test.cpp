#include "vvvstdhelper/endian.hpp"
#include <gtest/gtest.h>

TEST(endian, hton)
{
    const uint8_t x = 0x11u;
    const uint16_t a = 0x1122u;
    const uint32_t b = 0x11223344u;
    const uint64_t c = 0x1122334455667788ULL;
    EXPECT_EQ(0x11u, vvv::hton(x));
    EXPECT_EQ(0x2211u, vvv::hton(a));
    EXPECT_EQ(0x44332211u, vvv::hton(b));
    EXPECT_EQ(0x8877665544332211u, vvv::hton(c));
}

TEST(endian, ntoh)
{
    const uint8_t x = 0x11u;
    const uint16_t a = 0x1122u;
    const uint32_t b = 0x11223344u;
    const uint64_t c = 0x1122334455667788ULL;
    EXPECT_EQ(0x11u, vvv::ntoh(x));
    EXPECT_EQ(0x2211u, vvv::ntoh(a));
    EXPECT_EQ(0x44332211u, vvv::ntoh(b));
    EXPECT_EQ(0x8877665544332211u, vvv::ntoh(c));
}
