#ifndef VEB_TREE_H
#define VEB_TREE_H
#include "Biblioteca/template_basics.hpp"

class vEB6 {
public:
    inline bool empty() const { return !mask; }
    inline int getMax() const { return empty() ? -1 : 63 - __builtin_clzll(mask); }
    inline int getMin() const { return empty() ? 64 : __builtin_ctzll(mask); }
    // findNext and findPrevious are broken if you use a small vEB, consider adding ifs in case the previous/next doesn't exist
    inline int findNext(int pos) const { return __builtin_ctzll(mask & ~((1LL << (pos+1)) - 1)); }
    inline int findPrevious(int pos) const { return 63 - __builtin_clzll(mask & ((1LL << pos) - 1)); }
    inline void set(int pos) { mask |= 1LL << pos; }
    inline void erase(int pos) { mask &= ~(1LL << pos); }
private:
    unsigned long long mask = 0;
};

template<const int N> class vEB;
template<> class vEB<6> : public vEB6 {};
template<> class vEB<5> : public vEB6 {};
template<> class vEB<4> : public vEB6 {};
template<> class vEB<3> : public vEB6 {};
template<> class vEB<2> : public vEB6 {};
template<> class vEB<1> : public vEB6 {};

template<const int N>
class vEB {
public:
    inline bool empty() const { return min > max; }
    inline int getMax() const { return max; }
    inline int getMin() const { return min; }
    inline int findNext(int pos) const {
        if(pos < min) {
            return min;
        }
        if(pos >= max) {
            return 1 << N;
        }
        int bucket = pos >> restBits;
        int newPos = pos & ((1 << restBits) - 1);
        if(newPos < children[bucket].getMax()) {
            return (bucket << restBits) + children[bucket].findNext(newPos);
        }
        bucket = auxTree.findNext(bucket);
        return (bucket << restBits) + children[bucket].getMin();
    }

    inline int findPrevious(int pos) const {
        if(pos > max) {
            return max;
        }
        if(pos <= min) {
            return -1;
        }
        int bucket = pos >> restBits;
        int newPos = pos & ((1 << restBits) - 1);
        if(newPos > children[bucket].getMin()) {
            return (bucket << restBits) + children[bucket].findPrevious(newPos);
        }
        if(bucket <= auxTree.getMin()) {
            return min;
        }
        bucket = auxTree.findPrevious(bucket);
        return (bucket << restBits) + children[bucket].getMax();
    }

    inline void set(int pos) {
        assert(0 <= pos && pos < (1 << N));
        if(min > max) {
            min = max = pos;
            return;
        }
        if(pos < min) {
            std::swap(pos, min);
        }
        if(pos > max) {
            max = pos;
        }
        int bucket = pos >> restBits;
        int newPos = pos & ((1 << restBits) - 1);
        children[bucket].set(newPos);
        if(children[bucket].getMin() == children[bucket].getMax()) {
            auxTree.set(bucket);
        }
    }

    inline void erase(int pos) {
        if(min == max && max == pos) {
            min = 1 << N, max = -1;
            return;
        }
        if(pos == min) {
            int bucket = auxTree.getMin();
            min = pos = (bucket << restBits) + children[bucket].getMin();
        }
        int bucket = pos >> restBits;
        int newPos = pos & ((1 << restBits) - 1);
        children[bucket].erase(newPos);
        if(children[bucket].empty()) {
            auxTree.erase(bucket);
        }
        if(pos == max) {
            if(auxTree.empty()) {
                max = min;
            } else {
                bucket = auxTree.getMax();
                max = (bucket << restBits) + children[bucket].getMax();
            }
        }
    }
private:
    static constexpr int bucketBits = (N + 1) / 2;
    static constexpr int restBits = N - bucketBits;
    int min = 1 << N, max = -1;
    vEB<bucketBits> auxTree;
    vEB<restBits> children[1 << bucketBits];
};

#endif