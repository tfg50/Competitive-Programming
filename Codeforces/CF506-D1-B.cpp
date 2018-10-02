#include <iostream>
#include <vector>
#include <chrono>
#include <random>

std::mt19937 rng((int) std::chrono::steady_clock::now().time_since_epoch().count());

const int ms = 100100;

std::vector<int> edges1[ms], edges2[ms];
bool visit[ms];
int color[ms];

bool cycle(int on) {
	if(color[on] == 1) {
		return true;
	} else if(color[on] == 2) {
		return false;
	}
	// assert(color[on] == 0);
	color[on] = 1;
	for(auto to : edges2[on]) {
		if(cycle(to)) {
			color[on] = 2;
			return true;
		}
	}
	color[on] = 2;
	return false;
}

bool dfs(int on) {
	visit[on] = true;
	bool ans = cycle(on);
	for(auto to : edges1[on]) {
		if(!visit[to]) {
			if(dfs(to)) {
				ans = true;
			}
		}
	}
	return ans;
}


int main() {
	std::ios_base::sync_with_stdio(false); std::cin.tie(NULL);
	int n, m;
	std::cin >> n >> m;
	std::vector<std::pair<int, int>> ori(m);
	for(int i = 0; i < m; i++) {
		std::cin >> ori[i].first >> ori[i].second;
		ori[i].first--;
		ori[i].second--;
	}
	int ans = 0;
	for(int i = 0; i < m; i++) {
		edges1[ori[i].first].push_back(ori[i].second);
		edges1[ori[i].second].push_back(ori[i].first);
		edges2[ori[i].first].push_back(ori[i].second);
	}
	ans += n;
	for(int i = 0; i < n; i++) {
		if(!visit[i]) {
			ans--;
			//std::cout << "component starting from " << i << std::endl;
			if(dfs(i)) {
				//std::cout << "found cycle\n";
				ans++;
			}
		}
	}
	std::cout << ans << std::endl;
}