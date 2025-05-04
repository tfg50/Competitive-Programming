#ifndef LAGRANGE_H
#define LAGRANGE_H
#include "Biblioteca/template_basics.hpp"

class LagrangePoly {
public:
    LagrangePoly(std::vector<mint> _a) {
        //f(i) = _a[i]
        //interpola o vetor em um polinomio de grau y.size() - 1
        y = _a;
        den.resize(y.size());
        int n = (int) y.size();
        for(int i = 0; i < n; i++) {
            den[i] = ifat[n - i - 1] * ifat[i] ;
            if((n - i - 1) % 2 == 1) {
                den[i] = -den[i];
            }
        }
    }
    
    mint getVal(mint x) {
        int n = (int) y.size();
        std::vector<mint> l, r;
        l.resize(n);
        l[0] = 1;
        for(int i = 1; i < n; i++) {
            l[i] = l[i - 1] * (x - (i - 1));
        }
        r.resize(n);
        r[n - 1] = 1;
        for(int i = n - 2; i >= 0; i--) {
            r[i] = r[i + 1] * (x - (i + 1));
        }
        mint ans = 0;
        for(int i = 0; i < n; i++) {
            mint coef = l[i] * r[i];
            ans += coef * y[i] * den[i];
        }
        return ans;
    }
    
private:
    std::vector<mint> y, den;
};

#endif