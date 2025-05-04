#ifndef SEGTREE_NODES_H
#define SEGTREE_NODES_H
#include "Biblioteca/template_basics.hpp"

#ifdef JUST_AN_EXAMPLE

// Range update, range query segment tree
struct LazyContext {
    LazyContext() {}
    void operator += (LazyContext o) {}
    void operator <<= (int size) {}
};

struct Node {
    Node() { /* neutral element */}
    Node() { /* init */}
    Node(Node l, Node r) { /* merge */ }
    bool canBreak(LazyContext lazy) { /* return true if can break without applying lazy */ }
    bool canApply(LazyContext lazy) { /* returns true if can apply lazy */ }
    void apply(LazyContext &lazy) { /* changes lazy if needed */ }
};

#endif

template <class T>
struct AffineLazy {
    AffineLazy(std::pair<T, T> init = std::pair<T, T>(1, 0)) : sum(init) {}
    void operator += (AffineLazy o) { sum = {sum.first * o.sum.first, o.sum.second + o.sum.first * sum.second}; }
    std::pair<T, T> sum;
};

template <class T>
struct AffineNode {
    AffineNode(T v = 0) : sum(v) {}
    AffineNode(AffineNode l, AffineNode r) : sum(l.sum + r.sum) {}
    void apply(AffineLazy<T> lazy, int size, int _) { sum = lazy.sum.first * sum + lazy.sum.second * size; }
    AffineNode operator + (AffineNode o) const { return AffineNode(*this, o); }
    T sum;
};

template <class T = int64_t>
struct SumNode {
    SumNode(T v = 0) { sum = v; }
    SumNode(SumNode l, SumNode r) { sum = l.sum + r.sum; }
    void apply(T lazy, int size, int _) { sum += lazy * size; }
    SumNode operator + (SumNode o) const { return SumNode(*this, o); }
    T sum;
};

template <class T = int32_t>
struct MaxNode {
    MaxNode(T v = 0) { ans = v; }
    MaxNode(MaxNode l, MaxNode r) { ans = std::max(l.ans, r.ans); }
    void apply(T lazy, int size, int _) { ans += lazy; }
    MaxNode operator + (MaxNode o) const { return MaxNode(*this, o); }
    T ans;
};

template <class T = int32_t>
struct MinNode {
    MinNode(T v = 1e9) { ans = v; }
    MinNode(MinNode l, MinNode r) { ans = std::min(l.ans, r.ans); }
    void apply(T lazy, int size, int _) { ans += lazy; }
    MinNode operator + (MinNode o) const { return MinNode(*this, o); }
    T ans;
};

#endif
