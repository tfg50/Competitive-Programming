#ifndef NTT_H
#define NTT_H
#include "Biblioteca/template_basics.hpp"

// copy fexp from PrimitiveRoot.cpp

const int MOD = 998244353;
const int me = 15;
const int ms = 1 << me;

#define add(x, y) x+y>=MOD?x+y-MOD:x+y

const int gen = 3; // use search() from PrimitiveRoot.cpp if MOD isn't 998244353
int bits[ms], root[ms];

void initFFT() {
    root[1] = 1;
    for(int len = 2; len < ms; len += len) {
        int z = (int) fexp(gen, (MOD - 1) / len / 2);
        for(int i = len / 2; i < len; i++) {
            root[2 * i] = root[i];
            root[2 * i + 1] = (int)((long long) root[i] * z % MOD);
        }
    }
}

void pre(int n) {
    int LOG = 0;
    while(1 << (LOG + 1) < n) {
        LOG++;
    }
    for(int i = 1; i < n; i++) {
        bits[i] = (bits[i >> 1] >> 1) | ((i & 1) << LOG);
    }
}

std::vector<int> fft(std::vector<int> a, bool inv = false) {
    int n = (int) a.size();
    pre(n);
    if(inv) {
        std::reverse(a.begin() + 1, a.end());
    }
    for(int i = 0; i < n; i++) {
        int to = bits[i];
        if(i < to) { std::swap(a[i], a[to]); }
    }
    for(int len = 1; len < n; len *= 2) {
        for(int i = 0; i < n; i += len * 2) {
            for(int j = 0; j < len; j++) {
                int u = a[i + j], v = (int)((long long) a[i + j + len] * root[len + j] % MOD);
                a[i + j] = add(u, v);
                a[i + j + len] = add(u, MOD - v);
            }
        }
    }
    if(inv) {
        long long rev = fexp(n, MOD-2, MOD);
        for(int i = 0; i < n; i++)
            a[i] = (int)(a[i] * rev % MOD);
    }
    return a;
}

std::vector<int> shift(const std::vector<int> &a, int s) {
    int n = std::max(0, s + (int) a.size());
    std::vector<int> b(n, 0);
    for(int i = std::max(-s, 0); i < (int) a.size(); i++) {
        b[i + s] = a[i];
    }
    return b;
}

std::vector<int> cut(const std::vector<int> &a, int n) {
    std::vector<int> b(n, 0);
    for(int i = 0; i < (int) a.size() && i < n; i++) {
        b[i] = a[i];
    }
    return b;
}

std::vector<int> operator +(std::vector<int> a, const std::vector<int> &b) {
    int sz = (int) std::max(a.size(), b.size());
    a.resize(sz, 0);
    for(int i = 0; i < (int) b.size(); i++) {
        a[i] = add(a[i], b[i]);
    }
    return a;
}

std::vector<int> operator -(std::vector<int> a, const std::vector<int> &b) {
    int sz = (int) std::max(a.size(), b.size());
    a.resize(sz, 0);
    for(int i = 0; i < (int) b.size(); i++) {
        a[i] = add(a[i], MOD - b[i]);
    }
    return a;
}

std::vector<int> operator *(std::vector<int> a, std::vector<int> b) {
    while(!a.empty() && a.back() == 0) a.pop_back();
    while(!b.empty() && b.back() == 0) b.pop_back();
    if(a.empty() || b.empty()) return std::vector<int>(0, 0);
    int n = 1;
    while(n-1 < (int) a.size() + (int) b.size() - 2) n += n;
    a.resize(n, 0);
    b.resize(n, 0);
    a = fft(a, false);
    b = fft(b, false);
    for(int i = 0; i < n; i++) {
        a[i] = (int) ((long long) a[i] * b[i] % MOD); 
    }
    return fft(a, true);
}

std::vector<int> inverse(const std::vector<int> &a, int k) {
    assert(!a.empty() && a[0] != 0);
    if(k == 0) {
        return std::vector<int>(1, (int) fexp(a[0], MOD - 2));
    } else {
        int n = 1 << k;
        auto c = inverse(a, k-1);
        return cut(c * cut(std::vector<int>(1, 2) - cut(a, n) * c, n), n);
    }
}

std::vector<int> operator /(std::vector<int> a, std::vector<int> b) {
    // NEED TO TEST!
    while(!a.empty() && a.back() == 0) a.pop_back();
    while(!b.empty() && b.back() == 0) b.pop_back();
    assert(!b.empty());
    if(a.size() < b.size()) return std::vector<int>(1, 0);
    std::reverse(a.begin(), a.end());
    std::reverse(b.begin(), b.end());
    int n = (int) a.size() - (int) b.size() + 1;
    int k = 0;
    while((1 << k) - 1 < n) k++;
    a = cut(a * inverse(b, k), (int) a.size() - (int) b.size() + 1);
    std::reverse(a.begin(), a.end());
    return a;
}

std::vector<int> log(const std::vector<int> &a, int k) {
    assert(!a.empty() && a[0] != 0);
    int n = 1 << k;
    std::vector<int> b(n, 0);
    for(int i = 0; i+1 < (int) a.size() && i < n; i++) {
        b[i] = (int)((i + 1LL) * a[i+1] % MOD);
    }
    b = cut(b * inverse(a, k), n);
    assert((int) b.size() == n);
    for(int i = n - 1; i > 0; i--) {
        b[i] = (int) (b[i-1] * fexp(i, MOD - 2) % MOD);
    }
    b[0] = 0;
    return b;
}

std::vector<int> exp(const std::vector<int> &a, int k) {
    assert(!a.empty() && a[0] == 0);
    if(k == 0) {
        return std::vector<int>(1, 1);
    } else {
        auto b = exp(a, k-1);
        int n = 1 << k;
        return cut(b * cut(std::vector<int>(1, 1) + cut(a, n) - log(b, k), n), n);
    }
}

int bits[ms];
template<class mint, class T, const T MOD, const T gen, const T order = MOD - 1>
struct FFT {
    mint root[ms];
 
    FFT() {
        mint mgen(gen);
        assert(mgen.pow(order) == 1 && mgen.pow(order / 2) != 1);
        root[1] = 1;
        for(int len = 2; len < ms; len += len) {
            mint z = mgen.pow(order / len / 2);
            for(int i = len / 2; i < len; i++) {
                root[2 * i] = root[i];
                root[2 * i + 1] = root[i] * z;
            }
        }
    }
 
    void pre(int n) {
        int LOG = 0;
        while(1 << (LOG + 1) < n) {
            LOG++;
        }
        for(int i = 1; i < n; i++) {
            bits[i] = (bits[i >> 1] >> 1) | ((i & 1) << LOG);
        }
    }
 
    std::vector<mint> fft(std::vector<mint> a, bool inv = false) {
        int n = (int) a.size();
        pre(n);
        if(inv) {
            std::reverse(a.begin() + 1, a.end());
        }
        for(int i = 0; i < n; i++) {
            int to = bits[i];
            if(i < to) { std::swap(a[i], a[to]); }
        }
        for(int len = 1; len < n; len *= 2) {
            for(int i = 0; i < n; i += len * 2) {
                for(int j = 0; j < len; j++) {
                    mint u = a[i + j], v = a[i + j + len] * root[len + j];
                    a[i + j] = u + v;
                    a[i + j + len] = u - v;
                }
            }
        }
        if(inv) {
            mint val = mint(n).inverse();
            for(int i = 0; i < n; i++) {
                a[i] = a[i] * val;
            }
        }
        return a;
    }
 
    std::vector<mint> mult(const std::vector<mint> &a, const std::vector<mint> &b) {
        auto A = fft(a), B = a == b ? A : fft(b);
        for(int i = 0; i < (int) a.size(); i++) {
            A[i] = A[i] * B[i];
        }
        return fft(A, true);
    }
 
    std::vector<mint> mult(const std::vector<int> &a, const std::vector<int> &b) {
        std::vector<mint> A(a.size()), B(b.size());
        for(int i = 0; i < (int) a.size(); i++) {
            A[i] = a[i];
            B[i] = b[i] == a[i] ? A[i] : mint(b[i]);
        }
        return mult(A, B);
    }
};

FFT<mint, unsigned int, MOD, 3> fft;

std::vector<mint> convolution(const std::vector<mint> &a, const std::vector<mint> &b) {
    int N = (int) a.size(), M = (int) b.size();
    if(std::min(N, M) <= 32) {
        std::vector<mint> ans(N + M - 1, 0);
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < M; ++j) {
                ans[i + j] += a[i] * b[j];
            }
        }
        return ans;
    }
    int sz = 1;
    while(sz < N + M - 1) sz <<= 1;
    std::vector<mint> A(sz, 0), B(sz, 0);
    for(int i = 0; i < N; i++) {
        A[i] = a[i];
    }
    for(int i = 0; i < M; i++) {
        B[i] = b[i];
    }
    auto ans = fft.mult(A, B);
    ans.resize(N + M - 1);
    return ans;
}

#endif