template<class T>
struct Minimizer {
	T operator() (T a, T b) { return std::min(a, b); }
};

template <class T, class Merger = Minimizer<T>>
class SparseTable {
public:
	void init(const std::vector<T> &a) {
		int e = 0;
		int n = a.size();
		while((1 << e) / 2 < n) {
			e++;
		}
		table.assign(e, std::vector<T>(n));
		get.assign(n + 1, -1);
		for(int i = 0; i < n; i++) {
			table[0][i] = a[i];
			get[i+1] = get[(i+1)/2] + 1;
		}
		for(int i = 0; i + 1 < e; i++) {
			for(int j = 0; j + (1 << i) < n; j++) {
				table[i+1][j] = merge(table[i][j], table[i][j + (1 << i)]);
			}
		}
	}
	
	T qry(int l, int r) {
		int e = get[r - l];
		return merge(table[e][l], table[e][r - (1 << e)]);
	}

	int getPos(int x) {
		while(x >= (int) get.size()) get.push_back(get[(int) get.size() / 2] + 1);
		return get[x];
	}
private:
	std::vector<std::vector<T>> table;
	std::vector<int> get;
	Merger merge;
};