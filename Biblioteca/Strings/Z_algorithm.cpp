// credits to FMota/UFCG

template <class T>
struct ZFunc {
	std::vector<int> z;
	ZFunc(const std::vector<T> &v) : z(v.size()) {
		int n = (int) v.size(), a = 0, b = 0;
		if (!z.empty()) z[0] = n;
		for (int i = 1; i < n; i++) {
			int end = i; if (i < b) end = std::min(i + z[i - a], b);
			while(end < n && v[end] == v[end - i]) ++end;
			z[i] = end - i; if(end > b) a = i, b = end;
		}
	}
};
