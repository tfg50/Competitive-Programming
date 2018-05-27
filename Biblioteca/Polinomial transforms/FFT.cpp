#include <vector>
#include <complex>

typedef double ld;

const ld PI = acosl(-1.0);

typedef std::complex<ld> Complex;
typedef std::vector<Complex> CVector;

int bits[1 << 23];

void pre(int n) {
	int LOG = 0;
	while(1 << (LOG + 1) < n) {
		LOG++;
	}
	for(int i = 1; i < n; i++) {
		bits[i] = (bits[i >> 1] >> 1) | ((i & 1) << LOG);
	}
}

CVector fft(CVector a, bool inv = false) {
	int n = a.size();
	pre(n);
	for(int i = 0; i < n; i++) {
		int to = bits[i];
		if(to > i) {
			std::swap(a[to], a[i]);
		}
	}
	
	double angle = inv ? -PI : PI;
	for(int len = 1; len < n; len *= 2) {
		Complex delta(cosl(angle / len), sinl(angle / len));
		for(int i = 0; i < n; i += 2 * len) {
			Complex cur_root = 1;
			for(int j = 0; j < len; j++) {
				Complex u = a[i + j], v = a[i + j + len] * cur_root;
				a[i + j] = u + v;
				a[i + j + len] = u - v;
				cur_root *= delta;
			}
		}
	}
	if(inv) {
		for(int i = 0; i < n; i++)
			a[i] /= n;
	}
	return a;
}
