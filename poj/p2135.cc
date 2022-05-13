#include <cstdio>
#include <cstdlib>
#include <climits>
#include <queue>
#include <vector>
#include <utility>
#include <algorithm>
#include <functional>
#include <cstring>
#include <string>
#include <map>
using namespace std;

#define INF 100000000

#define MAX_N 1010

// Number of vertexes;
int N;

struct Edge {
	int to, cap, rev, cost;
};

vector<Edge> graph[MAX_N];

void add_edge(int from, int to, int cap, int cost) {
	Edge e;
	e.to = to;
	e.cap = cap;
	e.rev = graph[to].size();
	e.cost = cost;

	graph[from].push_back(e);

  e.to = from;
  e.cap = 0;
  e.rev = graph[from].size() - 1;
  e.cost = -cost;

	graph[to].push_back(e);
}

// Data structure for min_cost_flow.
// The prev structures don't need to be initialized, since they are 
// write-and-read.
int prevv[MAX_N];
int preve[MAX_N];
int dist[MAX_N];

// void get_shortest_path(int sourceNode, int destNode) {
	
// }
// Return the minimum cost for the flow from source node to dest node. 
int min_cost_flow(int sourceNode, int destNode, int flow) {
	// The cost to return.
	int rtn = 0;

	while (flow > 0) {
		// Find a min cost path from src to dest by Bellman-Ford algorithm.
		// Record prevv and preve.
		fill(dist, dist + MAX_N, INF);

		bool updated = true;
		dist[sourceNode] = 0;
		while (updated) {
			updated = false;

			// Iterate through all the nodes.
			for (int v = 0; v < N; v++) {
				if (dist[v] == INF) {
					continue;
				}
				for (int i = 0; i < graph[v].size(); i++) {
					Edge& e = graph[v][i];
					// The edge has capacity and the source node is already reached.
					if (e.cap > 0 && (dist[e.to] > dist[v] + e.cost)) {
						dist[e.to] = dist[v] + e.cost;
						prevv[e.to] = v;
						preve[e.to] = i;
						updated = true;
					}
				}
			}
		}

		if (dist[destNode] == INF) {
			return -1;
		}
		int tmp_flow = flow;
		for (int n = destNode; n != sourceNode; n = prevv[n]) {
			tmp_flow = min(tmp_flow, graph[prevv[n]][preve[n]].cap);
		}
		rtn += tmp_flow * dist[destNode];
		flow -= tmp_flow;

		for (int n = destNode; n != sourceNode; n = prevv[n]) {
			Edge& e = graph[prevv[n]][preve[n]];
			e.cap -= tmp_flow;
			graph[n][e.rev].cap += tmp_flow;
		}
	}
	return rtn;
}

int main() {
	int M;
	while(scanf("%d %d", &N, &M) != EOF) {
		for(int i = 0; i < M; i++) {
			int from, to, cost;
			scanf("%d %d %d", &from, &to, &cost);
			// Undirected graph, need to add edges in both directions.
			add_edge(from - 1, to - 1, 1, cost);
			add_edge(to - 1, from - 1, 1, cost);;
		}
		printf("%d\n", min_cost_flow(0, N - 1, 2));
	}

  return 0;
}