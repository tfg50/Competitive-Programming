ii ClosestPair(const std::vector<PT> &pts) {
	auto sqrDist = [](PT a) { return a*a; };
	long long dist = sqrDist(pts[0] - pts[1]);
	ii ans(0, 1);
	int n = pts.size();
	std::vector<int> p(n);
	for(int i = 0; i < n; i++) {
		p[i] = i;
	}
	std::sort(p.begin(), p.end(), [&](int a, int b) { return pts[a].x < pts[b].x; });
	std::set<ii> points;
	auto sqr = [](long long x) -> long long { return x * x; };
	for(int l = 0, r = 0; r < n; r++) {
		while(sqr(pts[p[r]].x - pts[p[l]].x) > dist) {
			points.erase(ii(pts[p[l]].y, p[l]));
			l++;
		}
		long long delta = sqrt(dist) + 1;
		auto itl = points.lower_bound(ii(pts[p[r]].y - delta, -1));
		auto itr = points.upper_bound(ii(pts[p[r]].y + delta, n + 1));
		for(auto it = itl; it != itr; it++) {
			long long curDist = sqrDist(pts[p[r]] - pts[it->second]);
			if(curDist < dist) {
				dist = curDist;
				ans = ii(p[r], it->second); 
			}
		}
		points.insert(ii(pts[p[r]].y, p[r]));
	}
	if(ans.first > ans.second) {
		std::swap(ans.first, ans.second);
	}
	return ans;
}