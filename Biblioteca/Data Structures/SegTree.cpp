// Range update, range query segment tree

struct LazyContext {
	LazyContext() {
		
	}
	
	void reset() {
		
	}
	
	void operator += (LazyContext o) {
		
	}
	
	// atributes
};

struct Node {
	Node() {
		// neutral element or empty node
	}
	
	Node() {
		// init
	}
	
	Node(Node &l, Node &r) {
		// merge
	}
	
	void apply(LazyContext lazy) {
		
	}
	
	// atributes
};

template <class i_t, class e_t, class lazy_cont = int>
class SegmentTree {
public:
	void init(std::vector<e_t> base) {
		n = base.size();
		h = 0;
		while((1 << h) < n) h++;
		tree.resize(2 * n);
		dirty.assign(n, false);
		lazy.resize(n);
		for(int i = 0; i < n; i++) {
			tree[i + n] = i_t(base[i]);
		}
		for(int i = n - 1; i > 0; i--) {
			tree[i] = i_t(tree[i + i], tree[i + i + 1]);
			lazy[i].reset();
		}
	}
	
	i_t qry(int l, int r) {
		if(l >= r) return i_t();
		l += n, r += n;
		push(l);
		push(r - 1);
		i_t lp, rp;
		for(; l < r; l /= 2, r /= 2) {
			if(l & 1) lp = i_t(lp, tree[l++]);
			if(r & 1) rp = i_t(tree[--r], rp);
		}
		return i_t(lp, rp);
	}
	
	void upd(int l, int r, lazy_cont lc) {
		if(l >= r) return;
		l += n, r += n;
		push(l);
		push(r - 1);
		int l0 = l, r0 = r;
		for(; l < r; l /= 2, r /= 2) {
			if(l & 1) apply(l++, lc);
			if(r & 1) apply(--r, lc);
		}
		build(l0);
		build(r0 - 1);
	}
	
	template<class F>
	int search(int l, int r, F f) {
		assert(l <= r);
		int lst = r;
		l += n, r += n;
		push(l);
		push(r-1);
		std::vector<int> pref, suf;
		pref.reserve(h+1), suf.reserve(h+1);
		for(; l < r; l /= 2, r /= 2) {
			if(l & 1) pref.push_back(l++);
			if(r & 1) suf.push_back(--r);
		}
		std::reverse(suf.begin(), suf.end());
		i_t cur;
		for(int rep = 0; rep < 2; rep++) {
			for(auto i : rep == 0 ? pref : suf) {
				if(f(i_t(cur, tree[i]))) {
					while(i < n) {
						pushNode(i);
						i += i;
						i_t other = i_t(cur, tree[i]);
						if(!f(other)) {
							cur = other;
							i++;
						}
					}
					return i - n;
				}
				cur = i_t(cur, tree[i]);
			}
		}
		return lst;
	}
private:
	int n, h;
	std::vector<bool> dirty;
	std::vector<i_t> tree;
	std::vector<lazy_cont> lazy;
	
	void apply(int p, lazy_cont &lc) {
		tree[p].apply(lc);
		if(p < n) {
			dirty[p] = true;
			lazy[p] += lc;
		}
	}
	
	void push(int p) {
		for(int s = h; s > 0; s--) {
			int i = p >> s;
			pushNode(i);
		}
	}

	inline void pushNode(int i) {
		if(dirty[i]) {
			apply(i + i, lazy[i]);
			apply(i + i + 1, lazy[i]);
			lazy[i].reset();
			dirty[i] = false;
		}
	}
	
	void build(int p) {
		for(p /= 2; p > 0; p /= 2) {
			tree[p] = i_t(tree[p + p], tree[p + p + 1]);
			if(dirty[p]) {
				tree[p].apply(lazy[p]);
			}
		}
	}
};