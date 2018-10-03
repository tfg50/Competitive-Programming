#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>

std::mt19937 rng((int) std::chrono::steady_clock::now().time_since_epoch().count());

const int ms = 100100;

int ans[ms];
std::pair<int, int> edges[ms];

int par[ms], size[ms];

int getPar(int x) { return x == par[x] ? x : par[x] = getPar(par[x]); }

bool makeUnion(int a, int b) {
	a = getPar(a);
	b = getPar(b);
	if(a == b) return false;
	if(size[a] > size[b]) std::swap(a, b);
	size[b] += size[a];
	par[a] = b;
	return true;
}

void reset(int n) {
	for(int i = 0; i < n; i++) {
		par[i] = i;
		size[i] = 1;
	}
}

struct Query {
	int l, r, id, want, u, v;
};

int main() {
	// code started in :16
	std::ios_base::sync_with_stdio(false); std::cin.tie(NULL);
	int n, m;
	std::cin >> n >> m;
	for(int i = 0; i < m; i++) {
		int u, v;
		std::cin >> u >> v;
		u--;v--;
		edges[i] = {u, v};
	}
	std::vector<Query> old;
	int q;
	std::cin >> q;
	for(int i = 0; i < q; i++) {
		int u, v, z;
		std::cin >> u >> v >> z;
		u--;v--;
		Query cur;
		cur.l = 0;
		cur.r = m;
		cur.id = i;
		cur.want = z;
		cur.u = u;
		cur.v = v;
		old.push_back(cur);
	}
	while(!old.empty()) {
		reset(n);
		std::vector<Query> nxt;
		int pt = 0;
		//std::cout << "starting new!\n";
		std::sort(old.begin(), old.end(), [](Query a, Query b) { return a.l < b.l; });
		for(auto qry : old) {
			int l = qry.l;
			int r = qry.r;
			//std::cout << "got query (" << l << ", " << r << ") id " << qry.id << std::endl;
			if(l == r) {
				ans[qry.id] = l;
				continue;
			}
			int mid = (l + r) / 2;
			while(pt < mid) {
				makeUnion(edges[pt].first, edges[pt].second);
				pt++;
			}
			int got = 0;
			if(getPar(qry.u) == getPar(qry.v)) {
				got += size[getPar(qry.u)];
			} else {
				got += size[getPar(qry.u)];
				got += size[getPar(qry.v)];
			}
			if(got >= qry.want) {
				qry.r = mid;
			} else {
				qry.l = mid + 1;
			}
			nxt.push_back(qry);
		}
		nxt.swap(old);
	}
	for(int i = 0; i < q; i++) {
		std::cout << ans[i] << std::endl;
	}
}
