struct LazyContext {
	LazyContext() { }
	void reset() { }
	void operator += (LazyContext o) { }
	// atributes
};

struct Info {
	Info() {}
	Info(int x) {}
	Info(Info l, Info r) {}
	void apply(LazyContext lazy, int size) {}
	void rotate() {}
	//bool operator < (const Info &o) const { return ans < o.ans; }
	// atributes
};

template<class i_t, class lazy_cont>
struct SplayTree {
	struct Node {
		Node *pp = NULL, *p = NULL, *left = NULL, *right = NULL;
		int size = 1;
		bool flip = false;
		i_t base, info;
		lazy_cont lazy;
		Node() {}
		Node(i_t v) : base(v), info(v) {}
		~Node() {
			if(left) delete left;
			if(right) delete right;
		}

		void applyLazy(lazy_cont l) {
			info.apply(l, size);
			base.apply(l, 1);
			lazy += l;
		}

		void fix() {
			push(), info = base;
			size = 1;
			if(left) info = i_t(left->info, info), size += left->size, left->p = this;
			if(right) info = i_t(info, right->info), size += right->size, right->p = this;
		}

		Node* splay() {
			pull();
			while(p) {
				Node *y = p, *z = p->p;
				bool t1 = (y->left != this);
				bool t2 = z && (z->left != y) == t1;
				if(t2) y->rotate(t1);
				rotate(t1);
				if(z && !t2) rotate(!t1);
			}
			fix();
			return this;
		}
		
		void push() {
			if(flip) {
				std::swap(left, right);
				if(left) left->flip = !left->flip, left->info.rotate();
				if(right) right->flip = !right->flip, right->info.rotate();
				flip = false;
			}
			if(left) left->applyLazy(lazy);
			if(right) right->applyLazy(lazy);
			lazy.reset();
		}

		void pull() {
			if(p) {
				p->pull();
				pp = p->pp;
			}
			push();
		}

		void rotate(bool leftRotate) {
			Node* y = p, *z = p->p, *&w = leftRotate ? left : right;
			if(z) (z->right == y ? z->right : z->left) = this;
			if(w) w->p = y;
			(leftRotate ? y->right : y->left) = w;
			w = y; p = z;
			y->p = this;
			y->fix();
			//fix();
		}
	};
	Node* root;
	SplayTree(Node* r = NULL) : root(r) {}
	Node* lower_bound(i_t val) { return search(val, [](const i_t &a, const i_t &b){ return !(b < a); }); }
	Node* upper_bound(i_t val) { return search(val, [](const i_t &a, const i_t &b){ return a < b; }); }

	Node* splayFirst() { return expose(1); }
	Node* splayLast() { return expose(1e9); }
	Node* expose(int k) {
		Node* cur = root;
		if(!cur) return NULL;
		k = std::min(k, cur->size);
		k = std::max(k, 1);
		while(cur) {
			cur->push();
			if(cur->left) {
				if(cur->left->size >= k) {
					cur = cur->left;
					continue;
				} else {
					k -= cur->left->size;
				}
			}
			if(k == 1) {
				break;
			}
			k--;
			cur = cur->right;
		}
		assert(cur);
		return root = cur->splay();
	}

	template<class F>
	Node* search(i_t val, F f) {
		Node* ans = NULL, *cur = root, *last = NULL;
		while(cur) {
			cur->push();
			last = cur;
			if(f(val, cur->base)) {
				ans = cur;
				cur = cur->left;
			} else {
				cur = cur->right;
			}
		}
		root = last->splay();
		if(ans) root = ans->splay();
		return ans;
	}
	void insert(i_t t) { insert(new Node(t)); }
	void insert(Node* node) {
		if(!root) {
			root = node;
			return;
		}
		Node* tree = root;
		while(1) {
			tree->push();
			Node* &w = node->base < tree->base ? tree->left : tree->right;
			if(w) {
				tree = w;
			} else {
				w = node;
				tree->fix();
				break;
			}
		}
		root = node->splay();
	}

	void erase(Node* node) {
		node->pull();
		if(node == root) {
			if(node->left) node->left->p = NULL;
			if(node->right) node->right->p = NULL;
			*this = SplayTree(node->left) + SplayTree(node->right);
		} else {
			assert(node->p);
			Node* &w = node->p->left == node ? node->p->left : node->p->right;
			if(!node->left || !node->right) {
				w = node->left ? node->left : node->right;
			} else {
				node->left->p = node->right->p = NULL;
				w = (SplayTree(node->left) + SplayTree(node->right)).root;
			}
			node->p->fix();
			root = node->p->splay();
		}
	}

	SplayTree operator + (SplayTree o) {
		if(!root) return o;
		if(!o.root) return *this;
		splayLast();
		assert(!root->right && !o.root->p);
		root->right = o.root;
		root->fix();
		return SplayTree(root);
	}
};

typedef SplayTree<Info, LazyContext> Splay;