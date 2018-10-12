#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <map>
#include <algorithm>

std::mt19937 rng((int) std::chrono::steady_clock::now().time_since_epoch().count());

/*
number of unique numbers in a path
classic mo on tree problem
https://codeforces.com/blog/entry/43230
*/

const int ms = 200100;

bool has[ms];
int val[ms], id[ms];
int freq[ms];

int curAns = 0;
void add(int x) {
	if(freq[x] == 0) curAns++;
	freq[x]++;
}

void del(int x) {
	freq[x]--;
	if(freq[x] == 0) curAns--;
}

void change(int x) {
	x = id[x];
	if(has[x]) {
		del(val[x]);
	} else {
		add(val[x]);
	}
	has[x] = !has[x];
}

std::vector<std::vector<int>> edges;
int in[ms], out[ms], c = 0;

void pre(int on, int par = -1) {
	in[on] = c;
	id[c++] = on;
	for(auto to : edges[on]) {
		if(to == par) continue;
		pre(to, on);
	}
	out[on] = c;
	id[c++] = on;
}

const int bs = 300;

struct Query {
	int l, r, idx, lca;
};

bool operator < (Query a, Query b) {
	if(a.l / bs == b.l / bs) return a.r < b.r;
	else return a.l < b.l;
}

// maybe do something for general edges?

namespace BinaryLifting {
	const int ms = 100100;
	const int me = 18;
	int anc[me][ms];
	int h[ms];

	void dfs(const std::vector<std::vector<int>> &edges, int on, int par, int he = 0) {
		h[on] = he;
		anc[0][on] = par;
		for(auto to : edges[on]) {
			if(to != par) {
				dfs(edges, to, on, he + 1);
			}
		}
	}

	void init(const std::vector<std::vector<int>> &edges, int on = 0) {
		dfs(edges, on, on);
		for(int i = 1; i < me; i++) {
			for(int j = 0; j < edges.size(); j++) {
				anc[i][j] = anc[i-1][anc[i-1][j]];
			}
		}
	}

	int getLCA(int u, int v) {
		if(h[u] < h[v]) {
			std::swap(u, v);
		}
		int ans = h[u] - h[v];
		for(int i = me - 1; i >= 0; i--) {
			if((1 << i) & ans) {
				u = anc[i][u];
			}
		}
		if(u == v) {
			return u;
		}
		for(int i = me - 1; i >= 0; i--) {
			if(anc[i][u] != anc[i][v]) {
				ans += 1 << (i + 1);
				u = anc[i][u];
				v = anc[i][v];
			}
		}
		return anc[0][u];
	}

	int getDist(int u, int v) {
		int lca = getLCA(u, v);
		return h[u] + h[v] - 2 * h[lca];
	}
}

int main() {
	std::ios_base::sync_with_stdio(false); std::cin.tie(NULL);
	int n, m;
	std::cin >> n >> m;
	std::map<int, int> uni;
	for(int i = 0; i < n; i++) {
		std::cin >> val[i];
		if(!uni.count(val[i])) {
			uni[val[i]] = (int) uni.size();
		}
		val[i] = uni[val[i]];
	}
	edges.resize(n);
	for(int i = 1; i < n; i++) {
		int u, v;
		std::cin >> u >> v;
		u--;v--;
		edges[u].push_back(v);
		edges[v].push_back(u);
	}
	pre(0);
	for(int i = 0; i < c; i++) {
		//std::cout << "(" << id[i] << ", " << val[id[i]] << ")" << (i + 1 == c ? '\n' : ' ');
	}
	std::vector<int> ans(m);
	std::vector<Query> qries;
	BinaryLifting::init(edges);
	for(int i = 0; i < m; i++) {
		int u, v;
		std::cin >> u >> v;
		u--;v--;
		if(u == v) {
			ans[i] = 1;
			continue;
		}
		if(in[u] > in[v]) std::swap(u, v);
		Query cur;
		cur.l = in[u];
		cur.r = in[v] + 1;
		cur.idx = i;
		cur.lca = BinaryLifting::getLCA(u, v);
		if(cur.lca == u) {
			cur.lca = -1;
		} else {
			cur.l = out[u];
			cur.r = in[v] + 1;
		}
		//std::cout << "query for " << u << ", " << v << " is [" << cur.l << ", " << cur.r << ")\n";
		//std::cout << "lca is " << cur.lca << "\n";
		qries.push_back(cur);
	}
	std::sort(qries.begin(), qries.end());
	int l = 0, r = 0;
	for(auto q : qries) {
		while(r < q.r) change(r++);
		while(r > q.r) change(--r);
		while(l < q.l) change(l++);
		while(l > q.l) change(--l);
		ans[q.idx] = curAns;
		int lca = q.lca;
		if(lca != -1 && freq[val[lca]] == 0) {
			ans[q.idx]++;
		}
	}
	for(int i = 0; i < m; i++) {
		std::cout << ans[i] << '\n';
	}
}
