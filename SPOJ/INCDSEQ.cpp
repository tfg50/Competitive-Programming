#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>

std::mt19937 rng((int) std::chrono::steady_clock::now().time_since_epoch().count());

const int ms = 10010;
const int mk = 55;
const int MOD = 5000000;

struct FenwickTree {
	int bit[ms];

	FenwickTree() {
		for(int i = 0; i < ms; i++) {
			bit[i] = 0;
		}
	}

	int qry(int x) {
		int ans = 0;
		for(; x > 0; x -= x & -x) {
			ans = (ans + bit[x]) % MOD;
		}
		return ans;
	}

	void upd(int x, int v) {
		v = (v % MOD + MOD) % MOD;
		for(; x < ms; x += x & -x) {
			bit[x] = (bit[x] + v) % MOD;
		}
	}
};

FenwickTree trees[mk];

int main() {
	std::ios_base::sync_with_stdio(false); std::cin.tie(NULL);
	int n, k;
	std::cin >> n >> k;
	std::vector<int> a(n);
	for(int i = 0; i < n; i++) {
		std::cin >> a[i];
	}
	std::vector<int> p = a;
	p.push_back(-1);
	std::sort(p.begin(), p.end());
	trees[0].upd(1, 1);
	for(int i = 0; i < n; i++) {
		int v = a[i];
		int idx = std::upper_bound(p.begin(), p.end(), v) - p.begin();
		for(int j = 0; j < k; j++) {
			trees[j+1].upd(idx, trees[j].qry(idx-1) - (trees[j+1].qry(idx) - trees[j+1].qry(idx - 1)));
		}
	}
	int ans = trees[k].qry(ms - 1);
	std::cout << ans << std::endl;
}
