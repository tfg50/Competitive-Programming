Segment getBisector(PT a, PT b) {
	Segment ans(a, b);
	std::swap(ans.a, ans.b);
	ans.b *= -1;
	ans.c = ans.a * (a.x + b.x) * 0.5 + ans.b * (a.y + b.y) * 0.5;
	return ans;
}

std::vector<PT> getCell(std::vector<PT> pts, int i) {
	std::vector<PT> ans;
	ans.emplace_back(0, 0);
	ans.emplace_back(1e6, 0);
	ans.emplace_back(1e6, 1e6);
	ans.emplace_back(0, 1e6);
	for(int j = 0; j < (int) pts.size(); j++) {
		if(j != i) {
			ans = cutPolygon(ans, getBisector(pts[i], pts[j]));
		}
	}
	// BE CAREFUL!
	// the first point may be any point
	return ans;
}