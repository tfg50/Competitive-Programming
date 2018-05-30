std::vector<PT> ConvexHull (std::vector<PT> pts) {
	if(pts.size() <= 1) return pts;
	std::sort(pts.begin(), pts.end(),
		[](PT a, PT b) -> bool {
			if(a.x != b.x) return a.x < b.x;
			else return a.y < b.y;
		});
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
