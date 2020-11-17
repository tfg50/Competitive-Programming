std::vector<int> topologicalOrder(const std::vector<std::vector<int>> &graph) {
	int n = (int) graph.size();
	std::vector<int> deg(n, 0);
	for(int i = 0; i < n; i++) {
		for(auto j : graph[i]) {
			deg[j]++;
		}
	}
	std::vector<int> order;
	for(int i = 0; i < n; i++) {
		if(deg[i] == 0) {
			order.push_back(i);
		}
	}
	for(int i = 0; i < (int) order.size(); i++) {
		int on = order[i];
		for(auto to : graph[on]) {
			if(--deg[to] == 0) {
				order.push_back(to);
			}
		}
	}
	return order;
}