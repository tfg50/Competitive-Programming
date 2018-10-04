#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <cassert>

std::mt19937 rng((int) std::chrono::steady_clock::now().time_since_epoch().count());

class HLD {
public:
	void init(int n) {
		// this doesn't delete edges!
		sz.resize(n);
		in.resize(n);
		out.resize(n);
		rin.resize(n);
		p.resize(n);
		edges.resize(n);
		nxt.resize(n);
		h.resize(n);
	}

	void addEdge(int u, int v) {
		edges[u].push_back(v);
		edges[v].push_back(u);
	}

	void setRoot(int n) {
		t = 0;
		p[n] = n;
		h[n] = 0;
		prep(n, n);
		nxt[n] = n;
		hld(n);
	}

	int getLCA(int u, int v) {
		while(!inSubtree(nxt[u], v)) {
			u = p[nxt[u]];
		}
		while(!inSubtree(nxt[v], u)) {
			v = p[nxt[v]];
		}
		return in[u] < in[v] ? u : v;
	}

	bool inSubtree(int u, int v) {
		// is v in the subtree of u?
		return in[u] <= in[v] && in[v] < out[u];
	}

	std::vector<std::pair<int, int>> getPathtoAncestor(int u, int anc) {
		// returns ranges [l, r) that have the path
		std::vector<std::pair<int, int>> ans;
		//assert(inSubtree(anc, u));
		while(nxt[u] != nxt[anc]) {
			ans.emplace_back(in[nxt[u]], in[u] + 1);
			u = p[nxt[u]];
		}
		// this includes the ancestor!
		ans.emplace_back(in[anc], in[u] + 1);
		return ans;
	}
//private:
	std::vector<int> in, out, p, rin, sz, nxt, h;
	std::vector<std::vector<int>> edges;
	int t;

	void prep(int on, int par) {
		sz[on] = 1;
		p[on] = par;
		for(int i = 0; i < (int) edges[on].size(); i++) {
			int &u = edges[on][i];
			if(u == par) {
				std::swap(u, edges[on].back());
				edges[on].pop_back();
				i--;
			} else {
				h[u] = 1 + h[on];
				prep(u, on);
				sz[on] += sz[u];
				if(sz[u] > sz[edges[on][0]]) {
					std::swap(edges[on][0], u);
				}
			}
		}
	}

	void hld(int on) {
		in[on] = t++;
		rin[in[on]] = on;
		for(auto u : edges[on]) {
			nxt[u] = (u == edges[on][0] ? nxt[on] : u);
			hld(u);
		}
		out[on] = t;
	}
};

struct Node {
	Node(int v = 1e9) {
		// init
		ans = v;
	}
	
	Node(Node l, Node r) {
		// merge
		ans = std::min(l.ans, r.ans);
	}

	// atributes
	int ans;
};

template <class i_t, class e_t>
class SegmentTree {
public:
	void init(std::vector<e_t> base) {
		n = base.size();
		tree.resize(2 * n);
		for(int i = 0; i < n; i++) {
			tree[i + n] = i_t(base[i]);
		}
		for(int i = n - 1; i > 0; i--) {
			tree[i] = i_t(tree[i + i], tree[i + i + 1]);
		}
	}
	
	i_t qry(int l, int r) {
		i_t lp, rp;
		for(l += n, r += n; l < r; l /= 2, r /= 2) {
			if(l & 1) lp = i_t(lp, tree[l++]);
			if(r & 1) rp = i_t(tree[--r], rp);
		}
		return i_t(lp, rp);
	}
	
	void upd(int pos, e_t v) {
		pos += n;
		tree[pos] = i_t(v);
		for(pos /= 2; pos > 0; pos /= 2) {
			tree[pos] = i_t(tree[pos + pos], tree[pos + pos + 1]);
		}
	}
	
private:
	int n;
	std::vector<i_t> tree;
};


int main() {
	std::ios_base::sync_with_stdio(false); std::cin.tie(NULL);
	int n, m;
	std::cin >> n >> m;
	HLD hld;
	hld.init(n);
	for(int i = 1; i < n; i++) {
		int u, v;
		std::cin >> u >> v;
		u--;v--;
		hld.addEdge(u, v);
	}
	hld.setRoot(0);
	SegmentTree<Node, int> tree1, tree2;
	tree1.init(std::vector<int>(n, (int)1e9));
	tree2.init(std::vector<int>(n, (int)1e9));
	std::vector<int> weights(n, (int) 1e9);
	auto upd = [&](int v) {
		int h = hld.h[v];
		tree1.upd(hld.in[v], h);
		auto chains = hld.getPathtoAncestor(v, 0);
		for(auto chain : chains) {
			weights[chain.second - 1] = std::min(weights[chain.second - 1], hld.h[v] - 2 * h);
			tree2.upd(chain.second - 1, weights[chain.second - 1]);
			h -= chain.second - chain.first;
		}
		assert(h == -1);
	};
	auto qry = [&](int v) {
		int ans = (int) 1e9;
		int h = hld.h[v];
		auto chains = hld.getPathtoAncestor(v, 0);
		for(auto chain : chains) {
			// lca is where it enters in the chain
			int enter = hld.rin[chain.second - 1];
			ans = std::min(ans, tree1.qry(hld.in[enter], hld.out[enter]).ans + hld.h[v] - 2 * h);
			// lca is somewhere else in the chain
			ans = std::min(ans, tree2.qry(chain.first, chain.second).ans + hld.h[v]);
			h -= chain.second - chain.first;
		}
		assert(h == -1);
		return ans;
	};
	upd(0);
	while(m--) {
		int t, v;
		std::cin >> t >> v;
		v--;
		if(t == 1) {
			upd(v);
		} else {
			std::cout << qry(v) << '\n';
		}
	}
}
