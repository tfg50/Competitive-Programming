#ifndef DYNACON_H
#define DYNACON_H
#include "Biblioteca/template_basics.hpp"

struct RollbackDSU {
    int getSize(int x) { return -par[getPar(x)]; }
    int getPar(int x) {
        while(par[x] >= 0) {
            x = par[x];
        }
        return x;
    }

    void makeUnion(int a, int b) {
        a = getPar(a), b = getPar(b);
        if(a == b) return;
        if(par[a] > par[b]) std::swap(a, b);
        op.emplace_back(a, par[a]);
        op.emplace_back(b, par[b]);
        par[a] += par[b];
        par[b] = a;
    }

    void init(int n) {
        par.assign(n, -1);
        op.clear();
    }

    void rollBack() {
        par[op.back().first] = op.back().second;
        op.pop_back();
    }

    std::vector<int> par;
    std::vector<std::pair<int, int>> op;
};

namespace DynamicConnectivity {
    struct Edge {
        int l, r, u, v;
    };

    template<class QueryFunction>
    void solve(int l, int r, std::vector<Edge> edges, const QueryFunction &f, RollbackDSU &dsu) {
        int st = (int) dsu.op.size();
        std::vector<Edge> down;
        for(auto e : edges) {
            if(e.r <= l || e.l >= r) {

            } else if(e.l <= l && r <= e.r) {
                dsu.makeUnion(e.u, e.v);
            } else {
                down.push_back(e);
            }
        }
        edges.clear();
        if(l + 1 == r) {
            f(l);
        } else {
            int mid = (l + r) / 2;
            solve(l, mid, down, f, dsu);
            solve(mid, r, down, f, dsu);
        }
        while((int) dsu.op.size() > st) {
            dsu.rollBack();
        }
    }
}

#endif
