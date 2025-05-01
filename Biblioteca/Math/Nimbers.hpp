#ifndef NIMBERS_H
#define NIMBERS_H
#include "Biblioteca/template_basics.hpp"

// mostly copied from https://judge.yosupo.jp/submission/20140

namespace nimbers {
    #define ctz __builtin_ctz
    typedef unsigned short u16;
    typedef unsigned int u32;
    typedef unsigned long long u64;
    constexpr u32 n2f[16] = {0x0001u, 0x8ff5u, 0x6cbfu, 0xa5beu, 0x761au, 0x8238u, 0x4f08u, 0x95acu, 0xf340u, 0x1336u, 0x7d5eu, 0x86e7u, 0x3a47u, 0xe796u, 0xb7c3u, 0x0008u},
                  f2n[16] = {0x0001u, 0x2827u, 0x392bu, 0x8000u, 0x20fdu, 0x4d1du, 0xde4au, 0x0a17u, 0x3464u, 0xe3a9u, 0x6d8du, 0x34bcu, 0xa921u, 0xa173u, 0x0ebcu, 0x0e69u};
    inline u32 nimber2field(u32 x) {u32 y = 0; for (; x; x &= x - 1) y ^= n2f[ctz(x)]; return y;}
    inline u32 field2nimber(u32 x) {u32 y = 0; for (; x; x &= x - 1) y ^= f2n[ctz(x)]; return y;}
    inline u32 __builtin_double(u32 x) {return x << 1 ^ (x < 0x8000u ? 0 : 0x1681fu);}

    u16 ln[65536], exp[131075], *Hexp = exp + 3, *H2exp = exp + 6;

    inline void init() {
        int i;
        for (*exp = i = 1; i < 65535; ++i) exp[i] = __builtin_double(exp[i - 1]);
        for (i = 1; i < 65535; ++i) exp[i] = field2nimber(exp[i]), ln[exp[i]] = i;
        memcpy(exp + 65535, exp, 131070);
        memcpy(exp + 131070, exp, 10);
    }

    inline u16 product(u16 A, u16 B) {return A && B ? exp[ln[A] + ln[B]] : 0;}
    inline u16 H(u16 A) {return A ? Hexp[ln[A]] : 0;}
    inline u16 H2(u16 A) {return A ? H2exp[ln[A]] : 0;}
    inline u16 Hproduct(u16 A, u16 B) {return A && B ? Hexp[ln[A] + ln[B]] : 0;}

    inline u32 product(u32 A, u32 B) {
        u16 a = A & 65535, b = B & 65535, c = A >> 16, d = B >> 16, e = product(a, b);
        return u32(product(u16(a ^ c), u16(b ^ d)) ^ e) << 16 | (Hproduct(c, d) ^ e);
    }

    inline u32 H(u32 A) {
        u16 a = A & 65535, b = A >> 16;
        return H(u16(a ^ b)) << 16 | H2(b);
    }
/*
    inline u64 product(u64 A, u64 B) {
        u32 a = A & UINT_MAX, b = B & UINT_MAX, c = A >> 32, d = B >> 32, e = product(a, b);
        return u64(product(a ^ c, b ^ d) ^ e) << 32 | (H(product(c, d)) ^ e);
    }*/
}

#endif