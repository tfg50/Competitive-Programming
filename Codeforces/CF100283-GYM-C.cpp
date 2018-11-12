#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <limits>
#include <cstdio>
#include <cstring>

std::mt19937 rng((int) std::chrono::steady_clock::now().time_since_epoch().count());

const int ms = 210;
const long long MOD = 1e9 + 7;

int got[ms][ms];
int sum[ms][ms];

void add(int &a, int b) { a = (a + b) % MOD; }
void sub(int &a, int b) { a = (a - b + MOD) % MOD; }

/*
Problem: propagate cells into manhatan distance diamonds

rotate and you're done
*/

int main() {
	std::ios_base::sync_with_stdio(false); std::cin.tie(NULL);
	freopen("treasures.in", "r", stdin);
	int t;
	std::cin >> t;
	for(int tt = 1; tt <= t; tt++) {
		int n, m, s, px, py;
		std::cin >> n >> m >> s >> px >> py;
		memset(got, 0, sizeof got);
		got[px][py] = 1;
		while(s > 1) {
			s--;
			memset(sum, 0, sizeof sum);
			for(int i = 1; i <= n; i++) {
				for(int j = 1; j <= m; j++) {
					int x = i + j, y = i - j + m;
					int x1 = x - s, x2 = x + s + 1;
					int y1 = y - s, y2 = y + s + 1;
					x2 = std::min(x2, ms - 1);
					y2 = std::min(y2, ms - 1);
					x1 = std::max(x1, 1);
					y1 = std::max(y1, 1);
					add(sum[x1][y1], got[i][j]);
					sub(sum[x2][y1], got[i][j]);
					sub(sum[x1][y2], got[i][j]);
					add(sum[x2][y2], got[i][j]);
				}
			}
			for(int i = 1; i < ms; i++) {
				for(int j = 1; j < ms; j++) {
					sum[i][j] = ((long long) sum[i][j] + sum[i-1][j] + sum[i][j-1] - sum[i-1][j-1] + MOD) % MOD;
				}
			}
			for(int i = 1; i <= n; i++) {
				for(int j = 1; j <= m; j++) {
					int x = i + j, y = i - j + m;
					got[i][j] = sum[x][y];
					//std::cout << got[i][j] << (j == m ? '\n' : ' ');
				}
			}
		}
		long long ans = 0;
		for(int i = 1; i <= n; i++) {
			for(int j = 1; j <= m; j++) {
				int c;
				std::cin >> c;
				ans = (ans + (long long) c * got[i][j]) % MOD;
			}
		}
		std::cout << "Case " << tt << ": " << ans << '\n';
	}
}
