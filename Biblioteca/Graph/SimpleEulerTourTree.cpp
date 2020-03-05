template<const bool REPEAT = false>
struct SimpleEulerTourTree {
	void init(const std::vector<std::vector<int>> &edges, int root = 0) {
		a.clear();
		in = out = std::vector<int>(edges.size(), 0);
		dfs(root, root, edges, 0);
	}

	void dfs(int on, int par, const std::vector<std::vector<int>> &edges, int h) {
		in[on] = (int) a.size();
		a.emplace_back(h, on);
		for(auto to : edges[on]) {
			if(to == par) continue;
			dfs(to, on, edges, h+1);
			if(REPEAT) a.emplace_back(h, on);
		}
		out[on] = (int) a.size();
	}

	std::vector<int> in, out;
	std::vector<std::pair<int, int>> a;
};