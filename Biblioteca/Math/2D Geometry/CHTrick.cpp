struct CHTrick {
	// queries are (x, 1) * (pt.x, pt.y)
	// this code is for max
	CHTrick(){ n = 0; }
	CHTrick(std::vector<PT> pts) {
		std::sort(pts.rbegin(), pts.rend());
		n = 0;
		for(auto pt : pts) {
			while(n >= 2 && (hull[n-1] - hull[n-2]) % (pt - hull[n-2]) <= 0) {
				hull.pop_back();
				n--;
			}
			hull.push_back(pt);
			n++;
		}
	}

	long long qry(long long x) {
		if(hull.empty()) return (long long) (-1e18);
		int l = 0, r = n-1;
		PT cur(x, 1);
		while(l != r) {
			int mid = (l + r) / 2;
			if(hull[mid] * cur <= hull[mid+1] * cur) {
				l = mid + 1;
			} else {
				r = mid;
			}
		}
		return hull[l] * cur;
	}

	int n;
	std::vector<PT> hull;
};