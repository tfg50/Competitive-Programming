#ifndef DIVISION_TRICK_H
#define DIVISION_TRICK_H
#include "Biblioteca/template_basics.hpp"

template<class F>
void rangeTrickFloor(int n, F f) {
    for(int l = 1, r; l <= n; l = r + 1) {
        r = n / (n / l);
        f(l, r);
        // n / i has the same value for l <= i <= r
    }
}

#endif