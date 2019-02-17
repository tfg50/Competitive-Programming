typedef int Key;
struct Treap {
	Treap(){}
	Treap(int k) {
		key = 1;
		size = 1;
		l = r = NULL;
		sum = val = k;
	}

	Treap *l, *r;
	Key key;
	int val;
	long long sum;
	int size;
};

typedef Treap * PTreap;

bool leftSide(PTreap l, PTreap r) {
	return (int) (rng() % (l->size + r->size)) < l->size;
}

void fix(PTreap t) {
	if(t == NULL) {
		return;
	}
	t->size = 1;
	t->key = 1;
	t->sum = t->val;
	if(t->l) {
		t->size += t->l->size;
		t->key += t->l->size;
		t->sum += t->l->sum;
	}
	if(t->r) {
		t->size += t->r->size;
		t->sum += t->r->sum;
	}
}

void split(PTreap t, Key key, PTreap &l, PTreap &r) {
	if(t == NULL) {
		l = r = NULL;
	} else if(t->key <= key) {
		l = new Treap();
		*l = *t;
		split(t->r, key - t->key, l->r, r);
		fix(l);
	} else {
		r = new Treap();
		*r = *t;
		split(t->l, key, l, r->l);
		fix(r);
	}
}

void merge(PTreap &t, PTreap l, PTreap r) {
	if(!l || !r) {
		t = l ? l : r;
		return;
	}
	t = new Treap();
	if(leftSide(l, r)) {
		*t = *l;
		merge(t->r, l->r, r);
	} else {
		*t = *r;
		merge(t->l, l, r->l);
	}
	fix(t);
}

PTreap build(int l, int r, std::vector<int> &a) {
	if(l >= r) return NULL;
	int mid = (l + r) / 2;
	auto ans = new Treap(a[mid]);
	ans->l = build(l, mid, a);
	ans->r = build(mid + 1, r, a);
	fix(ans);
	return ans;
}

long long KSum(PTreap t, int id) {
	if(t == NULL || id == 0) {
		return 0;
	}
	long long sum = 0;
	if(t->l && t->l->size >= id) {
		return KSum(t->l, id);
	}
	if(t->l) {
		sum += t->l->sum;
		id -= t->l->size;
	}
	if(id == 0) {
		return sum;
	}
	id--;
	sum += t->val;
	return sum + KSum(t->r, id);
}
