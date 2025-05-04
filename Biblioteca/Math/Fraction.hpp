#ifndef FRACTION_H
#define FRACTION_H
#include "Biblioteca/template_basics.hpp"

template<class T>
T gcd(T a, T b) { return b == 0 ? a : gcd(b, a % b); }

template<class T>
struct Frac {
    T p, q;
    Frac() {
        p = 0, q = 1;
    }
    Frac(T x) {
        p = x;
        q = 1;
    }
    Frac(T a, T b) {
        if(b == 0) {
            a = 0;
            b = 1;
        }
        p = a;
        q = b;
        fix();
    }
    Frac<T> operator + (Frac<T> o) const { return Frac(p * o.q + o.p * q, q * o.q); }
    Frac<T> operator - (Frac<T> o) const {return Frac(p * o.q - o.p * q, q * o.q); }
    Frac<T> operator * (Frac<T> o) const { return Frac(p * o.p, q * o.q); }
    Frac<T> operator / (Frac<T> o) const { return Frac(p * o.q, q * o.p); }

    void fix() {
        if(q < 0) {
            q = -q;
            p = -p;
        }
        auto g = gcd(std::max(p, -p), q);
        p /= g;
        q /= g;
    }

    bool operator < (Frac<T> o) const { return ((*this) - o).p < 0; }
    bool operator > (Frac<T> o) const { return ((*this) - o).p > 0; }

    friend std::ostream& operator << (std::ostream &os, const Frac<T> &f) {
        return os << f.p << '/' << f.q;
    }
    friend std::istream& operator >> (std::istream &is, Frac<T> &f) {
        char trash;
        return is >> f.p >> trash >> f.q;
    }
};

#endif
