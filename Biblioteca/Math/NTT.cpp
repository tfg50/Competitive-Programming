// copy fexp from PrimitiveRoot.cpp

const int MOD = 998244353;
const int me = 15;
const int ms = 1 << me;

#define add(x, y) x+y>=MOD?x+y-MOD:x+y

const int gen = 3; // use search() from PrimitiveRoot.cpp if MOD isn't 998244353
int bits[ms], root[ms];

void initFFT() {
	root[1] = 1;
	for(int len = 2; len < ms; len += len) {
		int z = (int) fexp(gen, (MOD - 1) / len / 2);
		for(int i = len / 2; i < len; i++) {
			root[2 * i] = root[i];
			root[2 * i + 1] = (int)((long long) root[i] * z % MOD);
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

std::vector<int> fft(std::vector<int> a, bool inv = false) {
	int n = (int) a.size();
	pre(n);
	if(inv) {
		std::reverse(a.begin() + 1, a.end());
	}
	for(int i = 0; i < n; i++) {
		int to = bits[i];
		if(i < to) { std::swap(a[i], a[to]); }
	}
	for(int len = 1; len < n; len *= 2) {
		for(int i = 0; i < n; i += len * 2) {
			for(int j = 0; j < len; j++) {
				int u = a[i + j], v = (int)((long long) a[i + j + len] * root[len + j] % MOD);
				a[i + j] = add(u, v);
				a[i + j + len] = add(u, MOD - v);
			}
		}
	}
	if(inv) {
		long long rev = fexp(n, MOD-2, MOD);
		for(int i = 0; i < n; i++)
			a[i] = (int)(a[i] * rev % MOD);
	}
	return a;
}

std::vector<int> shift(const std::vector<int> &a, int s) {
	int n = std::max(0, s + (int) a.size());
	std::vector<int> b(n, 0);
	for(int i = std::max(-s, 0); i < (int) a.size(); i++) {
		b[i + s] = a[i];
	}
	return b;
}

std::vector<int> cut(const std::vector<int> &a, int n) {
	std::vector<int> b(n, 0);
	for(int i = 0; i < (int) a.size() && i < n; i++) {
		b[i] = a[i];
	}
	return b;
}

std::vector<int> operator +(std::vector<int> a, const std::vector<int> &b) {
	int sz = (int) std::max(a.size(), b.size());
	a.resize(sz, 0);
	for(int i = 0; i < (int) b.size(); i++) {
		a[i] = add(a[i], b[i]);
	}
	return a;
}

std::vector<int> operator -(std::vector<int> a, const std::vector<int> &b) {
	int sz = (int) std::max(a.size(), b.size());
	a.resize(sz, 0);
	for(int i = 0; i < (int) b.size(); i++) {
		a[i] = add(a[i], MOD - b[i]);
	}
	return a;
}

std::vector<int> operator *(std::vector<int> a, std::vector<int> b) {
	while(!a.empty() && a.back() == 0) a.pop_back();
	while(!b.empty() && b.back() == 0) b.pop_back();
	if(a.empty() || b.empty()) return std::vector<int>(0, 0);
	int n = 1;
	while(n-1 < (int) a.size() + (int) b.size() - 2) n += n;
	a.resize(n, 0);
	b.resize(n, 0);
	a = fft(a, false);
	b = fft(b, false);
	for(int i = 0; i < n; i++) {
		a[i] = (int) ((long long) a[i] * b[i] % MOD); 
	}
	return fft(a, true);
}

std::vector<int> inverse(const std::vector<int> &a, int k) {
	assert(!a.empty() && a[0] != 0);
	if(k == 0) {
		return std::vector<int>(1, (int) fexp(a[0], MOD - 2));
	} else {
		int n = 1 << k;
		auto c = inverse(a, k-1);
		return cut(c * cut(std::vector<int>(1, 2) - cut(a, n) * c, n), n);
	}
}

std::vector<int> operator /(std::vector<int> a, std::vector<int> b) {
	// NEED TO TEST!
	while(!a.empty() && a.back() == 0) a.pop_back();
	while(!b.empty() && b.back() == 0) b.pop_back();
	assert(!b.empty());
	if(a.size() < b.size()) return std::vector<int>(1, 0);
	std::reverse(a.begin(), a.end());
	std::reverse(b.begin(), b.end());
	int n = (int) a.size() - (int) b.size() + 1;
	int k = 0;
	while((1 << k) - 1 < n) k++;
	a = cut(a * inverse(b, k), (int) a.size() - (int) b.size() + 1);
	std::reverse(a.begin(), a.end());
	return a;
}

std::vector<int> log(const std::vector<int> &a, int k) {
	assert(!a.empty() && a[0] != 0);
	int n = 1 << k;
	std::vector<int> b(n, 0);
	for(int i = 0; i+1 < (int) a.size() && i < n; i++) {
		b[i] = (int)((i + 1LL) * a[i+1] % MOD);
	}
	b = cut(b * inverse(a, k), n);
	assert((int) b.size() == n);
	for(int i = n - 1; i > 0; i--) {
		b[i] = (int) (b[i-1] * fexp(i, MOD - 2) % MOD);
	}
	b[0] = 0;
	return b;
}

std::vector<int> exp(const std::vector<int> &a, int k) {
	assert(!a.empty() && a[0] == 0);
	if(k == 0) {
		return std::vector<int>(1, 1);
	} else {
		auto b = exp(a, k-1);
		int n = 1 << k;
		return cut(b * cut(std::vector<int>(1, 1) + cut(a, n) - log(b, k), n), n);
	}
}