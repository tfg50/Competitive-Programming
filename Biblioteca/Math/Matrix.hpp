#ifndef MATRIX_H
#define MATRIX_H
#include "Biblioteca/template_basics.hpp"

template<const int n, const int m, class T = int>
struct Matrix {
    T v[n][m];
    
    Matrix(int d = 0) {
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < m; j++) {
                v[i][j] = T(0);
            }
            if(i < m) {
                v[i][i] = T(d);
            }
        }
    }
    
    template<int mm>
    Matrix<n, mm, T> operator *(const Matrix<m, mm, T> &o) {
        Matrix<n, mm, T> ans;
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < mm; j++) {
                for(int k = 0; k < m; k++) {
                    ans.v[i][j] = ans.v[i][j] + v[i][k] * o.v[k][j];
                }
            }
        }
        return ans;
    }
};

#endif