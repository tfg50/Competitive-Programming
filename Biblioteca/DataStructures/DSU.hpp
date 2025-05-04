#ifndef DSU_H
#define DSU_H
#include "Biblioteca/template_basics.hpp"

template<const bool useCompression = true>
struct DSU {
    int getSize(int x) { return -par[getPar(x)]; }
    int getPar(int x) { return par[x] < 0 ? x : (useCompression ? par[x] = getPar(par[x]) : getPar(par[x])); }
    bool makeUnion(int a, int b) {
        a = getPar(a), b = getPar(b);
        if(a == b) return false;
        if(par[a] > par[b]) std::swap(a, b);
        par[a] += par[b];
        par[b] = a;
        return true;
    }
    void init(int n) {
        par.assign(n, -1);
    }
    std::basic_string<int> par;
};

#endif
