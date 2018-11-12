#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <limits>
#include <queue>

std::mt19937 rng((int) std::chrono::steady_clock::now().time_since_epoch().count());

/*
Problem: Given a connected graph with directed and undirected edges
find a direction such that there's an euler circuit in the graph.

The condition for finding this is making the in/out degree of
the vertices balanced or for every in edge, there's an out edge.
This creates a restriction that every vertex must have even edges.

Assuming that the undirected edge (u, v) goes from u to v,
some edges might need to be flipped. Vertex u has
balance[u] = in[u] - out[u]
if balance[u] > 0, balance[u] / 2 edges that go in will have to be flipped
else, -balance[u] / 2 edges that go out will have to be flipped.

Now, we create a network where
if balance[u] > 0, it needs to send balance[u] / 2 to sink
else, it receives -balance[u] / 2 from source
and the edges are oriented (u, v)

there exists a valid configuration iff we can saturate the source/sink
*/

template <class T = int>
class Dinic {
public:
	struct Edge {
		Edge(int a, T b){to = a;cap = b;}
		int to;
		T cap;
	};

	Dinic(int s) {
		this->n = s;
		edges.resize(n);
	}

	T maxFlow(int src, int sink) {
		T ans = 0;
		while(bfs(src, sink)) {
			T flow;
			pt = std::vector<int>(n, 0);
			while(flow = dfs(src, sink)) {
				ans += flow;
			}
		}
		return ans;
	}

	void addEdge(int from, int to, T cap = 1) {
		edges[from].push_back(list.size());
		list.push_back(Edge(to, cap));
		edges[to].push_back(list.size());
		list.push_back(Edge(from, 0));
	}
private:
	int n;
	std::vector<std::vector<int> > edges;
	std::vector<Edge> list;
	std::vector<int> h, pt;

	T dfs(int on, int sink, T flow = 1e9) {
		if(flow == 0) {
			return 0;
		} if(on == sink) {
			return flow;
		}
		for(; pt[on] < edges[on].size(); pt[on]++) {
			int cur = edges[on][pt[on]];
			if(h[on] + 1 != h[list[cur].to]) {
				continue;
			}
			T got = dfs(list[cur].to, sink, std::min(flow, list[cur].cap));
			if(got) {
				list[cur].cap -= got;
				list[cur ^ 1].cap += got;
				return got;
			}
		}
		return 0;
	}

	bool bfs(int src, int sink) {
		h = std::vector<int>(n, n);
		h[src] = 0;
		std::queue<int> q;
		q.push(src);
		while(!q.empty()) {
			int on = q.front();
			q.pop();
			for(auto a : edges[on]) {
				if(list[a].cap == 0) {
					continue;
				}
				int to = list[a].to;
				if(h[to] > h[on] + 1) {
					h[to] = h[on] + 1;
					q.push(to);
				}
			}
		}
		return h[sink] < n;
	}
};

bool solve() {
	int n, m;
	std::cin >> n >> m;
	//std::cout << "reading " << n << ", " << m << std::endl;
	std::vector<int> bal(n, 0);
	Dinic<int> graph(n + 2);
	int src = n, sink = n + 1;
	for(int i = 0; i < m; i++) {
		int u, v, c;
		std::cin >> u >> v >> c;
		//std::cout << "(" << u << ", " << v << ", " << c << ")\n";
		u--;v--;
		bal[v]++;
		bal[u]--;
		if(c == 0) {
			graph.addEdge(u, v, 1);
		}
	}
	bool valid = true;
	for(int i = 0; i < n; i++) {
		valid = valid && bal[i] % 2 == 0;
		//std::cout << "balance of " << i << " is " << bal[i] << std::endl;
	}
	if(!valid) return false;
	int tot = 0;
	for(int i = 0; i < n; i++) {
		if(bal[i] >= 0) {
			graph.addEdge(i, sink, bal[i] / 2);
			tot += bal[i] / 2;
		} else {
			graph.addEdge(src, i, -bal[i] / 2);
		}
	}
	//std::cout << "tot is " << tot << '\n';
	return graph.maxFlow(src, sink) == tot;
}

int main() {
	//std::ios_base::sync_with_stdio(false); std::cin.tie(NULL);
	int t;
	std::cin >> t;
	while(t--) {
		std::cout << (solve() ? "possible\n" : "impossible\n");
	}
}
