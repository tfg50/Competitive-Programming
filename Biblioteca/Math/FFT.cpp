const ld PI = acosl(-1.0);

struct Complex {
	ld real, imag;

	Complex(ld a = 0, ld b = 0) : real(a), imag(b) {}
	Complex operator + (const Complex &o) const { return Complex(real + o.real, imag + o.imag); }
	Complex operator - (const Complex &o) const { return Complex(real - o.real, imag - o.imag); }
	Complex operator * (const Complex &o) const { return Complex(real * o.real - imag * o.imag, real * o.imag + imag * o.real); }
	Complex operator / (ld o) const { return Complex(real / o, imag / o); }
	void operator *= (Complex o) { *this = *this * o; }
	void operator /= (ld o) { real /= o, imag /= o; }
};

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

void fft2in1(CVector &a, CVector &b) {
	int n = (int) a.size();
	for(int i = 0; i < n; i++) {
		a[i] = Complex(a[i].real, b[i].real);
	}
	auto c = fft(a);
	for(int i = 0; i < n; i++) {
		a[i] = (c[i] + c[(n-i) % n].conj()) * Complex(0.5, 0);
		b[i] = (c[i] - c[(n-i) % n].conj()) * Complex(0, -0.5);
	}
}

void ifft2in1(CVector &a, CVector &b) {
	int n = (int) a.size();
	for(int i = 0; i < n; i++) {
		a[i] = a[i] + b[i] * Complex(0, 1);
	}
	a = fft(a, true);
	for(int i = 0; i < n; i++) {
		b[i] = Complex(a[i].imag, 0);
		a[i] = Complex(a[i].real, 0);
	}
}

std::vector<long long> mul(const std::vector<long long> &a, const std::vector<long long> &b, long long cut = 1 << 16) {
	int n = (int) a.size();
	std::vector<CVector> C(4, CVector(n, 0));
	for(int i = 0; i < n; i++) {
		C[0][i] = a[i] % cut;
		C[1][i] = a[i] / cut;
		C[2][i] = b[i] % cut;
		C[3][i] = b[i] / cut;
	}
	fft2in1(C[0], C[1]);
	fft2in1(C[2], C[3]);
	for(int i = 0; i < n; i++) {
		// 00, 01, 10, 11
		Complex cur[4];
		for(int j = 0; j < 4; j++) cur[j] = C[j/2+2][i] * C[j % 2][i];
		for(int j = 0; j < 4; j++) C[j][i] = cur[j];
	}
	ifft2in1(C[0], C[1]);
	ifft2in1(C[2], C[3]);
	std::vector<long long> ans(n, 0);
	for(int i = 0; i < n; i++) {
		// if there are negative values, care with rounding
		ans[i] += (long long) (C[0][i].real + 0.5);
		ans[i] += (long long) (C[1][i].real + C[2][i].real + 0.5) * cut;
		ans[i] += (long long) (C[3][i].real + 0.5) * cut * cut;
	}
	return ans;
}
