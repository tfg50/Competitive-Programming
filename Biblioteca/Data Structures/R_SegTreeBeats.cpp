// Recursive segment tree beats
// To make it turn into a standard segment tree, just
// make canBreak() == false and canApply() == true

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
		tree.resize(4 * n);
		dirty.assign(2 * n, false);
		lazy.resize(2 * n);
		build(1, 0, n - 1, base);
	}
	
	i_t qry(int l, int r) { return qry(1, 0, n - 1, l, r - 1); }
	void upd(int l, int r, lazy_cont lc) { upd(1, 0, n - 1, l, r - 1, lc); }
	void upd(int pos, e_t v) { upd(1, 0, n - 1, pos, v); }
private:
	int n;
	std::vector<bool> dirty;
	std::vector<i_t> tree;
	std::vector<lazy_cont> lazy;

	void build(int p, int l, int r, std::vector<e_t> &base) {
		if(l == r) {
			tree[p] = i_t(base[l]);
		} else {
			int mid = (l + r) / 2;
			build(p + p, l, mid, base);
			build(p + p + 1, mid + 1, r, base);
			tree[p] = i_t(tree[p + p], tree[p + p + 1]);
		}
	}

	void apply(int p, int l, int r, lazy_cont &lc) {
		tree[p].apply(lc);
		if(l != r) {
			dirty[p] = true;
			lazy[p] += lc;
		}
	}

	void push(int p, int l, int r) {
		if(dirty[p]) {
			int mid = (l + r) / 2;
			upd(p + p, l, mid, l, mid, lazy[p]);
			upd(p + p + 1, mid + 1, r, mid + 1, r, lazy[p]);
			lazy[p].reset();
			dirty[p] = false;
		}
	}

	void upd(int p, int l, int r, int pos, e_t &v) {
		if(l == r) {
			tree[p] = i_t(v);
		} else {
			push(p, l, r);
			int mid = (l + r) / 2;
			if(pos <= mid) {
				upd(p + p, l, mid, pos, v);
			} else {
				upd(p + p + 1, mid + 1, r, pos, v);
			}
			tree[p] = i_t(tree[p + p], tree[p + p + 1]);
		}
	}

	void upd(int p, int l, int r, int i, int j, lazy_cont lc) {
		if(i > r || j < l || tree[p].canBreak(lc)) {
			return;
		} else if(i <= l && r <= j && tree[p].canApply(lc)) {
			apply(p, l, r, lc);
		} else {
			push(p, l, r);
			int mid = (l + r) / 2;
			upd(p + p, l, mid, i, j, lc);
			upd(p + p + 1, mid + 1, r, i, j, lc);
			tree[p] = i_t(tree[p + p], tree[p + p + 1]);
		}
	}

	i_t qry(int p, int l, int r, int i, int j) {
		if(i > r || j < l) {
			return i_t();
		} else if(i <= l && r <= j) {
			return tree[p];
		} else {
			push(p, l, r);
			int mid = (l + r) / 2;
			return i_t(qry(p + p, l, mid, i, j), qry(p + p + 1, mid + 1, r, i, j));
		}
	}
};
