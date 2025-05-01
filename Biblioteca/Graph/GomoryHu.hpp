#ifndef GOMORY_HU_H
#define GOMORY_HU_H
#include "Biblioteca/template_basics.hpp"

struct Edge {
    Edge() {}
    Edge(int _u, int _v, int _c) : u(_u), v(_v), c(_c) {}
    int u, v;
    int c;
};

template<class T>
std::vector<Edge> GomoryHu(Dinic<T> graph) {
    std::vector<Edge> tree;
    int n = (int) graph.size();
    std::vector<int> par(n, 0);
    for(int i = 1; i < n; i++) {
        auto curGraph = graph;
        tree.emplace_back(i, par[i], curGraph.maxFlow(i, par[i]));
        for(int j = i+1; j < n; j++) {
            if(par[j] == par[i] && curGraph.inCut(j)) {
                par[j] = i;
            }
        }
    }
    return tree;
}

#endif