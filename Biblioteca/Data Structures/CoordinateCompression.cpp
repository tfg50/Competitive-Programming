template<class T>
class CoordinateCompression {
public:
	CoordinateCompression(const std::vector<T> &a) {
		arr = a;
		std::sort(arr.begin(), arr.end());
		arr.resize(std::unique(arr.begin(), arr.end()) - arr.begin());
	}

	int getL(T x) { return std::lower_bound(arr.begin(), arr.end(), x) - arr.begin(); }
	int getR(T x) { return std::upper_bound(arr.begin(), arr.end(), x) - arr.begin(); }
	int size() const { return (int) arr.size(); }
private:
	std::vector<T> arr;
};
