#include <iostream>
#include <vector>
#include <chrono>
#include <random>

std::mt19937 rng((int) std::chrono::steady_clock::now().time_since_epoch().count());

const int ms = 202;
const int MOD = 10007;

int dp[ms][ms][ms];

int fexp(int x, int e = MOD - 2) {
	x %= MOD;
	x += MOD;
	x %= MOD;
	int ans = 1;
	for(; e > 0; e /= 2) {
		if(e & 1) {
			ans = ans * x % MOD;
		}
		x = x * x % MOD;
	}
	return ans;
}

int pg(int x, int b, int n) {
	//std::cout << "pg returning " << (fexp(b, n + 1) - 1) * fexp(b - 1) % MOD * x % MOD << std::endl;
	return (fexp(b, n + 1) - 1) * fexp(b - 1) % MOD * x % MOD;
}

int main() {
	std::ios_base::sync_with_stdio(false); std::cin.tie(NULL);
	int k;
	std::string s;
	std::cin >> s >> k;
	int n = (int) s.size();
	dp[0][n-1][0] = 1;
	for(int l = 0; l < n; l++) {
		for(int r = n - 1; r > l; r--) {
			for(int i = 0; i <= n; i++) {
				if(s[l] == s[r]) {
					dp[l+1][r-1][i] = (dp[l+1][r-1][i] + dp[l][r][i]) % MOD;
				} else {
					dp[l+1][r][i+1] = (dp[l+1][r][i+1] + dp[l][r][i]) % MOD;
					dp[l][r-1][i+1] = (dp[l][r-1][i+1] + dp[l][r][i]) % MOD;
				}
			}
		}
	}
	int ans = 0;
	// solving for l == r
	int base = 26 * fexp(25) % MOD;
	for(int l = 0; l < n; l++) {
		for(int i = 0; i <= n; i++) {
			if(dp[l][l][i] == 0) continue;
			int newN = k - i;
			if(newN < 0) continue;
			int usedSpots = (n - 1 + i) / 2 + 1;
			int globalSpots = (k + n + 1) / 2;
			int restSpots = (globalSpots - usedSpots);
			restSpots = newN / 2;
			if(restSpots < 0) {
				continue;
			}
			//std::cout << "newN = " << newN << std::endl;
			// sum 26^i * 25^j, i + j == restSpots
			// or
			// sum 25^restSpots * (26/25)^i, i <= restSpots
			//std::cout << "solving for (" << l << ", " << l << ", " << i << "), rest is " << restSpots << "\n";
			//std::cout << "got here with " << dp[l][l][i] << "\n";
			ans = (ans + dp[l][l][i] * pg(fexp(25, restSpots), base, restSpots) % MOD) % MOD;
		}
	}
	// solving for l == r - 1
	for(int l = 1; l < n; l++) {
		for(int i = 0; i <= n; i++) {
			if(dp[l][l-1][i] == 0) continue;
			int newN = k - i;
			if(newN < 0) continue;
			int usedSpots = (n + i) / 2;
			int globalSpots = (k + n + 1) / 2;
			int restSpots = (globalSpots - usedSpots);
			restSpots = (newN + 1) / 2;
			if((k + n) % 2 == 1) {
				restSpots--;
				if(restSpots < 0) continue;
				ans = (ans + dp[l][l-1][i] * pg(fexp(25, restSpots) * 26 % MOD, base, restSpots) % MOD) % MOD;
				continue;
			}
			if(restSpots < 0) {
				continue;
			}
			// sum 26^i * 25^j, i + j == restSpots
			// or
			// sum 25^restSpots * (26/25)^i, i <= restSpots
			//std::cout << "solving for (" << l << ", " << l - 1 << ", " << i << "), rest is " << restSpots << "\n";
			//std::cout << "got here with " << dp[l][l-1][i] << "\n";
			ans = (ans + dp[l][l-1][i] * pg(fexp(25, restSpots), base, restSpots) % MOD) % MOD;
		}
	}
	std::cout << ans << std::endl;
}