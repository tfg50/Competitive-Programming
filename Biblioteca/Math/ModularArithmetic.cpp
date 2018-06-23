template <int mod = MOD>
struct modBase {
	modBase(int val = 0) : val(val) {}
	int val;

	modBase<mod> operator * (modBase<mod> o) { return (long long) val * o.val % mod; }
	modBase<mod> operator + (modBase<mod> o) { return val + o.val > mod ? val + o.val - mod : val + o.val; }
};

template <class T>
T fexp(T x, long long e) {
	T ans(1);
	for(; e > 0; e /= 2) {
		if(e & 1) ans = ans * x;
		x = x * x;
	}
	return ans;
}
