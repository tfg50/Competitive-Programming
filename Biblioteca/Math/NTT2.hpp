#ifndef NTT2_H
#define NTT2_H
#include "Biblioteca/Math/ModularArithmetic.hpp"

template<class Num, const int me = 22>
struct DFT {
    static constexpr int ms = 1 << me;
    inline static Num root[ms];
 
    static void init() {
        static bool initialized = false;
        if(!initialized) {
            Num gen = getRoot(ms, Num());
            int order = ms;
            initialized = true;
            root[1] = 1;
            for(int len = 2; len < ms; len += len) {
                Num z = fexp(gen, order / len / 2);
                for(int i = len / 2; i < len; i++) {
                    root[2 * i] = root[i];
                    root[2 * i + 1] = root[i] * z;
                }
            }
        }
    }
 
    static std::vector<Num> dft(std::vector<Num> a, bool inv = false) {
        init();
        int n = (int) a.size();
        if(inv) {
            std::reverse(a.begin() + 1, a.end());
        }
        for(int i = 0, j = 0; i < n; i++) {
            if(i < j) std::swap(a[i], a[j]);
            int bit = n >> 1;
            while(j & bit) {
                j ^= bit;
                bit >>= 1;
            }
            j ^= bit;
        }
        for(int len = 1; len < n; len *= 2) {
            for(int i = 0; i < n; i += len * 2) {
                for(int j = 0; j < len; j++) {
                    Num u = a[i + j], v = a[i + j + len] * root[len + j];
                    a[i + j] = u + v;
                    a[i + j + len] = u - v;
                }
            }
        }
        if(inv) {
            Num val = Num(1) / Num(n);
            for(int i = 0; i < n; i++) {
                a[i] = a[i] * val;
            }
        }
        return a;
    }
 
    static std::vector<Num> mult(const std::vector<Num> &a, const std::vector<Num> &b) {
        auto A = dft(a), B = a == b ? A : dft(b);
        for(int i = 0; i < (int) a.size(); i++) {
            A[i] = A[i] * B[i];
        }
        return dft(A, true);
    }
 
    static std::vector<Num> mult(const std::vector<int> &a, const std::vector<int> &b) {
        std::vector<Num> A(a.size()), B(b.size());
        for(int i = 0; i < (int) a.size(); i++) {
            A[i] = a[i];
            B[i] = b[i] == a[i] ? A[i] : Num(b[i]);
        }
        return mult(A, B);
    }
};

using mint = modBase32<998244353>;
using ntt = DFT<mint>;

mint getRoot(int ord, mint x) {
    return mint(3).pow((mint::getMod() - 1) / ord);
}

void fix(std::vector<mint> &a) {
    while(!a.empty() && a.back().val == 0) {
        a.pop_back();
    }
}

std::vector<mint> operator +(std::vector<mint> a, const std::vector<mint> &b) {
    int sz = (int) std::max(a.size(), b.size());
    a.resize(sz, 0);
    for(int i = 0; i < (int) b.size(); i++) {
        a[i] = a[i] + b[i];
    }
    return a;
}

std::vector<mint> operator -(std::vector<mint> a, const std::vector<mint> &b) {
    int sz = (int) std::max(a.size(), b.size());
    a.resize(sz, 0);
    for(int i = 0; i < (int) b.size(); i++) {
        a[i] = a[i] - b[i];
    }
    return a;
}

std::vector<mint> operator *(std::vector<mint> a, std::vector<mint> b) {
    while(!a.empty() && a.back() == 0) a.pop_back();
    while(!b.empty() && b.back() == 0) b.pop_back();
    if(a.empty() || b.empty()) return std::vector<mint>(0, 0);
    int n = 1;
    while(n-1 < (int) a.size() + (int) b.size() - 2) n += n;
    a.resize(n, 0);
    b.resize(n, 0);
    a = DFT<mint>::mult(a, b);
    fix(a);
    return a;
}

#endif