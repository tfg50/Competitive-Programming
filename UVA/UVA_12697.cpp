#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <utility>
#include <set>
#include <algorithm>

/*
UVA 12697
Find the smallest subarray with sum <= X
sum[l+1...r] = sum[r] - sum[l]
sum[r] - sum[l] <= X
sum[l] <= sum[r] - X
also, l < r
We can sort pairs of (sum[i], i) and do a two-pointer algorithm over it
Insert points l that sum[l] - X >= sum[r] in a set
and use that set to find the closest possible L
*/

std::mt19937 rng((int) std::chrono::steady_clock::now().time_since_epoch().count());

int main() {
	std::ios_base::sync_with_stdio(false); std::cin.tie(NULL);
	int t;
	std::cin >> t;
	while(t--) {
		int n, x;
		std::cin >> n >> x;
		std::vector<std::pair<long long, int>> a(n + 1);
		std::set<int> got;
		a[0] = {0, 0};
		for(int i = 1; i <= n; i++) {
			std::cin >> a[i].first;
			a[i].first += a[i-1].first;
			a[i].second = i;
		}
		std::sort(a.begin(), a.end());
		int ans = n + 1;
		for(int l = 0, r = 0; r <= n; r++) {
			while(l <= n && a[l].first <= a[r].first - x) {
				got.insert(a[l].second);
				l++;
			}
			auto it = got.lower_bound(a[r].second);
			if(it != got.begin()) {
				it--;
				ans = std::min(ans, a[r].second - *it);
			}
		}
		if(ans > n) ans = -1;
		std::cout << ans << std::endl;
	}
}
