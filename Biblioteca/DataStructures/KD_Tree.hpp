#ifndef KD_TREE_H
#define KD_TREE_H
#include "Biblioteca/template_basics.hpp"

template<class T, const int D = 2>
class KD_Tree {
public:
    using PT = std::array<T, D>;

    struct Node {
        PT point;
        Node *left, *right;
    };

    void init(std::vector<PT> pts) {
        if(pts.size() == 0) {
            return;
        }
        int n = 0;
        tree.resize(2 * pts.size());
        build<0>(pts.begin(), pts.end(), n);
        //assert(n <= (int) tree.size());
    }

    std::pair<T, PT> nearestNeighbor(PT point) {
        // assert(tree.size() > 0);
        std::pair<T, PT> ans;
        ans.first = 8.1e18; // BE CAREFUL HERE
        removedEqual = false;
        nearestNeighbor<0>(&tree[0], point, ans);
        return ans;
    }
private:
    std::vector<Node> tree;

    template<const int d>
    Node* build(auto l, auto r, int &n) {
        if(l >= r) {
            return NULL;
        }
        int id = n++;
        if(r - l == 1) {
            tree[id].left = tree[id].right = NULL;
            tree[id].point = *l;
        } else {
            auto mid = l + ((r - l) / 2);
            std::nth_element(l, mid - 1, r, [](const PT &u, const PT &v) { return u[d] < v[d]; });
            tree[id].point = *(mid - 1);
            // BE CAREFUL!
            // DO EVERYTHING BEFORE BUILDING THE LOWER PART!
            tree[id].left = build<(d+1)%D>(l, mid-1, n);
            tree[id].right = build<(d+1)%D>(mid, r, n);
        }
        return &tree[id];
    }
    bool removedEqual;
    template<const int d>
    void nearestNeighbor(Node* node, PT point, std::pair<T, PT> &ans) {
        if(!node) {
            return;
        }
        const T dist = sqrtDist(point, node->point);
        if((point != node->point || removedEqual) && dist < ans.first) {
            // THIS WAS FOR A PROBLEM
            // THAT YOU DON'T CONSIDER THE DISTANCE TO ITSELF!
            ans = {dist, node->point};
        } else if(point == node->point) {
            removedEqual = true;
        }
        T delta = point[d] - node->point[d];
        if(delta <= 0) {
            nearestNeighbor<(d+1)%D>(node->left, point, ans);
            if(ans.first > delta * delta) {
                nearestNeighbor<(d+1)%D>(node->right, point, ans);
            }
        } else {
            nearestNeighbor<(d+1)%D>(node->right, point, ans);
            if(ans.first > delta * delta) {
                nearestNeighbor<(d+1)%D>(node->left, point, ans);
            }
        }
    }

    T sqrtDist(const PT &u, const PT &v) {
        T ans = 0;
        for(int i = 0; i < D; i++) {
            T delta = u[i] - v[i];
            ans += delta * delta;
        }
        return ans;
    }
};

#endif
