#include <vector>
#include <complex>

typedef double ld;
typedef std::complex<ld> Complex;
typedef std::vector<Complex> CVector;

const ld PI = acosl(-1);

CVector fft(CVector a, bool inv = false)
{
	int n = a.size();
	std::vector<int> bits(n, 0);
	int LOG = 0;
	while(1 << (LOG + 1) < n)
		LOG++;
	for(int i = 1; i < n; i++)
		bits[i] = (bits[i >> 1] >> 1) | ((i & 1) << LOG);
	for(int i = 0; i < n; i++)
	{
		int to = bits[i];
		if(to > i)
			std::swap(a[to], a[i]);
	}
	
	CVector r(n / 2, 0);
	double angle = inv ? -1 : 1;
	r[0] = Complex(1, 0);
	r[1] = Complex(cosl(angle * 2 * PI / n), sinl(angle * 2 * PI / n));
	for(int i = 2; i < n / 2; i++)
		r[i] = r[i - 1] * r[1];
	for(int len = 1; len < n; len *= 2)
	{
		int delta = n / (2 * len);
		for(int i = 0; i < n; i += 2 * len)
		{
			int cur_root = 0;
			for(int j = 0; j < len; j++)
			{
				Complex u = a[i + j], v = a[i + j + len] * r[cur_root];
				a[i + j] = u + v;
				a[i + j + len] = u - v;
				cur_root += delta;
			}
		}
	}
	if(inv)
	{
		for(int i = 0; i < n; i++)
			a[i] /= n;
	}
	return a;
}
