#ifndef KMP_H
#define KMP_H
#include "Biblioteca/template_basics.hpp"

template<class T>
std::vector<int> getBorder(T str) {
    int n = str.size();
    std::vector<int> border(n, -1);
    for(int i = 1, j = -1; i < n; i++) {
        while(j >= 0 && str[i] != str[j + 1]) {
            j = border[j];
        }
        if(str[i] == str[j + 1]) {
            j++;
        }
        border[i] = j;
    }
    return border;
}

template<class T, class F>
void matchPattern(const T &txt, const T &pat, const std::vector<int> &border, F f) {
    for(int i = 0, j = -1; i < (int) txt.size(); i++) {
        while(j >= 0 && txt[i] != pat[j + 1]) {
            j = border[j];
        }
        if(pat[j + 1] == txt[i]) {
            j++;
        }
        if(j + 1 == (int) pat.size()) {
            //found occurence
            f(i - j);
            j = border[j];
        }
    }
}

#endif
