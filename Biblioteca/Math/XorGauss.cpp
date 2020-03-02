template<const int N, class T = unsigned int>
class XorGauss {
public:
	XorGauss() {
		for(int i = 0; i < N; i++) { a[i] = 0; }
	}
	T reduce(T x) {
		for(int i = N-1; i >= 0; i--) {
			x = std::min(x, x ^ a[i]);
		}
		return x;
	}

	T augment(T x) { return ~reduce(~x); }

	bool add(T x) {
		for(int i = N-1; i >= 0; i--) {
			if((x & (1 << i)) == 0) { continue; }
			if(a[i]) {
				x ^= a[i];
			} else {
				a[i] = x;
				return true;
			}
		}
		return false;
	}
private:
	T a[N];
};