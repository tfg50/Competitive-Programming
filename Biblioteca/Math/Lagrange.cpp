#include <vector>

typedef long long ll;

const ll MOD = ll(1e9) + 7;
const int ms = int(1e6) + 20;

ll fexp(ll x, ll e)
{
	ll ans = 1;
	for(; e; e >>= 1)
	{
		if(e & 1)
			ans = ans * x % MOD;
		x = x * x % MOD;
	}
	return ans;
}

ll fat[ms];
ll ifat[ms];

class LagrangePoly
{
public:
	LagrangePoly(std::vector<ll> _a)
	{
		//f(i) = _a[i]
		//interpola o vetor em um polinomio de grau y.size() - 1
		y = _a;
		den.resize(y.size());
		for(int i = 0; i < den.size(); i++)
		{
			den[i] = ifat[den.size() - i - 1] * ifat[i] % MOD;
			den[i] = den[i] * fexp(MOD - 1, den.size() - i - 1) % MOD;
		}
	}
	
	ll getVal(ll x)
	{
		std::vector<ll> delta(den.size() + 2, 1);
		for(int i = 0; i < den.size(); i++)
			delta[i + 1] = (x - i + MOD) % MOD;
		std::vector<ll> l, r;
		l = r = std::vector<ll> (den.size() + 2, 1);
		for(int i = 1; i < l.size(); i++)
			l[i] = l[i - 1] * delta[i] % MOD;
		for(int i = r.size() - 2; i >= 0; i--)
			r[i] = r[i + 1] * delta[i] % MOD;
		std::vector<ll> coef (den.size(), 1);
		for(int i = 0; i < coef.size(); i++)
			coef[i] = l[i] * r[i + 2] % MOD;
		ll ans = 0;
		for(int i = 0; i < coef.size(); i++)
			ans = (ans + (y[i] * coef[i] % MOD) * den[i] % MOD) % MOD;
		return ans;
	}
	
private:
	std::vector<ll> y, den;
};

#include <iostream>

int main()
{
	fat[0] = ifat[0] = 1;
	for(int i = 1; i < ms; i++)
	{
		fat[i] = fat[i - 1] * i % MOD;
		ifat[i] = fexp(fat[i], MOD - 2);
	}
	/*int n, q;
	while(std::cin >> n >> q)
	{
		std::vector<int> a(n, 0);
		for(int i = 0; i < n; i++)
			std::cin >> a[i];
		LagrangePoly f(a);
		while(q--)
		{
			ll x;
			std::cin >> x;
			std::cout << f.getVal(x) << '\n';
		}
	}*/
	// Codeforces 622F
	int x, k;
	std::cin >> x >> k;
	std::vector<ll> a;
	a.push_back(0);
	for(ll i = 1; i <= k + 1; i++)
		a.push_back((a.back() + fexp(i, k)) % MOD);
	LagrangePoly f(a);
	std::cout << f.getVal(x) << '\n';
}
