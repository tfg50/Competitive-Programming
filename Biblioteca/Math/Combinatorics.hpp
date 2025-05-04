#ifndef COMBINATORICS_H
#define COMBINATORICS_H
#include "Biblioteca/Math/ModularArithmetic.hpp"

mint fat[ms], ifat[ms];
void initComb() {
    fat[0] = 1;
    for(int i = 1; i < ms; i++) {
        fat[i] = fat[i-1] * i;
    }
    ifat[ms-1] = fexp(fat[ms-1], MOD - 2);
    for(int i = ms-1; i > 0; i--) {
        ifat[i-1] = ifat[i] * i;
    }
}
mint comb(int n, int a) { return a < 0 || a > n ? mint() : fat[n] * ifat[a] * ifat[n-a]; }

template<class T>
std::vector<T> partitionNumber(int n) {
    std::vector<T> ans(n, 0);
    ans[0] = 1;
    for(int i = 1; i < n; i++) {
        for(int j = 1; j * (3 * j + 1) / 2 <= i; j++) {
            ans[i] = ((j & 1) ? ans[i] + ans[i - j * (3 * j + 1) / 2] : ans[i] - ans[i - j * (3 * j + 1) / 2]);
        }
        for(int j = 1; j * (3 * j - 1) / 2 <= i; j++) {
            ans[i] = ((j & 1) ? ans[i] + ans[i - j * (3 * j - 1) / 2] : ans[i] - ans[i - j * (3 * j - 1) / 2]);
        }
    }
    return ans;
}

#endif
