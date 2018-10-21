const int MOD = 998244353;
const int me = 15;
const int ms = 1 << me;
//const int MOD = 754974721;

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
	for(int i = 2; i * i <= m; ++i) if(m % i == 0) {
		if(fexp(x, i, p) == 1) return false;
		if(fexp(x, m / i, p) == 1) return false;
	}
	return true;
}

//find the largest primitive root for p
ll search(ll p) {
	for(ll i = p - 1; i >= 2; --i) if(test(i, p)) return i;
	return -1;
}

#define add(x, y, mod) (x+y>=mod?x+y-mod:x+y)

const int gen = search(MOD);
int bits[ms], r[ms + 1];

void pre(int n) {
	int LOG = 0;
	while(1 << (LOG + 1) < n) {
		LOG++;
	}
	for(int i = 1; i < n; i++) {
		bits[i] = (bits[i >> 1] >> 1) | ((i & 1) << LOG);
	}
}

void pre(int n, int root, int mod) {
	pre(n);
	r[0] = 1;
	for(int i = 1; i <= n; i++) {
		r[i] = (ll) r[i - 1] * root % mod;
	}
}

std::vector<int> fft(std::vector<int> a, int mod, bool inv = false) {
	int root = gen;
	if(inv) {
		root = fexp(root, mod - 2, mod);
	}
	int n = a.size();
	root = fexp(root, (mod - 1) / n, mod);
	pre(n, root, mod);
	for(int i = 0; i < n; i++) {
		int to = bits[i];
		if(i < to) {
			std::swap(a[i], a[to]);
		}
	}
	for(int len = 1; len < n; len *= 2) {
		for(int i = 0; i < n; i += len * 2) {
			int cur_root = 0;
			int delta = n / (2 * len);
			for(int j = 0; j < len; j++) {
				int u = a[i + j], v = (ll) a[i + j + len] * r[cur_root] % mod;
				a[i + j] = add(u, v, mod);
				a[i + j + len] = add(u, mod - v, mod);
				cur_root += delta;
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
