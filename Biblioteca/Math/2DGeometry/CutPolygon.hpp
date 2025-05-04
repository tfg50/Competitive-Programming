#ifndef CUT_POLYGON_H
#define CUT_POLYGON_H
#include "Biblioteca/Math/2DGeometry/PT.hpp"

std::vector<PT> cutPolygon(std::vector<PT> poly, Segment seg) {
    int n = (int) poly.size();
    std::vector<PT> ans;
    for(int i = 0; i < n; i++) {
        double z = seg.plug(poly[i]);
        if(z > -eps) {
            ans.push_back(poly[i]);
        }
        double z2 = seg.plug(poly[(i + 1) % n]);
        if((z > eps && z2 < -eps) || (z < -eps && z2 > eps)) {
            ans.push_back(seg.lineIntersection(Segment(poly[i], poly[(i + 1) % n])));
        }
    }
    return ans;
}

#endif