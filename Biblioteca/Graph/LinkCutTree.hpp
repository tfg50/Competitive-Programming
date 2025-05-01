#ifndef LINK_CUT_TREE_H
#define LINK_CUT_TREE_H
#include "Biblioteca/DataStructures/SplayTree.hpp"

struct LinkCutTree {
    LinkCutTree(int N) : nodes(N) {}
    std::vector<Splay::Node> nodes; // can change to map<int, Splay::Node*> if possible
    
    /* USE THIS IF USING MAP
    void createVertex(int u, Info info) {
        assert(!nodes.count(u));
        nodes[u] = new Splay::Node(info);
    }*/

    void cutRight(Splay::Node* root) {
        if(root->right) {
            root->right->p = NULL;
            root->right->pp = root;
            root->right = NULL;
            root->fix();
        }
    }

    Splay::Node* getNode(int u) {
        return &nodes[u];
    }

    Splay::Node* access(int u) {
        for(Splay::Node *y = NULL, *z = getNode(u); z; y = z, z = z->pp) {
            z->splay();
            cutRight(z);
            if(y) y->p = z;
            z->right = y;
            z->fix();
        }
        auto root = getNode(u)->splay();
        assert(!root->flip && root->p == NULL && root->pp == NULL);
        root->flip = true;
        root->push();
        return root;
    }

    bool cut(int u, int v) {
        auto b = access(v);
        auto a = access(u);
        b->push();
        if(a->right != b || b->left) return false;
        a->right = b->p = b->pp = NULL;
        a->fix();
        return true;
    }

    bool isConnected(int u, int v) {
        auto a = access(u);
        auto b = access(v);
        return a == b || a->p;
    }

    bool link(int u, int v) {
        if(isConnected(u, v)) return false;
        assert(getNode(u)->pp == NULL && getNode(u)->p == NULL);
        getNode(u)->pp = getNode(v);
        return true;
    }

    Splay::Node* getPath(int u, int v) {
        if(!isConnected(v, u)) return NULL;
        else return getNode(u);
    }
};

#endif