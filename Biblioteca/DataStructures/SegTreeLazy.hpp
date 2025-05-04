#ifndef LAZY_SEGTREE_H
#define LAZY_SEGTREE_H
#include "Biblioteca/template_basics.hpp"

// Range update, range query segment tree

template <class i_t, class e_t, class lazy_cont>
class SegmentTree {
public:
    void init(std::vector<e_t> base) {
        n = (int) base.size();
        h = 0;
        while((1 << h) < n) h++;
        tree.resize(2 * n);
        dirty.assign(n, false);
        lazy.resize(n);
        for(int i = 0; i < n; i++) {
            tree[i + n] = i_t(base[i]);
        }
        for(int i = n - 1; i > 0; i--) {
            tree[i] = tree[i + i] + tree[i + i + 1];
            lazy[i] = lazy_cont();
        }
    }

    i_t qry(int l, int r) {
        if(l >= r) return i_t();
        l += n, r += n;
        push(l), push(r - 1);
        i_t lp, rp;
        for(; l < r; l /= 2, r /= 2) {
            if(l & 1) lp = lp + tree[l++];
            if(r & 1) rp = tree[--r] + rp;
        }
        return lp + rp;
    }

    void upd(int l, int r, lazy_cont lc) {
        if(l >= r) return;
        l += n, r += n;
        push(l), push(r - 1);
        int l0 = l, r0 = r;
        int s0 = 0, s1 = r - l;
        for(int sz = 1; l < r; l /= 2, r /= 2, sz *= 2) {
            if(l & 1) apply(l++, lc, sz, s0), s0 += sz;
            if(r & 1) s1 -= sz, apply(--r, lc, sz, s1);
        }
        build(l0), build(r0 - 1);
    }

    // don't copy this for most segment tree problems!
    // search first x in [l, r) such that [l, x) triggers f([l, x))
    template<class F>
    std::pair<int, i_t> search(int l, int r, F f) {
        // assert(l < r);
        if(l >= r) return std::pair<int, i_t>(r, i_t()); // careful with this case
        int lst = r;
        l += n, r += n;
        push(l), push(r-1);
        static int pref[30], suf[30];
        int s[2] = {0, 0};
        for(; l < r; l /= 2, r /= 2) {
            if(l & 1) pref[s[0]++] = l++;
            if(r & 1) suf[s[1]++] = --r;
        }
        i_t cur;
        for(int rep = 0; rep < 2; rep++) {
            if(rep == 1) { std::reverse(suf, suf + s[1]); }
            for(int id = 0; id < s[rep]; id++) {
                int i = rep == 0 ? pref[id] : suf[id];
                if(f(cur + tree[i])) {
                    while(i < n) {
                        pushNode(i);
                        i += i;
                        i_t other = cur + tree[i];
                        if(!f(other)) {
                            cur = other;
                            i++;
                        }
                    }
                    return std::pair<int, i_t>(i - n, cur);
                }
                cur = cur + tree[i];
            }
        }
        return std::pair<int, i_t>(lst, cur);
    }
private:
    int n, h;
    std::vector<bool> dirty;
    std::vector<i_t> tree;
    std::vector<lazy_cont> lazy;

    void apply(int p, lazy_cont &lc, int size, int shift) {
        tree[p].apply(lc, size, shift);
        if(p < n) {
            dirty[p] = true;
            lazy[p] += lc;
        }
    }

    void push(int p) {
        for(int s = h; s > 0; s--) {
            int i = p >> s;
            pushNode(i, 1 << (s - 1));
        }
    }

    inline void pushNode(int i, int size) {
        if(dirty[i]) {
            apply(i + i, lazy[i], size, 0);
            apply(i + i + 1, lazy[i], size, size);
            lazy[i] = lazy_cont();
            dirty[i] = false;
        }
    }

    void build(int p) {
        int size = 2;
        for(p /= 2; p > 0; p /= 2, size *= 2) {
            tree[p] = tree[p + p] + tree[p + p + 1];
            if(dirty[p]) {
                tree[p].apply(lazy[p], size, 0);
            }
        }
    }
};

#endif
