#ifndef CONVEX_HULL_H
#define CONVEX_HULL_H
#include "Biblioteca/Math/2DGeometry/PT.hpp"

std::vector<PT> splitHull(const std::vector<PT> &hull) {
    std::vector<PT> ans(hull.size());
    for(int i = 0, j = (int) hull.size()-1, k = 0; k < (int) hull.size(); k++) {
        if(hull[i] < hull[j]) {
            ans[k] = hull[i++];
        } else {
            ans[k] = hull[j--];
        }
    }
    return ans;
}

std::vector<PT> ConvexHull (std::vector<PT> pts, bool needs = true) {
    if(needs) {
        std::sort(pts.begin(), pts.end());
    }
    pts.resize(std::unique(pts.begin(), pts.end()) - pts.begin());
    if(pts.size() <= 1) return pts;
    std::vector<PT> ans(pts.size() * 2 + 1);
    int s = 0;
    for(int i = 0; i < (int) pts.size(); i++) {
        while(s > 1 && (pts[i] - ans[s - 2]) % (ans[s - 1] - ans[s - 2]) >= 0) {
            s--;
        }
        ans[s++] = pts[i];
    }
    for(int i = (int) pts.size() - 2, t = s + 1; i >= 0; i--) {
        while(s >= t && (pts[i] - ans[s - 2]) % (ans[s - 1] - ans[s - 2]) >= 0) {
            s--;
        }
        ans[s++] = pts[i];
    }
    ans.resize(s-1);
    return ans;
}

std::vector<PT> ConvexHull(const std::vector<PT> &a, const std::vector<PT> &b) {
    auto A = splitHull(a);
    auto B = splitHull(b);
    std::vector<PT> C(A.size() + B.size());
    std::merge(A.begin(), A.end(), B.begin(), B.end(), C.begin());
    return ConvexHull(C, false);
}

bool isInside(const std::vector<PT> &hull, PT point) {
    int n = hull.size();
    if((point - hull[0]) % (hull[1] - hull[0]) > 0 || (point - hull[0]) % (hull[n-1] - hull[0]) < 0) {
        return false;
    }
    int l = 1, r = n - 1;
    while(l != r) {
        int mid = (l + r + 1) / 2;
        if((point - hull[0]) % (hull[mid] - hull[0]) < 0) {
            l = mid;
        } else {
            r = mid - 1;
        }
    }
    return (hull[(l+1)%n] - hull[l]) % (point - hull[l]) >= 0;
}

int maximizeScalarProduct(const std::vector<PT> &hull, PT vec) {
    // this code assumes that there are no 3 colinear points
    int ans = 0;
    int n = (int) hull.size();
    if(n < 20) {
        for(int i = 0; i < n; i++) {
            if(hull[i] * vec > hull[ans] * vec) {
                ans = i;
            }
        }
    } else {
        if(hull[1] * vec > hull[ans] * vec) {
            ans = 1;
        }
        for(int rep = 0; rep < 2; rep++) {
            int l = 2, r = n - 1;
            while(l != r) {
                int mid = (l + r + 1) / 2;
                bool flag = hull[mid] * vec >= hull[mid-1] * vec;
                if(rep == 0) { flag = flag && hull[mid] * vec >= hull[0] * vec; }
                else { flag = flag || hull[mid-1] * vec < hull[0] * vec; }
                if(flag) {
                    l = mid;
                } else {
                    r = mid - 1;
                }
            }
            if(hull[ans] * vec < hull[l] * vec) {
                ans = l;
            }
        }
    }
    return ans;
}

#endif