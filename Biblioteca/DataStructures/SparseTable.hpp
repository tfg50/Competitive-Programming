#ifndef SPARSE_TABLE_H
#define SPARSE_TABLE_H
#include "Biblioteca/template_basics.hpp"

template<class T>
struct Minimizer {
    T operator() (T a, T b) { return std::min(a, b); }
};

template <class T, class Merger = Minimizer<T>>
class SparseTable {
public:
    void init(const std::vector<T> &a) {
        int e = 0;
        int n = a.size();
        while((1 << e) / 2 < n) {
            e++;
        }
        table.assign(e, std::vector<T>(n));
        for(int i = 0; i < n; i++) {
            table[0][i] = a[i];
        }
        for(int i = 0; i + 1 < e; i++) {
            for(int j = 0; j + (1 << i) < n; j++) {
                table[i+1][j] = merge(table[i][j], table[i][j + (1 << i)]);
            }
        }
    }

    T qry(int l, int r) {
        int e = getPos(r - l);
        return merge(table[e][l], table[e][r - (1 << e)]);
    }

    int getPos(int x) {
        return 31 - __builtin_clz(x);
    }
private:
    std::vector<std::vector<T>> table;
    Merger merge;
};

#endif
