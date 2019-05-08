const int MOD = 998244353;
const int me = 15;
const int ms = 1 << me;
ll fexp(ll x, ll e, ll mod = MOD) {
	ll ans = 1;
	x %= mod;
	for(; e > 0; e /= 2) {
		if(e & 1) {
			ans = ans * x % mod;
		}
		x = x * x % mod;
	}
	return ans;
}
//is n primitive root of p ?
bool test(ll x, ll p) {
	ll m = p - 1;
	for(int i = 2; i * i <= m; ++i) if(!(m % i)) {
		if(fexp(x, i, p) == 1) return false;
		if(fexp(x, m / i, p) == 1) return false;
	}
	return true;
}
//find the largest primitive root for p
int search(int p) {
	for(int i = p - 1; i >= 2; --i) if(test(i, p)) return i;
	return -1;
}
map<int, int> roots;
int get_root(int p) {
	if(roots[p]) {
		return roots[p];
	} else {
		roots[p]=search(p);
		return roots[p];
	}
}

#define add(x, y) x+y>=MOD?x+y-MOD:x+y

const int gen = search(MOD);
int bits[ms], root[ms];

void initFFT() {
	root[1] = 1;
	for(int len = 2; len < ms; len += len) {
		int z = fexp(gen, (MOD - 1) / len / 2);
		for(int i = len / 2; i < len; i++) {
			root[2 * i] = root[i];
			root[2 * i + 1] = (long long) root[i] * z % MOD;
		}
	}
}

void pre(int n) {
	int LOG = 0;
	while(1 << (LOG + 1) < n) {
		LOG++;
	}
	for(int i = 1; i < n; i++) {
		bits[i] = (bits[i >> 1] >> 1) | ((i & 1) << LOG);
	}
}

vector<int> fft(vector<int> a, int mod, bool inv = false) {
	int n = (int) a.size();
	pre(n);
	if(inv) {
		reverse(a.begin() + 1, a.end());
	}
	for(int i = 0; i < n; i++) {
		int to = bits[i];
		if(i < to) 
			swap(a[i], a[to]);
	}
	for(int len = 1; len < n; len *= 2) {
		for(int i = 0; i < n; i += len * 2) {
			for(int j = 0; j < len; j++) {
				int u = a[i + j], v = (ll) a[i + j + len] * root[len + j] % mod;
				a[i + j] = add(u, v);
				a[i + j + len] = add(u, mod - v);
			}
		}
	}
	if(inv) {
		int rev = fexp(n, mod-2, mod);
		for(int i = 0; i < n; i++)
			a[i] = (ll) a[i] * rev % mod;
	}
	return a;
}
