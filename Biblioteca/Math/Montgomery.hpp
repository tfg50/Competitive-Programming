#ifndef MONTGOMERY_H
#define MONTGOMERY_H
#include "Biblioteca/template_basics.hpp"

template<class LOW, class HIGH, const int bits, const LOW mod>
struct Montgomery {
    using mint = Montgomery<LOW, HIGH, bits, mod>;
 
    static constexpr LOW getInv() {
        LOW ans = 1;
        for (int i = 0; i < 7; i++)
            ans *= 2 - mod * ans;
        return ans;
    }
 
    static constexpr LOW getR2()  {
        LOW ans = -mod % mod;
        for (int i = 0; i < bits; i++) {
            ans <<= 1;
            if (ans >= mod) { ans -= mod; }
        }
        return ans;
    }
 
    static constexpr LOW reduce(HIGH x) {
        LOW q = (LOW) x * inv;
        LOW a = (LOW) (x >> bits) - (LOW) (((HIGH) q * mod) >> bits);
        if (a > mod)
            a += mod;
        return a;
    }
 
    static constexpr LOW mult(LOW a, LOW b) { return reduce((HIGH) a * b); }
    static constexpr LOW inv = getInv(), r2 = getR2();
 
    constexpr Montgomery() : val(0) {}
    constexpr Montgomery(const LOW x) : val(mult(x % mod, r2)) {}

    #define add(a, b) a + b >= mod ? a + b - mod : a + b
    #define sub(a, b) a < b ? a + mod - b : a - b
 
    constexpr mint &operator += (const mint &o) { return val = add(val, o.val), *this; }
    constexpr mint &operator -= (const mint &o) { return val = sub(val, o.val), *this; }
    constexpr mint &operator *= (const mint &o) { return val = reduce((HIGH) val * o.val), *this; }
    constexpr mint &operator /= (const mint &o) { return *this *= o.inverse(); }

    constexpr mint operator + (const mint &b) const { return mint(*this) += b; }
    constexpr mint operator - (const mint &b) const { return mint(*this) -= b; }
    constexpr mint operator * (const mint &b) const { return mint(*this) *= b; }
    constexpr mint operator / (const mint &b) const { return mint(*this) /= b; }

    constexpr mint operator - () const { return mint() - mint(*this); }
    constexpr bool operator == (const mint &b) const { return val == b.val; }
    constexpr bool operator != (const mint &b) const { return val != b.val; }

    template<class E> constexpr mint pow(E n) const { return fexp(*this, n); }
    constexpr mint inverse() const { return pow(mod-2); }
    constexpr LOW get() const { return reduce(val); }
    static constexpr LOW getMod() { return mod; }

    friend std::ostream& operator << (std::ostream &os, const mint &p) { return os << p.get(); }
    friend std::istream& operator >> (std::istream &is, mint &p) {
        LOW x;
        is >> x;
        p = mint(x);
        return is;
    }
private:
    LOW val;
};

template<const u32 mod> using Montgomery32 = Montgomery<uint32_t, uint64_t, 32, mod>;

#endif