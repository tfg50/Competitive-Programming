#ifndef MANACHER_H
#define MANACHER_H
#include "Biblioteca/template_basics.hpp"

// copied from https://github.com/kth-competitive-programming/kactl/blob/master/content/strings/Manacher.h
// manacher[0][i+1] is the length of matches of even length palindrome, starting from [i, i+1]
// manacher[1][i] is the length of matches of odd length palindrome, starting from [i, i]
std::array<std::vector<int>, 2> manacher(const std::string& s) {
    int n = (int) s.size();
    std::array<std::vector<int>, 2> p = {std::vector<int>(n+1), std::vector<int>(n)};
    for(int z = 0; z < 2; z++) for (int i = 0, l = 0, r = 0; i < n; i++) {
        int t = r-i+!z;
        if (i<r) p[z][i] = std::min(t, p[z][l+t]);
        int L = i-p[z][i], R = i+p[z][i]-!z;
        while (L>=1 && R+1<n && s[L-1] == s[R+1])
            p[z][i]++, L--, R++;
        if (R>r) l=L, r=R;
    }
    return p;
}

#endif