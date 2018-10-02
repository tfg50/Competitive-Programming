#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <map>
#include <cstring>
#include <algorithm>

std::mt19937 rng((int) std::chrono::steady_clock::now().time_since_epoch().count());

const int ms = 100100;
const int bs = 250;

int largeSize = 0, large[ms / bs + 2][ms];
int c = 0, to[2 * ms], prv[2 * ms];
int adj[ms];
bool visit[ms];

void addEdge(int u, int v) {
	prv[c] = adj[u];
	to[c] = v;
	adj[u] = c++;
	std::swap(u, v);
	prv[c] = adj[u];
	to[c] = v;
	adj[u] = c++;
}

std::vector<std::pair<int, int>> ori[ms];

int main() {
	std::ios_base::sync_with_stdio(false); std::cin.tie(NULL);
	int n, m;
	std::cin >> n >> m;
	for(int i = 0; i < m; i++) {
		int u, v, col;
		std::cin >> u >> v >> col;
		u--;v--;col--;
		ori[col].emplace_back(u, v);
	}
	std::vector<std::vector<int>> freq(n);
	memset(adj, -1, sizeof adj);
	for(int i = 0; i < m; i++) {
		if(ori[i].size() <= bs) {
			// do pairs
			for(auto e : ori[i]) {
				addEdge(e.first, e.second);
			}
			for(auto e : ori[i]) {
				if(!visit[e.first]) {
					std::vector<int> v;
					v.push_back(e.first);
					visit[e.first] = true;
					for(int id = 0; id < (int) v.size(); id++) {
						int cur = v[id];
						for(int ed = adj[cur]; ed != -1; ed = prv[ed]) {
							if(!visit[to[ed]]) {
								visit[to[ed]] = true;
								v.push_back(to[ed]);
							}
						}
					}
					std::sort(v.begin(), v.end());
					for(int ii = 0; ii < (int) v.size(); ii++) {
						for(int j = ii + 1; j < (int) v.size(); j++) {
							freq[v[ii]].push_back(v[j]);
						}
					}
				}
			}
			for(auto e : ori[i]) {
				visit[e.first] = visit[e.second] = 0;
				adj[e.first] = adj[e.second] = -1;
			}
			c = 0;
		} else {
			// do large
			int cmp = 1;
			for(auto e : ori[i]) {
				addEdge(e.first, e.second);
			}
			for(auto e : ori[i]) {
				if(!visit[e.first]) {
					std::vector<int> v;
					v.push_back(e.first);
					visit[e.first] = true;
					for(int id = 0; id < (int) v.size(); id++) {
						int cur = v[id];
						for(int ed = adj[cur]; ed != -1; ed = prv[ed]) {
							if(!visit[to[ed]]) {
								visit[to[ed]] = true;
								v.push_back(to[ed]);
							}
						}
					}
					for(int ii = 0; ii < (int) v.size(); ii++) {
						large[largeSize][v[ii]] = cmp;
					}
					cmp++;
				}
			}
			for(auto e : ori[i]) {
				visit[e.first] = visit[e.second] = 0;
				adj[e.first] = adj[e.second] = -1;
			}
			c = 0;
			largeSize++;
		}
	}
	for(int i = 0; i < n; i++) {
		std::sort(freq[i].begin(), freq[i].end());
	}
	int q;
	std::cin >> q;
	while(q--) {
		int u, v;
		std::cin >> u >> v;
		if(u > v) std::swap(u, v);
		u--;v--;
		int ans = std::upper_bound(freq[u].begin(), freq[u].end(), v) - std::lower_bound(freq[u].begin(), freq[u].end(), v);
		for(int i = 0; i < largeSize; i++) {
			if(large[i][u] == large[i][v] && large[i][u] != 0) {
				ans++;
			}
		}
		std::cout << ans << std::endl;
	}
}