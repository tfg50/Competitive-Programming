#ifndef MATROID_INTERSECTION_H
#define MATROID_INTERSECTION_H
#include "Biblioteca/template_basics.hpp"

// matroid needs:
// add(object), reset(), check()

struct Edge {
    int u, v;
    Edge() {}
    Edge(int a, int b) : u(std::min(a, b)), v(std::max(a, b)) {}
};

class ChoiceMatroid {
public:
    ChoiceMatroid(const std::vector<int> &freq) : limit(freq) {}
    bool check(const Edge &e) { return cur[e.u] && cur[e.v]; }
    void add(const Edge &e) {
        assert(check(e));
        cur[e.u]--;
        cur[e.v]--;
    }
    void clear() {
        cur = limit;
    }
private:
    std::vector<int> limit, cur;
};

class GraphicMatroid {
public:
    GraphicMatroid(int n) : par(n, -1) {}
    bool check(const Edge &e) { return getPar(e.u) != getPar(e.v); }
    void add(const Edge &e) {
        assert(makeUnion(e.u, e.v));
    }
    void clear() {
        par.assign((int) par.size(), -1);
    }
private:
    std::vector<int> par;
    int getPar(int x) { return par[x] < 0 ? x : par[x] = getPar(par[x]); }

    bool makeUnion(int a, int b) {
        a = getPar(a), b = getPar(b);
        if(a == b) return false;
        if(par[a] < par[b]) std::swap(a, b);
        par[b] += par[a];
        par[a] = b;
        return true;
    }
};

// heavy matroid should be M1

template<class M1, class M2, class T>
class MatroidIntersection {
public:
    MatroidIntersection(M1 m1, M2 m2, const std::vector<T> &obj) {
        m1.clear();
        m2.clear();
        {
            // removing useless elements
            for(auto ob : obj) {
                if(m1.check(ob) && m2.check(ob)) {
                    ground.push_back(ob);
                }
            }
            n = (int) ground.size();
            present.assign(n, false);
        }
        // greedy step
        for(int i = 0; i < n; i++) {
            if(m1.check(ground[i]) && m2.check(ground[i])) {
                present[i] = true;
                m1.add(ground[i]);
                m2.add(ground[i]);
            }
        }
        // augment step
        while(augment(m1, m2));
    }

    std::vector<T> getSet() {
        std::vector<T> ans;
        for(int i = 0; i < n; i++) {
            if(present[i]) {
                ans.push_back(ground[i]);
            }
        }
        return ans;
    }
private:
    int n;
    std::vector<T> ground;
    std::vector<int> curSet, dist;
    std::vector<bool> present;

    template<class M>
    void loadGround(M &m) {
        m.clear(), curSet.clear();
        for(int i = 0; i < n; i++) {
            if(present[i]) {
                m.add(ground[i]);
                curSet.push_back(i);
            }
        }
    }

    template<class M>
    int forwardEdge(int id, bool step, M &m) {
        assert(!present[id]);
        m.clear();
        m.add(ground[id]);
        for(auto i : curSet) {
            if(dist[i] != (step ? dist[id]-1 : -1)) {
                if(m.check(ground[i])) {
                    m.add(ground[i]);
                }
            }
        }
        for(auto i : curSet) {
            if(dist[i] == (step ? dist[id]-1 : -1)) {
                if(m.check(ground[i])) {
                    m.add(ground[i]);
                } else {
                    return i;
                }
            }
        }
        return -1;
    }

    template<class M>
    int backwardEdge(int id, bool step, M &m) {
        assert(present[id]);
        m.clear();
        for(auto i : curSet) {
            if(i != id) {
                m.add(ground[i]);
            }
        }
        for(int i = 0; i < n; i++) {
            if(!present[i] && dist[i] == (step ? dist[id]-1 : -1)) {
                if(m.check(ground[i])) {
                    return i;
                }
            }
        }
        return -1;
    }

    bool augment(M1 &m1, M2 &m2) {
        std::queue<int> q;
        dist.assign(n, -1);
        std::vector<int> frm(n, -1);
        loadGround(m1);
        for(int i = 0; i < n; i++) {
            if(!present[i] && m1.check(ground[i])) {
                q.push(i);
                dist[i] = 0;
            }
        }
        if(q.empty()) {
            return false;
        }
        auto getEdge = [&](int id, bool step) {
            if(!step) return dist[id] % 2 == 0 ? forwardEdge(id, step, m2) : backwardEdge(id, step, m1);
            else return dist[id] % 2 == 0 ? forwardEdge(id, step, m1) : backwardEdge(id, step, m2);
        };
        M2 checker2 = m2;
        loadGround(checker2);
        bool got = false;
        int limit = n + 1;
        while(!q.empty()) {
            int on = q.front();
            q.pop();
            if(dist[on] == limit) continue;
            for(int i = getEdge(on, false); i != -1; i = getEdge(on, false)) {
                q.push(i);
                dist[i] = dist[on] + 1;
                frm[i] = on;
                if(!present[i] && checker2.check(ground[i])) {
                    got = true;
                    limit = dist[i];
                    /*for(int pos = i; pos != -1; pos = frm[pos]) {
                        present[pos] = !present[pos];
                    }
                    return true;*/
                }
            }
        }
        if(!got) { return false; }
        M1 checker1 = m1;
        loadGround(checker1);
        std::function<bool(int)> dfs = [&](int on) {
            if(dist[on] == limit && !checker2.check(ground[on])) {
                dist[on] = -1;
                return false;
            }
            if(dist[on] == 0) {
                dist[on] = -1;
                if(checker1.check(ground[on])) {
                    present[on] = !present[on];
                    return true;
                } else {
                    return false;
                }
            }
            for(int to = getEdge(on, true); to != -1; to = getEdge(on, true)) {
                assert(dist[to] == dist[on]-1);
                if(dfs(to)) {
                    dist[on] = -1;
                    present[on] = !present[on];
                    return true;
                }
            }
            dist[on] = -1;
            return false;
        };
        got = false;
        for(int i = 0; i < n; i++) {
            if(dist[i] == limit && dfs(i)) {
                loadGround(checker1);
                loadGround(checker2);
                got = true;
            }
        }
        assert(got);
        return true;
    }
};

template<class M, class T>
class MatroidUnion {
public:
    MatroidUnion(M m, std::vector<T> objs) : base(m) {
        ground = objs;
        base.clear();
        n = (int) ground.size();
        mats = 0;
        curSet.emplace_back(0);
        col.assign(n, -1);
    }

    std::vector<int> getCol() { return col; }
    MatroidUnion<M, T>& addMatroids(int x) {
        while(x--) {
            mats++;
            mat.push_back(base);
            mat.back().clear();
            curSet.emplace_back(0);
        }
        return *this;
    }
    MatroidUnion<M, T>& solve() {
        // greedy step
        fix();
        for(int i = 0; i < mats; i++) {
            for(int j = 0; j < n; j++) {
                if(col[j] == -1 && mat[i].check(ground[j])) {
                    col[j] = i;
                    curSet[i].push_back(j);
                    mat[i].add(ground[j]);
                }
            }
        }
        M m = base;
        while(augment(m));
        return *this;
    }
private:
    int n, mats;
    std::vector<int> col, dist, pt;
    std::vector<T> ground;
    std::vector<M> mat;
    std::vector<std::vector<int>> curSet;
    M base;

    void fix() {
        for(int i = 0; i < mats; i++) {
            mat[i].clear();
            curSet[i].clear();
        }
        for(int i = 0; i < n; i++) {
            if(col[i] != -1) {
                mat[col[i]].add(ground[i]);
                curSet[col[i]].push_back(i);
            } else {
                curSet[mats].push_back(i);
            }
        }
    }

    bool augment(M &m) {
        std::queue<int> q;
        dist.assign(n, -1);
        pt.assign(n, 0);
        std::vector<int> frm(n, -1);
        m.clear();
        for(int i = 0; i < n; i++) {
            if(col[i] == -1 && m.check(ground[i])) {
                q.push(i);
                dist[i] = 0;
            }
        }
        if(q.empty()) {
            return false;
        }
        auto getEdge = [&](int id, bool step) {
            if(!step) {
                for(; pt[id] < mats; pt[id]++) {
                    int curMat = pt[id];
                    if(curMat == col[id]) continue;
                    m.clear();
                    m.add(ground[id]);
                    for(auto i : curSet[curMat]) {
                        if(dist[i] != -1) {
                            if(m.check(ground[i])) {
                                m.add(ground[i]);
                            }
                        }
                    }
                    for(auto i : curSet[curMat]) {
                        if(dist[i] == -1) {
                            if(m.check(ground[i])) {
                                m.add(ground[i]);
                            } else {
                                return i;
                            }
                        }
                    }
                }
            } else {
                m.clear();
                for(auto i : curSet[col[id]]) {
                    if(i != id) {
                        m.add(ground[i]);
                    }
                }
                for(; pt[id] <= mats; pt[id]++) {
                    int curMat = pt[id];
                    if(curMat == col[id]) continue;
                    for(auto i : curSet[curMat]) {
                        if(dist[i] == dist[id]-1 && m.check(ground[i])) {
                            return i;
                        }
                    }
                }
            }
            return -1;
        };
        bool got = false;
        int limit = n + 1;
        while(!q.empty()) {
            int on = q.front();
            q.pop();
            if(dist[on] == limit) continue;
            for(int i = getEdge(on, false); i != -1; i = getEdge(on, false)) {
                q.push(i);
                dist[i] = dist[on] + 1;
                frm[i] = on;
                int good = -1;
                for(int j = 0; j < mats; j++) {
                    if(j != col[i] && mat[j].check(ground[i])) {
                        good = j;
                    }
                }
                if(good != -1) {
                    got = true;
                    limit = dist[i];
                    /*for(int pos = i; pos != -1; pos = frm[pos]) {
                        std::swap(good, col[pos]);
                    }
                    fix();
                    assert(good == -1);
                    return true;*/
                }
            }
        }
        if(!got) { return false; }
        pt.assign(n, 0);
        got = false;
        std::function<bool(int, int)> dfs = [&](int on, int c) {
            if(c == -1) {
                for(int i = 0; i < mats; i++) {
                    if(i != col[on] && mat[i].check(ground[on])) {
                        c = i;
                    }
                }
            }
            if(c == -1) {
                dist[on] = -1;
                return false;
            }
            if(dist[on] == 0) {
                dist[on] = -1;
                col[on] = c;
                return true;
            }
            for(int to = getEdge(on, true); to != -1; to = getEdge(on, true)) {
                assert(dist[to] == dist[on]-1);
                if(dfs(to, col[on])) {
                    dist[on] = -1;
                    col[on] = c;
                    return true;
                }
            }
            dist[on] = -1;
            return false;
        };
        for(int i = 0; i < n; i++) {
            if(dist[i] == limit && dfs(i, -1)) {
                got = true;
                fix();
            }
        }
        assert(got);
        return true;
    }
};

#endif
