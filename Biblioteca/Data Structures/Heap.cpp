// supports lazy addition to values in the heap

template<class G>
struct Heap {
	int size() const { return (int) hp.size(); }
	long long top() const { return hp.top() + lazy; }
	void pop() { hp.pop(); }
	void push(long long x) { hp.push(x - lazy); }

	long long lazy = 0;
	std::priority_queue<long long, std::vector<long long>, G> hp;

	void print() {
		if(hp.empty()) {
			return;
		} else {
			std::cout << hp.top() + lazy << ' ';
			auto v = hp.top();
			hp.pop();
			print();
			hp.push(v);
		}
	}
};