#ifndef LEVESHTEIN_H
#define LEVESHTEIN_H
#include "Biblioteca/template_basics.hpp"

std::vector<int> levenshteinDistance(const std::string &a, const std::string &b) {
    int n = (int) a.size();
    std::vector<int> ans(n+1);
    for(int i = 0; i <= n; i++) {
        ans[i] = i;
    }
    for(auto ch : b) {
        auto nxt = ans;
        nxt[0]++;
        for(int j = 1; j <= n; j++) {
            nxt[j] = std::min(nxt[j-1], ans[j]) + 1;
            int cost = ch != a[j-1];
            nxt[j] = std::min(nxt[j], ans[j-1] + cost);
        }
        ans = nxt;
    }
    return ans;
}

#endif