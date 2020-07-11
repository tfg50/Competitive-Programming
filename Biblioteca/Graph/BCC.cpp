struct BCC {
public:
	BCC(int _n) : n(_n) {
		edges.resize(n);
		low.assign(n, -1);
		cur.assign(n, -1);
		down.assign(n, -1);
		group.assign(n, -1);
	}

	void addEdge(int u, int v) {
		edges[u].push_back(v);
		edges[v].push_back(u);
	}

	void init() {
		for(int i = 0, c = 0; i < n; i++) {
			if(low[i] == -1) {
				dfs(i, c, -1);
			}
		}
	}

	// returns -1 if (u, v) edge is bridge or some number if edge is in a cycle
	int getGroup(int u, int v) {
		if(cur[u] < cur[v]) std::swap(u, v);
		return group[u] == group[v] || low[u] == cur[v] ? group[u] : -1;
	}
	// returns is u is articulation point
	bool isArt(int u) { return down[cur[u]] == -1; }
private:
	int n;
	std::vector<std::vector<int>> edges;
	std::vector<int> low, cur, down, group;

	void dfs(int on, int &c, int par) {
		cur[on] = c;
		low[on] = c++;
		int children = 0;
		bool root = par == -1;
		bool isArt = false;
		for(auto to : edges[on]) {
			if(to == par) {
				par = -1;
			} else if(low[to] == -1) {
				children++;
				down[cur[on]] = to;
				dfs(to, c, on);
				low[on] = std::min(low[on], low[to]);
				if((root && children > 1) || (!root && low[to] >= cur[on])) {
					isArt = true;
				}
			} else {
				low[on] = std::min(low[on], cur[to]);
			}
		}
		group[on] = down[low[on]];
		down[cur[on]] = isArt ? -1 : 0;
	}
};