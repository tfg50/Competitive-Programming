#include <vector>

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
		// neutral element
	}
	
	Node() {
		// init
	}
	
	Node(Node l, Node r) {
		// merge
	}

	bool canBreak(LazyContext lazy) {
		// return true if can break without applying lazy
	}

	bool canApply(LazyContext lazy) {
		// returns true if can apply lazy
	}
	
	void apply(LazyContext &lazy) {
		// changes lazy if needed
	}
	
	// atributes
};

template <class i_t, class e_t, class lazy_cont>
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
			if(l & 1) downUpd(l++, lc);
			if(r & 1) downUpd(--r, lc);
		}
		build(l0);
		build(r0 - 1);
	}

	void upd(int pos, e_t v) {
		pos += n;
		push(pos);
		tree[pos] = i_t(v);
		build(pos);
	}
private:
	int n, h;
	std::vector<bool> dirty;
	std::vector<i_t> tree;
	std::vector<lazy_cont> lazy;

	void apply(int p, lazy_cont lc) {
		tree[p].apply(lc);
		if(p < n) {
			dirty[p] = true;
			lazy[p] += lc;
		}
	}

	void pushSingle(int p) {
		if(dirty[p]) {
			downUpd(p + p, lazy[p]);
			downUpd(p + p + 1, lazy[p]);
			lazy[p].reset();
			dirty[p] = false;
		}
	}

	void push(int p) {
		for(int s = h; s > 0; s--) {
			pushSingle(p >> s);
		}
	}

	void downUpd(int p, lazy_cont lc) {
		if(tree[p].canBreak(lc)) {
			return;
		} else if(tree[p].canApply(lc)) {
			apply(p, lc);
		} else {
			pushSingle(p);
			downUpd(p + p, lc);
			downUpd(p + p + 1, lc);
			tree[p] = i_t(tree[p + p], tree[p + p + 1]);
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
