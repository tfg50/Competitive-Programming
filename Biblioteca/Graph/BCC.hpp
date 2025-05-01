#ifndef BCC_H
#define BCC_H
#include "Biblioteca/template_basics.hpp"

struct BCC {
    BCC(int _n) : n(_n) {
        edges.resize(n);
        low.assign(n, -1);
        cur.assign(n, -1);
        down.assign(n, -1);
        group.assign(n, -1);
        col.assign(n, 0);
        inv.assign(n, -1);
    }

    void addEdge(int u, int v) {
        edges[u].push_back(v);
        edges[v].push_back(u);
    }

    void init() {
        for(int i = 0, c = 0; i < n; i++) {
            if(low[i] == -1) {
                dfs(i, c, -1);
            }
        }
    }

    // returns -1 if (u, v) edge is bridge or some number if edge is in a cycle
    int getLow(int u) {
        if(low[u] == cur[u] || low[group[u]] == low[u]) return low[u];
        else {
            getLow(inv[low[u]]);
            group[u] = group[inv[low[u]]];
            low[u] = low[inv[low[u]]];
            return low[u];
        }
    }
    int getGroup(int u, int v) {
        if(cur[u] < cur[v]) std::swap(u, v);
        getLow(u), getLow(v);
        return group[u] == group[v] || getLow(u) == cur[v] ? group[u] : -1;
    }
    int n;
    std::vector<std::vector<int>> edges;
    std::vector<int> low, cur, down, group, col, inv;

    void dfs(int on, int &c, int par) {
        cur[on] = c;
        inv[c] = on;
        low[on] = c++;
        int children = 0;
        bool root = par == -1;
        bool isArt = false;
        for(auto to : edges[on]) {
            if(to == par) {
                par = -1;
            } else if(low[to] == -1) {
                children++;
                down[cur[on]] = to;
                col[to] = 1 ^ col[on];
                dfs(to, c, on);
                low[on] = std::min(low[on], low[to]);
                if((root && children > 1) || (!root && low[to] >= cur[on])) {
                    isArt = true;
                }
            } else {
                low[on] = std::min(low[on], cur[to]);
            }
        }
        group[on] = down[low[on]];
        down[cur[on]] = isArt ? -1 : 0;
    }
};

#endif