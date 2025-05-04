#ifndef CHT_H
#define CHT_H
#include "Biblioteca/Math/2DGeometry/PT.hpp"

struct CHTrick {
    // queries are (x, 1) * (pt.x, pt.y)
    // this code is for max
    CHTrick(){ n = 0; }
    CHTrick(std::vector<PT> pts) {
        std::sort(pts.rbegin(), pts.rend());
        n = 0;
        for(auto pt : pts) {
            while(n >= 2 && (hull[n-1] - hull[n-2]) % (pt - hull[n-2]) <= 0) {
                hull.pop_back();
                n--;
            }
            hull.push_back(pt);
            n++;
        }
    }

    long long qry(long long x) {
        if(hull.empty()) return (long long) (-1e18);
        int l = 0, r = n-1;
        PT cur(x, 1);
        while(l != r) {
            int mid = (l + r) / 2;
            if(hull[mid] * cur <= hull[mid+1] * cur) {
                l = mid + 1;
            } else {
                r = mid;
            }
        }
        return hull[l] * cur;
    }

    int n;
    std::vector<PT> hull;
};

struct Line {
    mutable ll k, m, p;
    bool operator<(const Line& o) const { return k < o.k; }
    bool operator<(ll x) const { return p < x; }
};

struct LineContainer : multiset<Line, less<>> {
    // (for doubles, use inf = 1/.0, div(a,b) = a/b)
    static const ll inf = LLONG_MAX;
    ll div(ll a, ll b) { // floored division
        return a / b - ((a ^ b) < 0 && a % b); }
    bool isect(iterator x, iterator y) {
        if (y == end()) return x->p = inf, 0;
        if (x->k == y->k) x->p = x->m > y->m ? inf : -inf;
        else x->p = div(y->m - x->m, x->k - y->k);
        return x->p >= y->p;
    }
    void add(ll k, ll m) {
        auto z = insert({k, m, 0}), y = z++, x = y;
        while (isect(y, z)) z = erase(z);
        if (x != begin() && isect(--x, y)) isect(x, y = erase(y));
        while ((y = x) != begin() && (--x)->p >= y->p)
            isect(x, erase(y));
    }
    ll query(ll x) {
        assert(!empty());
        auto l = *lower_bound(x);
        return l.k * x + l.m;
    }
};

#endif
