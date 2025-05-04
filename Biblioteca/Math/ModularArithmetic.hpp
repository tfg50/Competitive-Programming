#ifndef MODULAR_ARITHMETIC_H
#define MODULAR_ARITHMETIC_H
#include "Biblioteca/template_basics.hpp"

typedef unsigned __int128 u128;
typedef __int128 i128;
typedef unsigned long long u64;
typedef long long i64;
typedef unsigned int u32;
typedef int i32;


// TODO: add static asserts

template <class T, class E>
constexpr T fexp(T x, E e) {
    T ans({1});
    for(; e > 0; e >>= 1) {
        if(e & 1) ans = ans * x;
        x = x * x;
    }
    return ans;
}

template <class LOW, class HIGH, const LOW mod>
struct modBase {
    using mint = modBase<LOW, HIGH, mod>;
    constexpr modBase() : val(0) {}
    // be careful of negative numbers!
    constexpr modBase(const LOW v) : val((v % mod + mod) % mod) {} 
    LOW val;

    #define add(a, b) a + b >= mod ? a + b - mod : a + b
    #define sub(a, b) a < b ? a + mod - b : a - b

    constexpr mint &operator += (const mint &o) { return val = add(val, o.val), *this; }
    constexpr mint &operator -= (const mint &o) { return val = sub(val, o.val), *this; }
    constexpr mint &operator *= (const mint &o) { return val = (LOW) ((HIGH) val * o.val % mod), *this; }
    constexpr mint &operator /= (const mint &o) { return *this *= o.inverse(); }

    constexpr mint operator + (const mint &b) const { return mint(*this) += b; }
    constexpr mint operator - (const mint &b) const { return mint(*this) -= b; }
    constexpr mint operator * (const mint &b) const { return mint(*this) *= b; }
    constexpr mint operator / (const mint &b) const { return mint(*this) /= b; }

    constexpr mint operator - () const { return mint() - mint(*this); }
    constexpr bool operator == (const mint &b) const { return val == b.val; }
    constexpr bool operator != (const mint &b) const { return val != b.val; }

    template<class E> constexpr mint pow (E e) const { return fexp(*this, e); }
    constexpr mint inverse() const { return pow(mod - 2); }
    constexpr LOW get() const { return val; }
    static constexpr LOW getMod() { return mod; }

    friend std::ostream& operator << (std::ostream &os, const mint &p) { return os << p.val; }
    friend std::istream& operator >> (std::istream &is, mint &p) { return is >> p.val; }
};

template<const u32 mod> using modBase32 = modBase<i32, i64, mod>;
template<const u64 mod> using modBase64 = modBase<i64, i128, mod>;

#endif