#ifndef COLOR_UPDATE_H
#define COLOR_UPDATE_H
#include "Biblioteca/template_basics.hpp"

template <class InfoType = int32_t, class RangeType = int32_t>
struct ColorUpdate {
public:
    struct Range {
        Range(RangeType _l = 0) : l(_l) {}
        Range(RangeType _l, RangeType _r, InfoType _v) : l(_l), r(_r), v(_v) { }
        RangeType l, r;
        InfoType v;

        bool operator < (const Range &b) const { return l < b.l; }
    };

    std::vector<Range> erase(RangeType l, RangeType r) {
        std::vector<Range> ans;
        if(l >= r) return ans;
        auto it = ranges.lower_bound(l);
        if(it != ranges.begin()) {
            it--;
            if(it->r > l) {
                auto cur = *it;
                ranges.erase(it);
                ranges.insert(Range(cur.l, l, cur.v));
                ranges.insert(Range(l, cur.r, cur.v));
            }
        }
        it = ranges.lower_bound(r);
        if(it != ranges.begin()) {
            it--;
            if(it->r > r) {
                auto cur = *it;
                ranges.erase(it);
                ranges.insert(Range(cur.l, r, cur.v));
                ranges.insert(Range(r, cur.r, cur.v));
            }
        }
        for(it = ranges.lower_bound(l); it != ranges.end() && it->l < r; it++) {
            ans.push_back(*it);
        }
        ranges.erase(ranges.lower_bound(l), ranges.lower_bound(r));
        return ans;
    }

    std::vector<Range> upd(RangeType l, RangeType r, InfoType v) {
        auto ans = erase(l, r);
        ranges.insert(Range(l, r, v));
        return ans;
    }

    bool exists(RangeType x) {
        auto it = ranges.upper_bound(x);
        if(it == ranges.begin()) return false;
        it--;
        return it->l <= x && x < it->r;
    }
private:
    std::set<Range> ranges;
};

struct CrazySet {
public:
    void addLazy(long long x) {
        lazy += x;
    }

    void invert() {
        lazy = -lazy;
        inverted = !inverted;
    }

    void addRange(long long l, long long r) {
        if(!inverted) {
            ranges.upd(l-lazy, r-lazy, true);
        } else {
            ranges.upd(-r+1+lazy, -l+1+lazy, true);
        }
    }

    void removeRange(long long l, long long r) {
        if(!inverted) {
            ranges.erase(l-lazy, r-lazy);
        } else {
            ranges.erase(-r+1+lazy, -l+1+lazy);
        }
    }

    bool exists(long long x) {
        if(!inverted) {
            return ranges.exists(x - lazy);
        } else {
            return ranges.exists(-x + lazy);
        }
    }

    bool empty() { return ranges.ranges.empty(); }
private:
    ColorUpdate<bool, long long> ranges;
    bool inverted = false;
    long long lazy = 0;
};

#endif
