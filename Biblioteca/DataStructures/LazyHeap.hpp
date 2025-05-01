#ifndef LAZY_HEAP_H
#define LAZY_HEAP_H
#include "Biblioteca/template_basics.hpp"
// supports lazy addition to values in the heap

template<class H>
struct LazyHeap {
    int size() const { return (int) hp.size(); }
    long long top() const { return hp.top() + lazy; }
    void pop() { hp.pop(); }
    void push(long long x) { hp.push(x - lazy); }

    long long lazy = 0;
    H hp;
};

#endif