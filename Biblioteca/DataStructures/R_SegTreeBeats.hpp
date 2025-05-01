#ifndef R_SEGTREEBEATS_H
#define R_SEGTREEBEATS_H
#include "Biblioteca/template_basics.hpp"

// Recursive segment tree beats
// make canBreak() == false and canApply() == true

template <class i_t, class e_t, class lazy_cont>
class SegmentTree {
public:
    void init(std::vector<e_t> base) {
        n = base.size();
        tree.resize(4 * n);
        dirty.assign(2 * n, false);
        lazy.resize(2 * n);
        build(1, 0, n - 1, base);
    }
    
    i_t qry(int l, int r) { return qry(1, 0, n - 1, l, r - 1); }
    void upd(int l, int r, lazy_cont lc) { upd(1, 0, n - 1, l, r - 1, lc); }
    void upd(int pos, e_t v) { upd(1, 0, n - 1, pos, v); }
private:
    int n;
    std::vector<bool> dirty;
    std::vector<i_t> tree;
    std::vector<lazy_cont> lazy;

    void build(int p, int l, int r, std::vector<e_t> &base) {
        if(l == r) {
            tree[p] = i_t(base[l]);
        } else {
            int mid = (l + r) / 2;
            build(p + p, l, mid, base);
            build(p + p + 1, mid + 1, r, base);
            tree[p] = i_t(tree[p + p], tree[p + p + 1]);
            lazy[p] = lazy_cont();
        }
    }

    void apply(int p, int l, int r, lazy_cont &lc) {
        tree[p].apply(lc);
        if(l != r) {
            dirty[p] = true;
            lazy[p] += lc;
        }
    }

    void push(int p, int l, int r) {
        if(dirty[p]) {
            int mid = (l + r) / 2;
            upd(p + p, l, mid, l, mid, lazy[p]);
            upd(p + p + 1, mid + 1, r, mid + 1, r, lazy[p]);
            lazy[p] = lazy_cont();
            dirty[p] = false;
        }
    }

    void upd(int p, int l, int r, int pos, e_t &v) {
        if(l == r) {
            tree[p] = i_t(v);
        } else {
            push(p, l, r);
            int mid = (l + r) / 2;
            if(pos <= mid) {
                upd(p + p, l, mid, pos, v);
            } else {
                upd(p + p + 1, mid + 1, r, pos, v);
            }
            tree[p] = i_t(tree[p + p], tree[p + p + 1]);
        }
    }

    void upd(int p, int l, int r, int i, int j, lazy_cont lc) {
        if(i > r || j < l || tree[p].canBreak(lc)) {
            return;
        } else if(i <= l && r <= j && tree[p].canApply(lc)) {
            apply(p, l, r, lc);
        } else {
            push(p, l, r);
            int mid = (l + r) / 2;
            upd(p + p, l, mid, i, j, lc);
            upd(p + p + 1, mid + 1, r, i, j, lc);
            tree[p] = i_t(tree[p + p], tree[p + p + 1]);
        }
    }

    i_t qry(int p, int l, int r, int i, int j) {
        if(i > r || j < l) {
            return i_t();
        } else if(i <= l && r <= j) {
            return tree[p];
        } else {
            push(p, l, r);
            int mid = (l + r) / 2;
            return i_t(qry(p + p, l, mid, i, j), qry(p + p + 1, mid + 1, r, i, j));
        }
    }
};

#endif