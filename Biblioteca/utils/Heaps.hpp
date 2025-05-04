#ifndef HEAPS_H
#define HEAPS_H
#include "Biblioteca/template_basics.hpp"
#include "Biblioteca/utils/Container.hpp"
template<class T>
using MinHeap = std::priority_queue<T, Container<T>, std::greater<T>>;
template<class T>
using MaxHeap = std::priority_queue<T, Container<T>, std::less<T>>;
#endif
