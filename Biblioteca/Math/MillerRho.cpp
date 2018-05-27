#include <cstdlib>
#include <ctime>
#include <vector>
#include <iostream>

//miller_rabin
typedef unsigned long long ull;
typedef long double ld;

ull fmul(ull a, ull b, ull m) {
	ull q = (ld) a * (ld) b / (ld) m;
	ull r = a * b - q * m;
	return (r + m) % m;
}

ull fexp(ull x, ull e, ull m) {
	ull ans = 1;
	x = x % m;
	for(; e; e >>= 1) {
		if(e & 1) {
			ans = fmul(ans, x, m);
		}
		x = fmul(x, x, m);
	}
	return ans;
}

bool miller(ull p, ull a) {
	ull s = p - 1;
	while(s % 2 == 0) s >>= 1;
	while(a >= p) a >>= 1;
	ull mod = fexp(a, s, p);
	while(s != p - 1 && mod != 1 && mod != p - 1) {
		mod = fmul(mod, mod, p);
		s <<= 1;
	}
	if(mod != p - 1 && s % 2 == 0)return false;
	else return true;
}

bool prime(ull p) {
	if(p <= 3)
		return true;
	if(p % 2 == 0)
		return false;
	return miller(p, 2) && miller(p, 3)
		&& miller(p, 5) && miller(p, 7)
		&& miller(p, 11) && miller(p, 13)
		&& miller(p, 17) && miller(p, 19)
		&& miller(p, 23) && miller(p, 29)
		&& miller(p, 31) && miller(p, 37);
}

//pollard_rho

ull func(ull x, ull c, ull n) {
	return (fmul(x, x, n) + c) % n;
}

ull gcd(ull a, ull b) {
	if(!b) return a;
	else return gcd(b, a % b);
}

ull rho(ull n) {
	if(n % 2 == 0) return 2;
	if(prime(n)) return n;
	while(1) {
		ull c;
		do {
			c = rand() % n;
		} while(c == 0 || (c + 2) % n == 0);
		ull x = 2, y = 2, d = 1;
		ull pot = 1, lam = 1;
		do {
			if(pot == lam) {
				x = y;
				pot <<= 1;
				lam = 0;
			}
			y = func(y, c, n);
			lam++;
			d = gcd(x >= y ? x - y : y - x, n);
		} while(d == 1);
		if(d != n) return d;
	}
}

std::vector<ull> factors(ull n) {
	std::vector<ull> ans, rest, times;
	if(n == 1) return ans;
	rest.push_back(n);
	times.push_back(1);
	while(!rest.empty()) {
		ull x = rho(rest.back());
		if(x == rest.back()) {
			int freq = 0;
			for(int i = 0; i < rest.size(); i++) {
				int cur_freq = 0;
				while(rest[i] % x == 0) {
					rest[i] /= x;
					cur_freq++;
				}
				freq += cur_freq * times[i];
				if(rest[i] == 1) {
					std::swap(rest[i], rest.back());
					std::swap(times[i], times.back());
					rest.pop_back();
					times.pop_back();
					i--;
				}
			}
			while(freq--) {
				ans.push_back(x);
			}
			continue;
		}
		ull e = 0;
		while(rest.back() % x == 0) {
			rest.back() /= x;
			e++;
		}
		e *= times.back();
		if(rest.back() == 1) {
			rest.pop_back();
			times.pop_back();
		}
		rest.push_back(x);
		times.push_back(e);
	}
	return ans;
}
