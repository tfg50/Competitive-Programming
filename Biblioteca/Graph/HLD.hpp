#ifndef HLD_H
#define HLD_H
#include "Biblioteca/template_basics.hpp"

struct HLD {
public:
    void init(int n) {
        // this doesn't delete edges!
        sz.resize(n);
        in.resize(n);
        out.resize(n);
        rin.resize(n);
        p.resize(n);
        edges.resize(n);
        nxt.resize(n);
        h.resize(n);
    }

    void addEdge(int u, int v) {
        edges[u].push_back(v);
        edges[v].push_back(u);
    }

    void setRoot(int n) {
        t = 0;
        p[n] = n;
        h[n] = 0;
        prep(n, n);
        nxt[n] = n;
        hld(n);
    }

    int getLCA(int u, int v) {
        while(!inSubtree(nxt[u], v)) {
            u = p[nxt[u]];
        }
        while(!inSubtree(nxt[v], u)) {
            v = p[nxt[v]];
        }
        return in[u] < in[v] ? u : v;
    }

    bool inSubtree(int u, int v) {
        // is v in the subtree of u?
        return in[u] <= in[v] && in[v] < out[u];
    }

    std::vector<std::pair<int, int>> getPathtoAncestor(int u, int anc) {
        // returns ranges [l, r) that the path has
        std::vector<std::pair<int, int>> ans;
        //assert(inSubtree(anc, u));
        while(nxt[u] != nxt[anc]) {
            ans.emplace_back(in[nxt[u]], in[u] + 1);
            u = p[nxt[u]];
        }
        // this includes the ancestor!
        ans.emplace_back(in[anc], in[u] + 1);
        return ans;
    }
    std::vector<int> in, out, p, rin, sz, nxt, h;
    std::vector<std::vector<int>> edges;
    int t;
private:
    void prep(int on, int par) {
        sz[on] = 1;
        p[on] = par;
        for(int i = 0; i < (int) edges[on].size(); i++) {
            int &u = edges[on][i];
            if(u == par) {
                std::swap(u, edges[on].back());
                edges[on].pop_back();
                i--;
            } else {
                h[u] = 1 + h[on];
                prep(u, on);
                sz[on] += sz[u];
                if(sz[u] > sz[edges[on][0]]) {
                    std::swap(edges[on][0], u);
                }
            }
        }
    }

    void hld(int on) {
        in[on] = t++;
        rin[in[on]] = on;
        for(auto u : edges[on]) {
            nxt[u] = (u == edges[on][0] ? nxt[on] : u);
            hld(u);
        }
        out[on] = t;
    }
};

#endif