#ifndef BINARY_LIFTING_H
#define BINARY_LIFTING_H
#include "Biblioteca/template_basics.hpp"

// maybe do something for general edges?

namespace BinaryLifting {
    const int ms = 100100;
    const int me = 18;
    int anc[me][ms];
    int h[ms];
    
    void dfs(const std::vector<std::vector<int>> &edges, int on, int par, int he = 0) {
        h[on] = he;
        anc[0][on] = par;
        for(auto to : edges[on]) {
            if(to != par) {
                dfs(edges, to, on, he + 1);
            }
        }
    }
    
    void init(const std::vector<std::vector<int>> &edges, int on = 0) {
        dfs(edges, on, on);
        for(int i = 1; i < me; i++) {
            for(int j = 0; j < edges.size(); j++) {
                anc[i][j] = anc[i-1][anc[i-1][j]];
            }
        }
    }
    
    int getLCA(int u, int v) {
        if(h[u] < h[v]) {
            std::swap(u, v);
        }
        int ans = h[u] - h[v];
        for(int i = me - 1; i >= 0; i--) {
            if((1 << i) & ans) {
                u = anc[i][u];
            }
        }
        if(u == v) {
            return u;
        }
        for(int i = me - 1; i >= 0; i--) {
            if(anc[i][u] != anc[i][v]) {
                ans += 1 << (i + 1);
                u = anc[i][u];
                v = anc[i][v];
            }
        }
        return anc[0][u];
    }
    
    int getDist(int u, int v) {
        int lca = getLCA(u, v);
        return h[u] + h[v] - 2 * h[lca];
    }
}

#endif