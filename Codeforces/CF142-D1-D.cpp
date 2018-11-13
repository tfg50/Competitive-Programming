#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <limits>

/*
I read the editorial months ago, had to reread to get this solution.
Tons of tricky cases.
Need to check the editorial in more details.
*/

std::mt19937 rng((int) std::chrono::steady_clock::now().time_since_epoch().count());

int main() {
	std::ios_base::sync_with_stdio(false); std::cin.tie(NULL);
	int n, m, k;
	std::cin >> n >> m >> k;
	int msk = 0;
	std::vector<int> got(20, 0);
	while(n--) {
		std::string str;
		std::cin >> str;
		std::vector<int> pos;
		for(int i = 0; i < m; i++) {
			if(str[i] != '-') {
				pos.push_back(i);
			}
		}
		if(pos.size() == 1) {
			msk |= str[pos[0]] == 'R' ? 1 : 2;
		} else if(pos.size() == 2 && str[pos[0]] != str[pos[1]]) {
			int size = pos[1] - pos[0] - 1;
			int bit = 0;
			while(size > 0) {
				got[bit++] += size & 1;
				size /= 2;
			}
		} else if(pos.size() == 2 && m > 2) {
			msk |= str[pos[0]] == 'R' ? 1 : 2;
		}
	}
	bool win = false;
	for(auto v : got) {
		win = win || v % (k + 1) != 0;
	}
	if(m == 1) {
		msk = 1;
	}
	if(msk == 3) {
		std::cout << "Draw\n";
	} else if(msk == 1) {
		std::cout << "Second\n";
	} else if(msk == 2) {
		std::cout << "First\n";
	} else if(win) {
		std::cout << "First\n";
	} else {
		std::cout << "Second\n";
	}
}
