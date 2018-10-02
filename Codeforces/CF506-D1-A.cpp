#include <iostream>
#include <vector>
#include <chrono>
#include <random>

std::mt19937 rng((int) std::chrono::steady_clock::now().time_since_epoch().count());

const int ms = 30300;
const int bs = 302;

int dp[ms][bs + bs];
int val[ms];

int main() {
	std::ios_base::sync_with_stdio(false); std::cin.tie(NULL);
	int n, d;
	std::cin >> n >> d;
	for(int i = 0; i < ms; i++) {
		for(int j = 0; j < bs + bs; j++) {
			dp[i][j] = -1e9;
		}
	}
	for(int i = 0; i < n; i++) {
		int pos;
		std::cin >> pos;
		val[pos]++;
	}
	dp[d][bs] = 0;
	int ans = 0;
	for(int i = 0; i < ms; i++) {
		for(int j = 0; j < bs + bs; j++) {
			if(dp[i][j] < 0) {
				continue;
			}
			dp[i][j] += val[i];
			ans = std::max(ans, dp[i][j]);
			int jmp = d + j - bs;
			for(int k = -1; k <= 1 && i + jmp + k < ms; k++) {
				if(jmp + k <= 0) continue;
				dp[i+jmp+k][j+k] = std::max(dp[i+jmp+k][j+k], dp[i][j]);
			}
		}
	}
	std::cout << ans << std::endl;
}
