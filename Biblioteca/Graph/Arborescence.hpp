#ifndef ARBORESCENCE_H
#define ARBORESCENCE_H
#include "Biblioteca/template_basics.hpp"

long long arborescence(vector<vector<pair<ll, int>>> edges) {
    // root is -1
    int n = (int) edges.size();
    vector<int> prv(n, -1);
    vector<long long> cost(n, 1e18);
    long long ans = 0;
    for(int i = 0; i < n; i++) {
        if(edges[i].empty()) continue;
        long long mn = 1e18;
        for(auto &e : edges[i]) {
            mn = min(mn, e.first);
        }
        ans += mn;
        for(auto &e : edges[i]) {
            e.first -= mn;
            if(e.first == 0) {
                prv[i] = e.second;
            }
        }
    }
    for(int i = 0; i < n; i++) {
        if(edges[i].empty()) continue;
        vector<bool> visit(n, false);
        int x = i;
        while(x != -1 && !visit[x]) {
            visit[x] = true;
            x = prv[x];
        }
        if(x != i) continue;
        // found cycle
        vector<pair<long long, int>> ed;
        for(int j = 0; j < n; j++) {
            if(!visit[j]) {
                for(auto &e : edges[j]) {
                    if(e.second != -1 && visit[e.second]) {
                        e.second = x;
                    }
                }
                continue;
            }
            for(auto e : edges[j]) {
                if(e.second == -1 || !visit[e.second]) {
                    ed.push_back(e);
                }
            }
            edges[j].clear();
        }
        edges[x] = ed;
        return ans + arborescence(edges);
    }
    return ans;
}

#endif
