#include <iostream>
#include <vector>
#include <chrono>
#include <random>

std::mt19937 rng((int) std::chrono::steady_clock::now().time_since_epoch().count());

int mod;

struct modBase {
	modBase(int val = 0) : val(val) {}
	int val;

	modBase operator * (modBase o) { return (long long) val * o.val % mod; }
	modBase operator + (modBase o) { return val + o.val > mod ? val + o.val - mod : val + o.val; }
};

template<const size_t n, const size_t m, class T = modBase>
struct Mat {
	T v[n][m];
	
	Mat(int d = 0) {
		for(int i = 0; i < n; i++) {
			for(int j = 0; j < m; j++) {
				v[i][j] = T(0);
			}
			if(i < m) {
				v[i][i] = T(d);
			}
		}
	}
	
	template<size_t mm>
	Mat<n, mm, T> operator *(Mat<m, mm, T> &o) {
		Mat<n, mm, T> ans;
		for(int i = 0; i < n; i++) {
			for(int j = 0; j < mm; j++) {
				for(int k = 0; k < m; k++) {
					ans.v[i][j] = ans.v[i][j] + v[i][k] * o.v[k][j];
				}
			}
		}
		return ans;
	}
};

template<class T>
T fexp(T x, int e) {
	T ans(1);
	for(; e > 0; e /= 2) {
		if(e & 1) ans = ans * x;
		x = x * x;
	}
	return ans;
}

class TourCounting {
public:
	int countTours(std::vector<std::string> adj, int k, int m) {
		mod = m;
		//if(m == 1 || k <= 2) return 0;
		Mat<36, 36> base(0);
		int n = (int) adj.size();
		for(int i = 0; i < n; i++) {
			for(int j = 0; j < n; j++) {
				if(adj[i][j] == 'Y') {
					base.v[j][i] = 1;
				}
			}
		}
        base.v[n][n] = 1;
		modBase ans(0);
        k--;
		for(int i = 0; i < n; i++) {
			Mat<36, 36> cur = base;
			for(int j = 0; j < n; j++) {
				if(adj[j][i] == 'Y') {
					cur.v[n][j] = 1;
				}
			}
			cur = fexp<>(cur, k);
			for(int ii = 0; ii <= n; ii++) {
				for(int jj = 0; jj <= n; jj++) {
					//std::cout << cur.v[ii][jj].val << (jj == n ? '\n' : ' ');
				}
			}
            //std::cout << cur.v[n][i].val << '\n';
			ans = ans + cur.v[n][i];
		}
		return ans.val;
	}
};
