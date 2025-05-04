#ifndef TREEDP_H
#define TREEDP_H
#include "Biblioteca/template_basics.hpp"

struct TreeDP {
    TreeDP() {}
    TreeDP(int _n) {
        n = _n;
        cnt = 0;
        edges.resize(n);
        up.assign(n, -1);
    }

    void addEdge(int u, int v) {
        edges[u].emplace_back(v, cnt++);
        edges[v].emplace_back(u, cnt++);
    }

    template<class DP, class V, class E>
    std::vector<DP> solve(V v, E e, bool reroot = true) {
        std::vector<DP> dp(n);
        go(0, 0, dp, v, e);
        if(reroot) back(0, 0, dp, v, e, DP());
        return dp;
    }

    template<class DP, class V, class E>
    void go(int on, int par, std::vector<DP> &dp, V &v, E &e) {
        dp[on] = v(on);
        for(int i = 0; i < (int) edges[on].size(); i++) {
            int to = edges[on][i].first;
            if(to == par) {
                up[on] = edges[on][i].second;
                std::swap(edges[on][i], edges[on].back());
                edges[on].pop_back();
                i--;
                continue;
            }
            go(to, on, dp, v, e);
            dp[on] = DP(dp[on], e(dp[to], edges[on][i].second));
        }
    }

    template<class DP, class V, class E>
    void back(int on, int par, std::vector<DP> &dp, V &v, E &e, DP above) {
        if(par != on) {
            above = e(above, up[on]);
            above = DP(v(on), above);
        } else {
            above = v(on);
        }
        if(edges[on].empty()) {
            dp[on] = above;
            return;
        }
        std::vector<DP> pref((int) edges[on].size());
        pref[0] = DP();
        for(int i = 0; i+1 < (int) edges[on].size(); i++) {
            int to = edges[on][i].first;
            pref[i+1] = DP(pref[i], e(dp[to], edges[on][i].second));
        }
        for(int i = (int) edges[on].size()-1; i >= 0; i--) {
            int to = edges[on][i].first;
            DP nxt = DP(above, e(dp[to], edges[on][i].second));
            back(to, on, dp, v, e, DP(above, pref[i]));
            above = nxt;
        }
        dp[on] = above;
    }

    int n, cnt;
    std::vector<std::vector<std::pair<int, int>>> edges;
    std::vector<int> up;
};

// sample
struct DP {
    DP(DP l, DP r) {
        ans = l.ans + std::max(r.ans, 0);
    }
    DP(int v = 0) : ans(v) {}
    int ans;
};

// V receives the vertex index and returns dp
auto V = [&](int v) { return DP(a[v]); };
// E receives DP and 2*e if using in (u, v) or 2*e+1 if using in (v, u)
auto E = [](DP dp, int e) { return DP(std::max(dp.ans, 0)); };

#endif
