std::vector<PT> ConvexHull (std::vector<PT> pts) {
	if(pts.size() <= 1) return pts;
	std::sort(pts.begin(), pts.end(), [](PT a, PT b) -> bool { return a.x != b.x ? a.x < b.x : a.y < b.y; });
	//pts.resize(std::unique(pts.begin(), pts.end(), [](PT a, PT b) -> bool { return a.x == b.x && a.y == b.y; }) - pts.begin());
	std::vector<PT> ans(2 * pts.size());
	int s = 0;
	for(int i = 0; i < pts.size(); i++) {
		while(s > 1 && (pts[i] - ans[s - 2]) % (ans[s - 1] - ans[s - 2]) >= 0) {
			s--;
		}
		ans[s++] = pts[i];
	}
	for(int i = pts.size() - 1, t = s + 1; i > 0; i--) {
		while(s >= t && (pts[i - 1] - ans[s - 2]) % (ans[s - 1] - ans[s - 2]) >= 0) {
			s--;
		}
		ans[s++] = pts[i - 1];
	}
	ans.resize(s - 1);
	return ans;
}

bool isInside(const std::vector<PT> &hull, PT point) {
	int n = hull.size();
	if((point - hull[0]) % (hull[1] - hull[0]) > 0 || (point - hull[0]) % (hull[n-1] - hull[0]) < 0) {
		return false;
	}
	int l = 1, r = n - 1;
	while(l != r) {
		int mid = (l + r + 1) / 2;
		if((point - hull[0]) % (hull[mid] - hull[0]) < 0) {
			l = mid;
		} else {
			r = mid - 1;
		}
	}
	return (hull[(l+1)%n] - hull[l]) % (point - hull[l]) >= 0;
}

int maximizeScalarProduct(const std::vector<PT> &hull, PT vec) {
	int ans = 0;
	int n = hull.size();
	if(n < 20) {
		for(int i = 0; i < n; i++) {
			if(hull[i] * vec > hull[ans] * vec) {
				ans = i;
			}
		}
	} else {
		int diff = 1;
		if(hull[0] * vec == hull[1] * vec) {
			int l = 2, r = n - 1;
			while(l != r) {
				int mid = (l + r) / 2;
				if((hull[1] - hull[0]) * (hull[mid] - hull[0]) > 0 && (hull[1] - hull[0]) % (hull[mid] - hull[0]) == 0) {
					l = mid + 1;
				} else {
					r = mid;
				}
			}
			diff = l;
			//diff = 2;
		}
		if(hull[0] * vec < hull[diff] * vec) {
			int l = diff, r = n - 1;
			while(l != r) {
				int mid = (l + r + 1) / 2;
				if(hull[mid] * vec >= hull[mid - 1] * vec && hull[mid] * vec >= hull[0] * vec) {
					l = mid;
				} else {
					r = mid - 1;
				}
			}
			if(hull[0] * vec < hull[l] * vec) {
				ans = l;
			}
		} else {
			int l = diff, r = n - 1;
			while(l != r) {
				int mid = (l + r + 1) / 2;
				if(hull[mid] * vec >= hull[mid - 1] * vec || hull[mid - 1] * vec < hull[0] * vec) {
					l = mid;
				} else {
					r = mid - 1;
				}
			}
			if(hull[0] * vec < hull[l] * vec) {
				ans = l;
			}
		}
	}
	return ans;
}