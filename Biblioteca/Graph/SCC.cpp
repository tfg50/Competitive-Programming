struct SCC {
	std::vector<std::vector<int>> edges;
	std::vector<int> comp, st, idx;
	int c;

	void init(int n) {
		edges.resize(n);
		comp.assign(n, -1);
		idx.assign(n, -2);
		st.clear();
	}

	void addEdge(int u, int v) {
		edges[u].push_back(v);
	}

	int dfs(int on) {
		if(idx[on] != -2) {
			return idx[on] == -1 ? c : comp[on];
		}
		idx[on] = comp[on] = c++;
		st.push_back(on);
		for(auto to : edges[on]) {
			comp[on] = std::min(comp[on], dfs(to));
		}
		if(idx[on] == comp[on]) {
			while(1) {
				int v = st.back();
				st.pop_back();
				idx[v] = -1;
				comp[v] = comp[on];
				if(v == on) {
					break;
				}
			}
		}
		return comp[on];
	}

	void tarjan() {
		c = 0;
		for(int i = 0; i < edges.size(); i++) {
			dfs(i);
		}
	}
};
