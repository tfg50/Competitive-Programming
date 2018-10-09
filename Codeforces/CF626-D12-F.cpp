#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>

std::mt19937 rng((int) std::chrono::steady_clock::now().time_since_epoch().count());

const int ms = 210;
const int mk = 1010;
const int MOD = 1e9 + 7;

/*
open-close dp
sort the numbers
open some intervals
close some intervals
the intervals existent in some transition will have a new max
dp[numbers considered][curent intervals][total sum]
*/

int dp[ms][ms][mk];

int main() {
	std::ios_base::sync_with_stdio(false); std::cin.tie(NULL);
	int n, m;
	std::cin >> n >> m;
	std::vector<int> a(n);
	for(int i = 0; i < n; i++) {
		std::cin >> a[i];
	}
	std::sort(a.begin(), a.end());
	a.push_back(a.back());
	dp[1][1][a[1] - a[0]] = 1;
	dp[1][0][0] = 1;
	for(int i = 1; i < n; i++) {
		// middle transition
		int nxt = a[i+1] - a[i];
		for(int j = 0; j <= i; j++) {
			for(int k = 0; k + j * nxt <= m; k++) {
				dp[i+1][j][k+j*nxt] = (dp[i+1][j][k+j*nxt] + (long long) dp[i][j][k] * (j + 1) % MOD) % MOD;
			}
		}
		// end transition
		for(int j = 1; j <= i; j++) {
			for(int k = 0; k + (j-1) * nxt <= m; k++) {
				dp[i+1][j-1][k+(j-1)*nxt] = (dp[i+1][j-1][k+(j-1)*nxt] + (long long) dp[i][j][k] * j % MOD) % MOD;
			}
		}
		// start transition
		for(int j = 0; j <= i; j++) {
			for(int k = 0; k + (j+1) * nxt <= m; k++) {
				dp[i+1][j+1][k+(j+1)*nxt] = (dp[i+1][j+1][k+(j+1)*nxt] + dp[i][j][k]) % MOD;
			}
		}
		// start and end transition is in middle transition
	}
	int ans = 0;
	for(int k = 0; k <= m; k++) {
		ans = (ans + dp[n][0][k]) % MOD;
	}
	std::cout << ans << std::endl;
}
