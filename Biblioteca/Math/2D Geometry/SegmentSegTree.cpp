const double eps = 1e-8;
// this code isn't complete
struct PT {
	typedef long long T;
	T x, y;
	PT(T xx = 0, T yy = 0) : x(xx), y(yy){}
	PT operator +(const PT &p) const { return PT(x+p.x,y+p.y); }
	PT operator -(const PT &p) const { return PT(x-p.x,y-p.y); }
	T operator *(const PT &p)  const { return x*p.x+y*p.y;     }
	T operator %(const PT &p)  const { return x*p.y-y*p.x;     }
	bool operator < (const PT &p) const { return x != p.x ? x < p.x : y < p.y; }
};

struct Segment {
	PT a, b;

	void read() {
		std::cin >> a.x >> a.y >> b.x >> b.y;
		fix();
	}

	void fix() {
		if(a.x > b.x) {
			std::swap(a, b);
		} else if(a.x == b.x && a.y > b.y) {
			std::swap(a, b);
		}
	}

	PT norm() const {
		PT ans = a - b;
		ans = PT(ans.y, -ans.x);
		if(ans.y < 0) {
			ans = PT() - ans;
		}
		return ans;
	}

	double eval(long long x) const {
		// a.x + (b.x - a.x) * t == x
		double t = (double) (x - a.x) / (double) (b.x - a.x);
		return (double) a.y + (double) (b.y - a.y) * t;
	}

	bool operator < (Segment o) const {
		long long l = std::max(a.x, o.a.x);
		long long r = std::min(b.x, o.b.x);
		assert(l <= r);
		return std::min(eval(l) - o.eval(l), eval(r) - o.eval(r)) < -eps;
		//return eval(l) < o.eval(l);
	}

	int getInter(PT pt) {
		long long val = norm() * (pt - a);
		if(val < 0) return -1;
		else if(val == 0) return 0;
		else return 1;
	}

	PT getLower() {
		if(a.y < b.y) return a;
		else return b;
	}
};

const int INF = 2e6;

struct SegTree {
	int getID(long long x) {
		return std::lower_bound(coords.begin(), coords.end(), x) - coords.begin();
	}
	void init(std::vector<Segment> segs) {
		const int pointLocation = 1;
		for(auto &seg : segs) {
			if(seg.a.x == seg.b.x) {
				// vertical
				vert.emplace_back(seg.a, seg.b);
			} else {
				// horizontal
				coords.push_back(seg.a.x + pointLocation);
				coords.push_back(seg.a.x-1 + pointLocation);
				coords.push_back(seg.b.x);
				coords.push_back(seg.b.x+1);
			}
		}
		std::sort(coords.begin(), coords.end());
		coords.resize(std::unique(coords.begin(), coords.end()) - coords.begin());
		n = (int) coords.size();
		tree.resize(2 * n);
		for(auto seg : segs) {
			if(seg.a.x != seg.b.x) {
				upd(getID(seg.a.x + pointLocation), getID(seg.b.x+1), seg);
			}
		}
		// part for vertical segments
		std::sort(vert.begin(), vert.end());
		std::vector<std::pair<PT, PT>> ver;
		for(int l = 0, r = 0; l < (int) vert.size(); l = r) {
			while(r < (int) vert.size() && vert[r].first.x == vert[l].first.x) {
				r++;
			}
			std::pair<PT, PT> cur = vert[l];
			for(int i = l+1; i < r; i++) {
				if(cur.second < vert[i].first) {
					ver.push_back(cur);
					cur = vert[i];
				} else if(cur.second < vert[i].second) {
					cur.second = vert[i].second;
				}
			}
			ver.push_back(cur);
		}
		ver.swap(vert);
		// part for non-vertical segments
		for(int i = 1; i < n+n; i++) {
			std::sort(tree[i].begin(), tree[i].end());
		}
	}

	void upd(int l, int r, Segment seg) {
		for(l += n, r += n; l < r; l /= 2, r /= 2) {
			if(l & 1) tree[l++].push_back(seg);
			if(r & 1) tree[--r].push_back(seg);
		}
	}

	int bSearch(int pos, PT ref, int off) {
		// returns last position with below the point
		// off == 0 for below
		// off == 1 for strictly
		int l = -1, r = (int) tree[pos].size() - 1;
		while(l != r) {
			int mid = (l + r + 1) / 2;
			if(tree[pos][mid].getInter(ref) >= off) {
				l = mid;
			} else {
				r = mid-1;
			}
		}
		return l;
	}

	int qry(PT ref) {
		// return 1 if it's inside
		// return 0 if it's in the border
		// return -1 if outside
		{
			// solving vertical segments
			auto it = std::lower_bound(vert.begin(), vert.end(), std::pair<PT, PT>(ref+PT(0, 1), ref));
			if(it != vert.begin()) {
				it--;
				if(ref.x == it->first.x && it->first.y <= ref.y && ref.y <= it->second.y) {
					return 0;
				}
			}
		}
		int pos = getID(ref.x);
		if(pos == n) {
			return -1;
		}
		int ans = 0;
		for(pos += n; pos > 0; pos /= 2) {
			int l = bSearch(pos, ref, 0);
			if(l == -1) continue;
			ans = (ans + l + 1) % 2;
			if(tree[pos][l].getInter(ref) == 0) {
				return 0;
			}
		}
		ans = ans == 0 ? -1 : 1;
		return ans;
	}

	Segment qry(PT ref, int off) {
		// returns the segment nearest to ref
		// off = 0 allows for below
		// off = 1 allows for strictly bellow
		int pos = getID(ref.x);
		Segment ans;
		ans.a = PT(-INF, -INF+1);
		ans.b = PT(INF, -INF);
		if(pos == n) {
			return ans;
		}
		for(pos += n; pos > 0; pos /= 2) {
			int l = bSearch(pos, ref, off);
			if(l == -1) continue;
			if(ans < tree[pos][l]) {
				ans = tree[pos][l];
			}
		}
		return ans;
	}

	int n;
	std::vector<std::vector<Segment>> tree;
	std::vector<std::pair<PT, PT>> vert;
	std::vector<long long> coords;
};
