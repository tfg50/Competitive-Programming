template<class T, class F>
std::vector<T> multiplicativeFunction(int n, F f) {
	std::vector<T> ans(n+1, 1);
	std::vector<bool> isPrime(n+1, true);
	for(int i = 2; i <= n; i++) {
		if(!isPrime[i]) continue;
		for(int j = i, e = 1; 1; j *= i, e++) {
			ans[j] = f(i, e);
			if(n / i / j == 0) break;
		}
		for(int j = i+i; j <= n; j += i) {
			int x = j;
			while(x % i == 0) x /= i;
			ans[j] = ans[x] * ans[j / x];
			isPrime[j] = false;
		}
	}
	return ans;
}