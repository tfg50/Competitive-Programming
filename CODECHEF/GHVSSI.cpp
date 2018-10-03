#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <cstring>
#include <cassert>

std::mt19937 rng((int) std::chrono::steady_clock::now().time_since_epoch().count());

const int ms = 303;

char memo[ms][ms][ms][4];

int dp(int f1, int f2, int f3, int mod) {
	if(mod == 2 || f1 + f2 + f3 == 0) {
		return 0;
	}
	char &ans = memo[f1][f2][f3][mod];
	if(ans != -1) {
		return (int) ans;
	}
	ans = 0;
	bool arr[4];
	memset(arr, 0, sizeof arr);
	if(f1 && (mod+1) % 4 != 2) {
		arr[dp(f1-1, f2, f3, (mod+1)%4)] = true;
	}
	if(f2 && mod != 0) {
		arr[dp(f1, f2-1, f3, mod^2)] = true;
	}
	if(f3 && (mod+3) % 4 != 2) {
		arr[dp(f1, f2, f3-1, (mod+3)%4)] = true;
	}
	while(arr[(int)ans]) {
		ans++;
	}
	return (int) ans;
}

int main() {
	// reading started in :50
	// thinking started in :52
	// coding started in :01
	std::ios_base::sync_with_stdio(false); std::cin.tie(NULL);
	memset(memo, -1, sizeof memo);
	int t;
	std::cin >> t;
	while(t--) {
		int n;
		std::cin >> n;
		int freq[4] = {0, 0, 0, 0};
		for(int i = 0; i < n; i++) {
			int v;
			std::cin >> v;
			freq[v % 4]++;
		}
		int ans = dp(freq[1], freq[2], freq[3], 0) ^ (freq[0] % 2);
		//std::cout << "freq is (" << freq[0] << ", " << freq[1] << ", " << freq[2] << ", " << freq[3] << ")\n";
		std::cout << (ans ? "Ghayeeth\n" : "Siroj\n");
	}
}
