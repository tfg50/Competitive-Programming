#include <vector>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>

class EllysChessboard {
public:
	int getDist(std::pair<int, int> a, std::pair<int, int> b) {
		int dx = a.first - b.first;
		int dy = a.second - b.second;
		dx = abs(dx), dy = abs(dy);
		//std::cout << "dist (" << a.first << ", " << a.second << "), (" << b.first << ", " << b.second << ") is " << std::max(dx, dy) << std::endl;
		return std::max(dx, dy);
	}
	int minCost(std::vector<std::string> board) {
		std::vector<std::pair<int, int>> points;
		for(int i = 0; i < 8; i++) {
			for(int j = 0; j < 8; j++) {
				if(board[i][j] == '#') {
					points.emplace_back(i - j + 7, i + j);
					//std::cout << "found point in (" << i-j+7 << ", " << i + j << ")!\n";
				}
			}
		}
		const int ms = 16;
		int dp[ms][ms][ms][ms];
		memset(dp, 0x3f, sizeof dp);
		if(points.size() == 0) return 0;
		for(int i = 0; i < points.size(); i++) {
			std::pair<int, int> e = points[i];
			dp[e.first][e.first][e.second][e.second] = 0;
		}
		int ans = 1e9;
		int t = 0;
		int visit[ms][ms][ms][ms];
		memset(visit, 0, sizeof visit);
		for(int xl = ms - 1; xl >= 0; xl--) {
			for(int xr = xl; xr < ms; xr++) {
				for(int yl = ms - 1; yl >= 0; yl--) {
					for(int yr = yl; yr < ms; yr++) {
						if(dp[xl][xr][yl][yr] > 1000) continue;
						//std::cout << "cost for (" << xl << ", " << xr << "), (" << yl << ", " << yr << ") = " << dp[xl][xr][yl][yr] << std::endl;
						bool valid = true;
						std::vector<std::pair<int, int>> inside;
						for(int i = 0; i < points.size(); i++) {
							std::pair<int, int> pt = points[i];
							if(xl <= pt.first && pt.first <= xr && yl <= pt.second && pt.second <= yr) continue;
							valid = false;
							inside.push_back(pt);
						}
						inside.swap(points);
						t++;
						for(int i = 0; i < points.size(); i++) {
							std::pair<int, int> pt = points[i];
							if(xl <= pt.first && pt.first <= xr && yl <= pt.second && pt.second <= yr) continue;
							valid = false;
							int cost = 0;
							int x1 = std::min(xl, pt.first), x2 = std::max(xr, pt.first);
							int y1 = std::min(yl, pt.second), y2 = std::max(yr, pt.second);
							if(visit[x1][x2][y1][y2] == t) continue;
							visit[x1][x2][y1][y2] = t;
							for(int j = 0; j < points.size(); j++) {
								pt = points[j];
								if(xl <= pt.first && pt.first <= xr && yl <= pt.second && pt.second <= yr) continue;
								if(x1 <= pt.first && pt.first <= x2 && y1 <= pt.second && pt.second <= y2) {
									//std::cout << "using point " << j << std::endl;
									cost += std::max(getDist(pt, {x1, y1}), getDist(pt, {x2, y2}));
								}
							}
							pt = points[i];
							//std::cout << "going from (" << xl << ", " << xr << "), (" << yl << ", " << yr << ") to (" << x1 << ", " << x2 << "), (" << y1 << ", " << y2 << ") with cost " << cost << std::endl;
							dp[x1][x2][y1][y2] =
								std::min(dp[x1][x2][y1][y2], dp[xl][xr][yl][yr] + cost);
						}
						inside.swap(points);
						if(valid) {
							ans = std::min(ans, dp[xl][xr][yl][yr]);
						}
					}
				}
			}
		}
		return ans;
	}
};
