typedef int Key;
struct Treap {
	Treap(){}
	Treap(Key k) {
		key = k;
		prio = std::uniform_int_distribution<int>()(rng);
		size = 1;
		l = r = NULL;
	}

	Treap *l, *r;
	Key key;
	int prio;
	int size;
};

typedef Treap * PTreap;

void fix(PTreap t) {
	if(t == NULL) {
		return;
	}
	t->size = 1;
	if(t->l) {
		t->size += t->l->size;
	}
	if(t->r) {
		t->size += t->r->size;
	}
}

void pushLazy(PTreap t) {
	if(t == NULL) {
		return;
	}
}

void split(PTreap t, Key key, PTreap &l, PTreap &r) {
	pushLazy(t);
	if(t == NULL) {
		l = r = NULL;
	} else if(t->key < key) {
		l = t;
		split(t->r, key, t->r, r);
	} else {
		r = t;
		split(t->l, key, l, t->l);
	}
	fix(t);
}

void insert(PTreap &t, PTreap it) {
	pushLazy(t);
	if(t == NULL) {
		t = it;
	} else if(it->prio > t->prio) {
		split(t, it->key, it->l, it->r);
		t = it;
	} else {
		insert(t->key < it->key ? t->r : t->l, it);
	}
	fix(t);
}

void merge(PTreap &t, PTreap l, PTreap r) {
	if(!l || !r) {
		t = l ? l : r;
		return;
	}
	pushLazy(l);
	pushLazy(r);
	if(l->prio > r->prio) {
		merge(l->r, l->r, r);
		t = l;
	} else {
		merge(r->l, l, r->l);
		t = r;
	}
	fix(t);
}

PTreap find(PTreap t, Key key) {
	if(t == NULL) {
		return NULL;
	} else if(t->key == key) {
		return t;
	} else {
		return find(t->key < key ? t->r : t->l, key);
	}
}

PTreap erase(PTreap &t, Key key) {
	if(t == NULL) {
		return NULL;
	}
	pushLazy(t);
	if(t->key == key) {
		PTreap ret = t;
		merge(t, t->l, t->r);
		fix(t);
		return ret;
	} else {
		PTreap ret = erase(t->key < key ? t->r : t->l, key);
		fix(t);
		return ret;
	}
}

PTreap kth_element(PTreap t, int k) {
	if(!t) {
		return NULL;
	}
	if(t->l) {
		if(t->l->size >= k) {
			return kth_element(t->l, k);
		} else {
			k -= t->l->size;
		}
	}
	if(k == 1) {
		return t;
	} else {
		return kth_element(t->r, k - 1);
	}
}

int countLeft(PTreap t, Key key) {
	if(!t) {
		return 0;
	} else if(t->key < key) {
		return 1 + (t->l ? t->l->size : 0) + countLeft(t->r, key);
	} else {
		return countLeft(t->l, key);
	}
}
