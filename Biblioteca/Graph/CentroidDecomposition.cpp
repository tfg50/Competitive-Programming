// sample solution from Digit Tree
struct Hash {
	Hash() { pot = 1, ans[0] = ans[1] = 0; }
	Hash(int v) {
		pot = 10 % MOD;
		ans[0] = ans[1] = v % MOD;
	}
	Hash(Hash h1, Hash h2) {
		pot = (int) ((long long) h1.pot * h2.pot % MOD);
		ans[0] = (int) (((long long) h1.ans[0] * h2.pot + h2.ans[0]) % MOD);
		ans[1] = (int) ((h1.ans[1] + (long long) h2.ans[1] * h1.pot) % MOD);
	}
	int pot, ans[2];
};

struct Solver {
	long long operator()(const std::vector<Hash> &a) {
		cur.clear();
		for(auto h : a) {
			cur.push_back(h.ans[1]);
		}
		std::sort(cur.begin(), cur.end());
		long long ans = 0;
		for(auto h : a) {
			int want = (int) (-h.ans[0] * modinverse(h.pot, MOD) % MOD + MOD) % MOD;
			ans += std::upper_bound(cur.begin(), cur.end(), want) - std::lower_bound(cur.begin(), cur.end(), want);
		}
		return ans;
	}

	std::vector<int> cur;
};

template<class Cost>
class CentroidDecomposition {
public:
	struct Edge {
		Edge(){}
		Edge(int v, Cost cost) : to(v), c(cost) {}
		int to;
		Cost c;
	};
	CentroidDecomposition(int _n) : n(_n), edges(_n), tree(_n), col(_n, -1), size(_n), pt(_n, 0) {}

	void addEdge(int u, int v, Cost c) {
		edges[u].emplace_back(v, c);
		edges[v].emplace_back(u, c);
		pt[u]++, pt[v]++;
	}

	int init() {
		int ans = init(0, 0);
		for(int i = 0; i < n; i++) {
			std::sort(edges[i].begin(), edges[i].end(), [&](Edge e1, Edge e2) { return col[e1.to] > col[e2.to]; });
		}
		return ans;
	}

	// to get only one centroid
	int getCentroid() {
		preDFS(0, -1);
		return getCen(0);
	}

	template<class Solver, class Info>
	long long solveEdges(Solver sol, Info base) {
		long long ans = 0;
		std::vector<Info> a, b;
		for(int i = 0; i < n; i++) {
			b.clear();
			b.push_back(base);
			for(int j = 0; j < pt[i]; j++) {
				a.clear();
				dfs(a, edges[i][j].to, i, col[i], Info(base, edges[i][j].c));
				ans -= sol(a);
				for(auto v : a) b.push_back(v);
			}
			ans += sol(b);
		}
		return ans;
	}

	template<class Info>
	void dfs(std::vector<Info> &vec, int on, int par, int lim, Info h) {
		vec.push_back(h);
		for(auto e : edges[on]) {
			if(e.to == par) continue;
			if(col[e.to] <= lim) break;
			dfs(vec, e.to, on, lim, Info(h, e.c));
		}
	}

private:
	void preDFS(int on, int par) {
		size[on] = 1;
		for(int i = 0; i < pt[on]; i++) {
			int to = edges[on][i].to;
			if(col[to] != -1 || to == par) {
				std::swap(edges[on][i], edges[on][pt[on]-1]);
				if(col[to] != -1) {
					pt[on]--;
					i--;
				} else if(i+1 < pt[on]) {
					i--;
				}
				continue;
			}
			preDFS(to, on);
			size[on] += size[to];
			if(size[to] > size[edges[on][0].to]) {
				std::swap(edges[on][0], edges[on][i]);
			}
		}
	}

	int getCen(int on) {
		int sz = size[on];
		while(size[on] > 1 && 2 * size[edges[on][0].to] >= sz) on = edges[on][0].to;
		return on;
	}

	int init(int on, int c) {
		preDFS(on, -1);
		on = getCen(on);
		col[on] = c;
		for(auto e : edges[on]) {
			if(col[e.to] == -1) {
				tree[on].push_back(init(e.to, c+1));
			}
		}
		return on;
	}

	int n;
	std::vector<std::vector<Edge>> edges;
	std::vector<std::vector<int>> tree; // centroid tree edges
	std::vector<int> col, size, pt;
};