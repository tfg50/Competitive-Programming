#ifndef MODULAR_PA_SUM_H
#define MODULAR_PA_SUM_H
#include "Biblioteca/template_basics.hpp"

//calcula (sum(0 <= i <= n) P(i)) % mod, 
//onde P(i) é uma PA modular (com outro modulo)
namespace sum_pa_mod{
    ll calc(ll a, ll b, ll n, ll mod){
        assert(a&&b);
        if(a >= b){
            ll ret = ((n*(n+1)/2)%mod)*(a/b);
            if(a%b) ret = (ret + calc(a%b,b,n,mod))%mod;
            else ret = (ret+n+1)%mod;
            return ret;
        }
        return ((n+1)*(((n*a)/b+1)%mod) - calc(b,a,(n*a)/b,mod) + mod + n/b + 1)%mod;
    }
    //P(i) = a*i mod m
    ll solve(ll a, ll n, ll m, ll mod){
        a = (a%m + m)%m;
        if(!a) return 0;
        ll ret = (n*(n+1)/2)%mod;
        ret = (ret*a)%mod;
        ll g = __gcd(a,m);
        ret -= m*(calc(a/g,m/g,n,mod)-n-1);
        return (ret%mod + mod)%mod;
    }
    //P(i) = a + r*i mod m
    ll solve(ll a, ll r, ll n, ll m, ll mod){
        a = (a%m + m)%m;
        r = (r%m + m)%m;
        if(!r) return (a*(n+1))%mod;
        if(!a) return solve(r, n, m, mod);
        ll g, x, y;
        g = gcdExtended(r, m, x, y);
        x = (x%m + m)%m;
        ll d = a - (a/g)*g;
        a -= d;
        x = (x*(a/g))%m;
        return (solve(r, n+x, m, mod) - solve(r, x-1, m, mod) + mod + d*(n+1))%mod;
    }
};

#endif