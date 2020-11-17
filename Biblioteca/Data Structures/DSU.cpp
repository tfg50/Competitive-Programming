struct DSU {
	int getSize(int x) { return -par[getPar(x)]; }
	int getPar(int x) {
		while(par[x] >= 0) {
			x = par[x];
		}
		return x;
	}
 
	void makeUnion(int a, int b) {
		a = getPar(a), b = getPar(b);
		if(a == b) return;
		if(par[a] > par[b]) std::swap(a, b);
		op.emplace_back(a, par[a]);
		op.emplace_back(b, par[b]);
		par[a] += par[b];
		par[b] = a;
	}
 
	void init(int n) {
		par.resize(n);
		for(int i = 0; i < n; i++) {
			par[i] = -1;
		}
		op.clear();
	}
 
	void rollBack() {
		par[op.back().first] = op.back().second;
		op.pop_back();
	}
 
	std::vector<int> par;
	std::vector<std::pair<int, int>> op;
};

struct Edge {
	int l, r, u, v;
};

const int ms = 500500;
std::vector<int> qries[ms];

DSU dsu;

void solve(int l, int r, std::vector<Edge> hmm) {
	int st = (int) dsu.op.size();
	std::vector<Edge> down;
	for(auto e : hmm) {
		if(e.r <= l || e.l >= r) {
 
		} else if(e.l <= l && r <= e.r) {
			dsu.makeUnion(e.u, e.v);
		} else {
			down.push_back(e);
		}
	}
	hmm.clear();
	if(l + 1 == r) {
		for(auto u : qries[l]) {
			std::cout << dsu.getSize(u) << '\n';
		}
	} else {
		int mid = (l + r) / 2;
		solve(l, mid, down);
		solve(mid, r, down);
	}
	while((int) dsu.op.size() > st) {
		dsu.rollBack();
	}
}