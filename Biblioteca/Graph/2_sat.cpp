#include <vector>
#include <stack>
#include <utility>
#include <cstring>

typedef std::pair<int, int> ii;

const int mv = 100100;

namespace sat2
{
	std::vector<int> edges[2 * mv][4];
	int step[2 * mv];
	int component[2 * mv];
	int degree[2 * mv];
	int cur_step;

	void init()
	{
		memset(step, 0, sizeof step);
		cur_step = 0;
	}

	void dfs0(int on, std::stack<int> &s)
	{
		if(step[on] == cur_step)
			return;
		step[on] = cur_step;
		for(int i = 0; i < edges[on][0].size(); i++)
		{
			int to = edges[on][0][i];
			dfs0(to, s);
		}
		s.push(on);
	}

	void dfs1(int on, int num)
	{
		if(step[on] == cur_step)
			return;
		step[on] = cur_step;
		component[on] = num;
		for(int i = 0; i < edges[on][1].size(); i++)
		{
			int to = edges[on][1][i];
			dfs1(to, num);
		}
	}
	
	std::vector<int> solve(int n, std::vector<ii> clause)
	{
		for(int i = 0; i < 2 * n; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				edges[i][j].clear();
			}
		}
		for(int i = 0; i < clause.size(); i++)
		{
			// x  -> 2 * |x|
			// !x -> 2 * |x| + 1
			int u = clause[i].first;
			int v = clause[i].second;
			edges[u ^ 1][0].push_back(v);
			edges[v ^ 1][0].push_back(u);
			edges[v][1].push_back(u ^ 1);
			edges[u][1].push_back(v ^ 1);
		}
		std::stack<int> s[2];
		cur_step++;
		for(int i = 0; i < 2 * n; i++)
			dfs0(i, s[0]);

		cur_step++;
		int comp = 0;
		while(!s[0].empty())
		{
			int on = s[0].top();
			s[0].pop();
			if(step[on] != cur_step)
				dfs1(on, comp++);
		}
		std::vector<int> ans;
		for(int i = 0; i < 2 * n; i += 2)
		{
			if(component[i] == component[i + 1])
				return ans;
		}
		for(int i = 0; i < 2 * n; i++)
		{
			edges[component[i]][3].push_back(component[i ^ 1]);
			for(int j = 0; j < edges[i][0].size(); j++)
			{
				int u = i;
				int v = edges[i][0][j];
				u = component[u];
				v = component[v];
				if(u == v)
					continue;
				edges[u][2].push_back(v);
				degree[v]++;
			}
		}
		cur_step++;
		for(int i = 0; i < comp; i++)
		{
			if(degree[i] == 0)
			{
				s[0].push(i);
				step[i] = cur_step;
			}
		}
		std::vector<int> c;
		c = std::vector<int> (comp, -1);
		ans = std::vector<int> (n, -1);
		while(!s[0].empty())
		{
			int on = s[0].top();
			s[1].push(on);
			s[0].pop();
			for(int i = 0; i < edges[on][2].size(); i++)
			{
				int v = edges[on][2][i];
				degree[v]--;
				if(degree[v] == 0)
					s[0].push(v);
			}
		}
		while(!s[1].empty())
		{
			int on = s[1].top();
			s[1].pop();
			if(c[on] == -1)
				c[on] = 1;
			for(int i = 0; i < edges[on][3].size(); i++)
			{
				int v = edges[on][3][i];
				c[v] = c[on] ^ 1;
			}
		}
		for(int i = 0; i < n; i++)
			ans[i] = c[component[2 * i]];
		return ans;
	}
};

#include <iostream>

int main()
{
	// 776D - codeforces
	int n, m;
	while(std::cin >> n >> m)
	{
		std::vector<int> a(n, 0);
		std::vector<ii> b(n, ii(m, m));
		for(int i = 0; i < n; i++)
			std::cin >> a[i];
		for(int i = 0; i < m; i++)
		{
			int s;
			std::cin >> s;
			while(s--)
			{
				int t;
				std::cin >> t;
				t--;
				if(b[t].first == m)
					b[t].first = i;
				else
					b[t].second = i;
			}
		}
		std::vector<ii> c;
		for(int i = 0; i < n; i++)
		{
			int u = 2 * b[i].first;
			int v = 2 * b[i].second;
			if(a[i])
				u++;
			c.push_back(ii(u, v));
			c.push_back(ii(u ^ 1, v ^ 1));
		}
		c.push_back(ii(2 * m + 1, 2 * m + 1));
		if(sat2::solve(m + 1, c).size() == 0)
			std::cout << "NO\n";
		else
			std::cout << "YES\n";
	}
}