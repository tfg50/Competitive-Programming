// example matroids

struct Edge {
	int u, v;
	Edge() {}
	Edge(int a, int b) : u(std::min(a, b)), v(std::max(a, b)) {}
	bool operator < (const Edge &o) const { return u != o.u ? u < o.u : v < o.v; }
	bool operator == (const Edge &o) const { return u == o.u && v == o.v; }
};

class ChoiceMatroid {
public:
	ChoiceMatroid(const std::vector<int> &freq) : limit(freq) {}
	bool isIndependent(const std::vector<Edge> &a) {
		return getRank(a) == (int) a.size();
	}
	int getRank(const std::vector<Edge> &a) {
		cur = limit;
		int ans = 0;
		for(auto e : a) {
			if(cur[e.u] && cur[e.v]) {
				cur[e.u]--;
				cur[e.v]--;
				ans++;
			}
		}
		return ans;
	}
private:
	std::vector<int> limit, cur;
};

class GraphicMatroid {
public:
	GraphicMatroid(int n) : par(n, -1) {}
	bool isIndependent(const std::vector<Edge> &a) {
		return getRank(a) == (int) a.size();
	}
	int getRank(const std::vector<Edge> &a) {
		int ans = 0;
		for(auto e : a) {
			ans += makeUnion(e.u, e.v) ? 1 : 0;
		}
		for(auto e : a) {
			par[e.u] = par[e.v] = -1;
		}
		return ans;
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

// to get answer just call MatroidIntersection<M1, M2, T>(m1, m2, obj).getSet()

// slow but smaller O(N^3) * oracle with good constant
// TODO: make it even faster

template<class M1, class M2, class T>
class MatroidIntersection {
public:
	MatroidIntersection(M1 m1, M2 m2, const std::vector<T> &obj) :
			n((int) obj.size()), objects(obj), present(obj.size(), false) {
		// greedy step
		for(int i = 0; i < n; i++) {
			if(test(m1, i) && test(m2, i)) {
				present[i] = true;
			}
		}
		// augment step
		while(augment(m1, m2));
	}
 
	std::vector<T> getSet(int o = -1) {
		std::vector<T> ans;
		for(int i = 0; i < n; i++) {
			if(present[i] && i != o) {
				ans.push_back(objects[i]);
			}
		}
		return ans;
	}
private:
	int n;
	std::vector<T> objects;
	std::vector<bool> present;
 
	template<class M>
	bool test(M &m, int add, int rem = -1) {
		if(present[add] || (rem != -1 && !present[rem])) return false;
		auto st = getSet(rem);
		st.push_back(objects[add]);
		return m.isIndependent(st);
	}
 
	bool augment(M1 &m1, M2 &m2) {
		bool got = false;
		std::queue<int> q;
		std::vector<int> dist(n, -1);
		std::vector<int> frm(n, -1);
		std::vector<std::vector<int>> layers;
		for(int i = 0; i < n; i++) {
			if(test(m1, i)) {
				q.push(i);
				dist[i] = 0;
			}
		}
		if(q.empty()) {
			return false;
		}
		int limit = 1e9;
		while(!q.empty()) {
			int on = q.front();
			q.pop();
			if(dist[on] > limit) {
				dist[on] = -1;
				continue;
			}
			for(int i = 0; i < n; i++) {
				if(dist[i] == -1 && (dist[on] % 2 == 0 ? test(m2, on, i) : test(m1, i, on))) {
					q.push(i);
					dist[i] = dist[on] + 1;
					frm[i] = on;
					if(test(m2, i)) {
						limit = dist[i];
						for(int pos = i; pos != -1; pos = frm[pos]) {
							present[pos] = !present[pos];
						}
						assert(m1.isIndependent(getSet()) && m2.isIndependent(getSet()));
						return true;
					}
				}
			}
		}
		return got;
	}
};

// fast O(N*R^1.5*logN matroid intersection) * oracle
// implementation of https://arxiv.org/pdf/1911.10765.pdf

template<class M1, class M2, class T>
class MatroidIntersection {
public:
	MatroidIntersection(M1 m1, M2 m2, const std::vector<T> &obj) :
			n((int) obj.size()), objects(obj), present(obj.size(), false) {
		// greedy step
		for(int i = 0; i < n; i++) {
			if(test(m1, i) && test(m2, i)) {
				present[i] = true;
				curAns++;
			}
		}
		// augment step
		while(augment(m1, m2));
	}

	std::vector<T> getSet(int o = -1) {
		std::vector<T> ans;
		for(int i = 0; i < n; i++) {
			if(present[i] && i != o) {
				ans.push_back(objects[i]);
			}
		}
		return ans;
	}
private:
	int curAns = 0;
	int n;
	std::vector<T> objects;
	std::vector<bool> present;

	template<class M>
	bool test(M &m, int add, int rem = -1) {
		if(present[add] || (rem != -1 && !present[rem])) return false;
		auto st = getSet(rem);
		st.push_back(objects[add]);
		return m.isIndependent(st);
	}

	bool augment(M1 &m1, M2 &m2) {
		std::queue<int> q;
		std::vector<int> dist(n, -1);
		std::vector<int> frm(n, -1);
		std::vector<std::vector<int>> layers;
		for(int i = 0; i < n; i++) {
			if(test(m1, i)) {
				q.push(i);
				dist[i] = 0;
			}
		}
		if(q.empty()) {
			return false;
		}
		int limit = 1e9;
		// faster algorithm helper functions:
		auto outArc = [&](int u, bool phase) {
			// TODO: optimize this to return every good candidate
			std::vector<T> st;
			std::vector<int> others;
			if(present[u]) {
				// find free
				for(int i = 0; i < n; i++) {
					if(present[i] && i != u) {
						st.push_back(objects[i]);
					} else if(!present[i] && dist[i] == (phase ? dist[u] + 1 : -1)) {
						others.push_back(i);
					}
				}
				auto _test = [&](int l, int r) {
					auto cur = st;
					for(int i = l; i < r; i++) {
						cur.push_back(objects[others[i]]);
					}
					return m1.getRank(cur) >= curAns;
				};
				int l = 0, r = (int) others.size();
				if(l == r || !_test(l, r)) return -1;
				while(l + 1 != r) {
					int mid = (l + r) / 2;
					if(_test(l, mid)) {
						r = mid;
					} else {
						l = mid;
					}
				}
				return others[l];
			} else {
				// find exchange
				for(int i = 0; i < n; i++) {
					if(present[i] && dist[i] != (phase ? dist[u] + 1 : -1)) {
						st.push_back(objects[i]);
					} else if(present[i]) {
						others.push_back(i);
					}
				}
				auto _test = [&](int l, int r) {
					auto cur = st;
					for(int i = 0; i < l; i++) {
						cur.push_back(objects[others[i]]);
					}
					for(int i = r; i < (int) others.size(); i++) {
						cur.push_back(objects[others[i]]);
					}
					cur.push_back(objects[u]);
					return m2.isIndependent(cur);
				};
				int l = 0, r = (int) others.size();
				if(l == r || !_test(l, r)) return -1;
				while(l + 1 != r) {
					int mid = (l + r) / 2;
					if(_test(l, mid)) {
						r = mid;
					} else {
						l = mid;
					}
				}
				return others[l];
			}
		};
		while(!q.empty()) {
			int on = q.front();
			q.pop();
			if((int) layers.size() <= dist[on]) layers.emplace_back(0);
			layers[dist[on]].push_back(on);
			if(dist[on] == limit) continue;
			for(int i = outArc(on, false); i != -1; i = outArc(on, false)) {
				assert(dist[i] == -1 && (dist[on] % 2 == 0 ? test(m2, on, i) : test(m1, i, on)));
				q.push(i);
				dist[i] = dist[on] + 1;
				frm[i] = on;
				if(limit > n && test(m2, i)) {
					limit = dist[i];
					// remove continue and code below to be maybe faster
					// but worse complexity
					continue;
					for(on = i; on != -1; on = frm[on]) {
						present[on] = !present[on];
					}
					curAns++;
					return true;
				}
			}
		}
		if(limit > n) return false;
		auto rem = [&](int on) {
			assert(dist[on] != -1);
			auto it = std::find(layers[dist[on]].begin(), layers[dist[on]].end(), on);
			assert(it != layers[dist[on]].end());
			layers[dist[on]].erase(it);
			dist[on] = -1;
		};
		std::function<bool(int)> dfs = [&](int on) {
			if(dist[on] == 0 && !test(m1, on)) {
				rem(on);
				return false;
			}
			if(dist[on] == limit) {
				rem(on);
				if(test(m2, on)) {
					present[on] = !present[on];
					return true;
				} else {
					return false;
				}
			}
			for(int to = outArc(on, true); to != -1; to = outArc(on, true)) {
				if(dfs(to)) {
					rem(on);
					present[on] = !present[on];
					return true;
				}
			}
			rem(on);
			return false;
		};
		bool got = false;
		while(!layers[0].empty()) {
			if(dfs(layers[0].back())) {
				got = true;
				assert(m1.isIndependent(getSet()) && m2.isIndependent(getSet()));
				curAns++;
			}
		}
		assert(got);
		return true;
	}
};