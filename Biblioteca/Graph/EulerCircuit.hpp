#ifndef EULER_CIRCUIT_H
#define EULER_CIRCUIT_H
#include "Biblioteca/template_basics.hpp"

template <const bool BIDIRECTIONAL>
class EulerCircuit {
public:
    void init(int size) {
        n = size;
        edges.resize(n);
        loops.resize(n, 0);
    }

    void addEdge(int u, int v) {
        if(u == v) {
            loops[u]++;
            return;
        }
        int cur = (int) used.size();
        used.push_back(false);
        edges[u].emplace_back(v, cur);
        if(BIDIRECTIONAL) {
            edges[v].emplace_back(u, cur);
        }
    }

    std::vector<int> solve(int on = 0) {
        std::vector<int> ans;
        dfs(on, ans);
        return ans;
    }
private:
    int n;
    std::vector<int> loops;
    std::vector<std::vector<std::pair<int, int>>> edges;
    std::vector<bool> used;

    void fix(int on) {
        while(!edges[on].empty() && used[edges[on].back().second]) {
            edges[on].pop_back();
        }
    }

    void dfs(int on, std::vector<int> &ans) {
        fix(on);
        int st = on;
        std::vector<int> path;
        if(!edges[on].empty()) do {
            path.push_back(on);
            used[edges[on].back().second] = true;
            on = edges[on].back().first;
            fix(on);
        } while(!edges[on].empty() && on != st);
        assert(on == st);
        for(auto x : path) {
            dfs(x, ans);
        }
        ans.push_back(on);
        while(loops[on] > 0) {
            loops[on]--;
            ans.push_back(on);
        }
    }
};

#endif