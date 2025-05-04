#ifndef MINKOWSKI_H
#define MINKOWSKI_H
#include "Biblioteca/Math/2DGeometry/PT.hpp"

bool comp(PT a, PT b){
    int hp1 = (a.x < 0 || (a.x==0 && a.y<0));
    int hp2 = (b.x < 0 || (b.x==0 && b.y<0));
    if(hp1 != hp2) return hp1 < hp2;
    long long R = a%b;
    if(R) return R > 0;
    return a*a < b*b;
}

// This code assumes points are ordered in ccw and the first points in both vectors is the min lexicographically
std::vector<PT> minkowskiSum(const std::vector<PT> &a, const std::vector<PT> &b){
    if(a.empty() || b.empty()) return std::vector<PT>(0);
    std::vector<PT> ret;
    int na = (int) a.size(), nb = (int) b.size();
    if(std::min(a.size(), b.size()) < 2){
        for(int i = 0; i < na; i++) {
            for(int j = 0; j < nb; j++) {
                ret.push_back(a[i]+b[j]);
            }
        }
        return ret;
    }
    ret.push_back(a[0]+b[0]);
    PT p = ret.back();
    int pa = 0, pb = 0;
    while(pa + pb + 1 < na + nb) {
        PT va = (a[(pa+1)%na]-a[pa%na]);
        PT vb = (b[(pb+1)%nb]-b[pb%nb]);
        if(pb == nb || (pa < na && comp(va, vb))) { p = p + va, pa++; }
        else { p = p + vb, pb++; }
        while(ret.size() >= 2 && (p-ret.back()) % (p-ret[(int)ret.size()-2]) == 0) {
            // removing colinear points
            // needs the scalar product stuff it the result is a line
            ret.pop_back();
        }
        ret.push_back(p);
    }
    return ret;
}

#endif