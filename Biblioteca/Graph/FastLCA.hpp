#ifndef FACTLCA_H
#define FASTLCA_H
#include "Biblioteca/Graph/SimpleEulerTourTree.hpp"
#include "Biblioteca/DataStructures/FastRMQ.hpp"

class FastLCA {
public:
    void init(const std::vector<std::vector<int>> &edges, int root) {
        ett.init(edges, root);
        rmq.init(ett.a);
    }

    int lca(int u, int v) {
        if(ett.in[u] > ett.in[v]) std::swap(u, v);
        return rmq.qry(ett.in[u], ett.out[v]).second;
    }
private:
    SimpleEulerTourTree<true> ett;
    FastRMQ<std::pair<int, int>> rmq;
};

#endif
