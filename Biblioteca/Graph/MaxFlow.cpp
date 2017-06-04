#include <vector>
#include <queue>

struct Edge
{
	Edge(int a, int b){to = a;cap = b;}
	int to, cap;
};

class Dinic
{
public:
	Dinic(int n)
	{
		edges.resize(n);
		this->n = n;
	}

	int maxFlow(int src, int sink)
	{
		int ans = 0;
		while(bfs(src, sink))
		{
			int flow;
			pt = std::vector<int>(n, 0);
			while(flow = dfs(src, sink))
				ans += flow;
		}
		return ans;
	}

	void addEdge(int from, int to, int cap)
	{
		edges[from].push_back(list.size());
		list.push_back(Edge(to, cap));
		edges[to].push_back(list.size());
		list.push_back(Edge(from, 0));
	}

private:
	int n;
	std::vector<std::vector<int> > edges;
	std::vector<Edge> list;
	std::vector<int> h, pt;

	int dfs(int on, int sink, int flow = 1e9)
	{
		if(flow == 0)
			return 0;
		if(on == sink)
			return flow;
		for(; pt[on] < edges[on].size(); pt[on]++)
		{
			int cur = edges[on][pt[on]];
			if(h[on] + 1 != h[list[cur].to])
				continue;
			int got = dfs(list[cur].to, sink, std::min(flow, list[cur].cap));
			if(got)
			{
				list[cur].cap -= got;
				list[cur ^ 1].cap += got;
				return got;
			}
		}
		return 0;
	}

	bool bfs(int src, int sink)
	{
		h = std::vector<int>(n, n);
		h[src] = 0;
		std::queue<int> q;
		q.push(src);
		while(!q.empty())
		{
			int on = q.front();
			q.pop();
			for(auto a:edges[on])
			{
				if(list[a].cap == 0)
					continue;
				int to = list[a].to;
				if(h[to] > h[on] + 1)
				{
					h[to] = h[on] + 1;
					q.push(to);
				}
			}
		}
		return h[sink] < n;
	}
};
