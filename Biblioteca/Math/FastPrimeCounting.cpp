const int ms = 5001000;
const int lim_n = 3e5;
const int lim_p = 1e2;

std::vector<int> primes;
int id[ms];
int memo[lim_n][lim_p];
void pre() {
	std::vector<bool> isPrime(ms, true);
	for(int i = 2; i < ms; i++) {
		id[i] = (int) primes.size();
		if(!isPrime[i]) continue;
		id[i]++;
		primes.push_back(i);
		for(int j = i+i; j < ms; j += i) {
			isPrime[j] = false;
		}
	}
	for(int i = 1; i < lim_n; i++) {
		memo[i][0] = i;
		for(int j = 1; j < lim_p; j++) {
			memo[i][j] = memo[i][j-1] - memo[i/primes[j-1]][j-1];
		}
	}
}

int cbc(long long n) {
	int ans = std::max(0, (int) pow((double) n, 1.0 / 3) - 2);
	while((long long) ans * ans * ans < n) ans++;
	return ans;
}


long long dp(long long n, int i) {
	if(n == 0) return 0;
	if(i == 0) return n;
	if((long long) primes[i-1] * primes[i-1] > n && n < ms) {
		return max(1, id[n] - (i-1));
	} else if(n < lim_n && i < lim_p) {
		return memo[n][i];
	} else {
		return dp(n, i-1) - dp(n / primes[i-1], i-1);
	}
}

long long primeFunction(long long n) {
	if(n < ms) {
		return id[(int)n];
	}
	int i = id[cbc(n)];
	long long ans = dp(n, i) + i - 1;
	while((long long) primes[i] * primes[i] <= n) {
		ans -= primeFunction(n / primes[i]) - i;
		i++;
	}
	return ans;
}