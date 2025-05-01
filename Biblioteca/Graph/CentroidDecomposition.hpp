#ifndef CENTROID_DECOMP_H
#define CENTROID_DECOMP_H
#include "Biblioteca/template_basics.hpp"

struct Info {
    // needs base constructor and (Info, EdgeCostType) constructor
};
struct Solver {
    void operator()(const std::vector<std::vector<std::pair<int, Info>>> &a) {}
};

template<class Cost>
class CentroidDecomposition {
public:
    struct Edge {
        Edge(){}
        Edge(int v, Cost cost) : to(v), c(cost) {}
        int to;
        Cost c;
    };
    CentroidDecomposition(int _n) : n(_n), edges(_n), tree(_n), col(_n, -1), size(_n), pt(_n, 0) {}

    void addEdge(int u, int v, Cost c) {
        edges[u].emplace_back(v, c);
        edges[v].emplace_back(u, c);
        pt[u]++, pt[v]++;
    }

    int init() {
        int ans = init(0, 0);
        for(int i = 0; i < n; i++) {
            std::sort(edges[i].begin(), edges[i].end(), [&](Edge e1, Edge e2) { return col[e1.to] > col[e2.to]; });
        }
        return ans;
    }

    // to get only one centroid
    int getCentroid() {
        preDFS(0, -1);
        return getCen(0);
    }

    template<class Info>
    std::vector<std::vector<std::pair<int, Info>>> getInfo(int i, Info base) {
        std::vector<std::pair<int, Info>> a;
        std::vector<std::vector<std::pair<int, Info>>> ans(1, std::vector<std::pair<int, Info>>(1, std::pair<int, Info>(i, base)));
        for(int j = 0; j < pt[i]; j++) {
            a.clear();
            dfs(a, edges[i][j].to, i, col[i], Info(base, edges[i][j].c));
            ans.push_back(a);
        }
        return ans;
    }

    template<class Info>
    void dfs(std::vector<std::pair<int, Info>> &vec, int on, int par, int lim, Info h) {
        vec.emplace_back(on, h);
        for(auto e : edges[on]) {
            if(e.to == par) continue;
            if(col[e.to] <= lim) break;
            dfs(vec, e.to, on, lim, Info(h, e.c));
        }
    }

private:
    void preDFS(int on, int par) {
        size[on] = 1;
        for(int i = 0; i < pt[on]; i++) {
            int to = edges[on][i].to;
            if(col[to] != -1 || to == par) {
                std::swap(edges[on][i], edges[on][pt[on]-1]);
                if(col[to] != -1) {
                    pt[on]--;
                    i--;
                } else if(i+1 < pt[on]) {
                    i--;
                }
                continue;
            }
            preDFS(to, on);
            size[on] += size[to];
            if(size[to] > size[edges[on][0].to]) {
                std::swap(edges[on][0], edges[on][i]);
            }
        }
    }

    int getCen(int on) {
        int sz = size[on];
        while(size[on] > 1 && 2 * size[edges[on][0].to] >= sz) on = edges[on][0].to;
        return on;
    }

    int init(int on, int c) {
        preDFS(on, -1);
        on = getCen(on);
        col[on] = c;
        for(auto e : edges[on]) {
            if(col[e.to] == -1) {
                tree[on].push_back(init(e.to, c+1));
            }
        }
        return on;
    }

    int n;
    std::vector<std::vector<Edge>> edges;
    std::vector<std::vector<int>> tree; // centroid tree edges
    std::vector<int> col, size, pt;
};

#endif