std::vector<int> getBorder(std::string str) {
	int n = str.size();
	std::vector<int> border(n, -1);
	for(int i = 1, j = -1; i < n; i++) {
		while(j >= 0 && str[i] != str[j + 1]) {
			j = border[j];
		}
		if(str[i] == str[j + 1]) {
			j++;
		}
		border[i] = j;
	}
	return border;
}

int matchPattern(const std::string &txt, const std::string &pat, std::vector<int> &border) {
	int freq = 0;
	for(int i = 0, j = -1; i < txt.size(); i++) {
		while(j >= 0 && txt[i] != pat[j + 1]) {
			j = border[j];
		}
		if(pat[j + 1] == txt[i]) {
			j++;
		}
		if(j + 1 == pat.size()) {
			//found occurence
			freq++;
			j = border[j];
		}
	}
	return freq;
}
