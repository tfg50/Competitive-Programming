#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>

std::mt19937 rng((int) std::chrono::steady_clock::now().time_since_epoch().count());

const int me = 12;
const int ms = 1 << me;

long long freq[ms];
long long a[ms];
int k, n, m;

/*
https://csacademy.com/blog/#fast-fourier-transform-and-variations-of-it
This problem can be solved by a variation of these transforms
Each bit gets a different "operator" as in xor, and, or, nor and the problem is solved just like that
*/

long long solve(std::string gates) {
	int size = 1 << k;
	int A = 0;
	int O = 0;
	int x = 0;
	int inv = 0;
	for(int i = 0; i < k; i++) {
		if(gates[i] == 'A') {
			A |= 1 << i;
		} else if(gates[i] == 'a') {
			O |= 1 << i;
			inv |= 1 << i;
		} else if(gates[i] == 'O') {
			O |= 1 << i;
		} else if(gates[i] == 'o') {
			A |= 1 << i;
			inv |= 1 << i;
		} else if(gates[i] == 'x') {
			x |= 1 << i;
		}
	}
	for(int i = 0; i < size; i++) {
		a[i^inv] = freq[i];
	}
	for(int i = 0; i < size; i++) {
		if(O & i) {
			a[i] = 0;
		}
	}
	for(int e = 0; e < k; e++) {
		int len = 1 << e;
		if(A & (1 << e)) {
			// and
			for(int i = 0; i < size; i += len + len) {
				for(int j = 0; j < len; j++) {
					a[i + j] += a[i + j + len];
				}
			}
		} else {
			// xor
			for(int i = 0; i < size; i += len + len) {
				for(int j = 0; j < len; j++) {
					long long u = a[i + j], v = a[i + j + len];
					a[i + j] = u + v;
					a[i + j + len] = u - v;
				}
			}
		}
	}
	for(int i = 0; i < size; i++) {
		a[i] *= a[i];
	}
	long long fact = 1;
	for(int e = k - 1; e >= 0; e--) {
		int len = 1 << e;
		if(A & (1 << e)) {
			// and
			for(int i = 0; i < size; i += len + len) {
				for(int j = 0; j < len; j++) {
					a[i + j] -= a[i + j + len];
				}
			}
		} else {
			// xor
			fact *= 2;
			for(int i = 0; i < size; i += len + len) {
				for(int j = 0; j < len; j++) {
					long long u = a[i + j], v = a[i + j + len];
					a[i + j] = u + v;
					a[i + j + len] = u - v;
				}
			}
		}
	}
	return a[x] / fact;
}

int main() {
	std::ios_base::sync_with_stdio(false); std::cin.tie(NULL);
	std::cin >> k >> n >> m;
	for(int i = 0; i < n; i++) {
		int val;
		std::cin >> val;
		freq[val]++;
	}
	while(m--) {
		std::string str;
		std::cin >> str;
		std::reverse(str.begin(), str.end());
		std::cout << (int) solve(str) << '\n';
	}
}
