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

#define MAX_COWS 110
#define MAX_FOODS 110
#define MAX_DRINKS 110

// The code is structured in this way:
// Data for module0.
// Data for module1.
// Code for module1.
// Code for module0.
// An inner module interleaved between the data and code of the outer module.
// Becase normally it needs the outer data.

// Global data.
// Number of cows, types of foods, types of drinks.
int N, F, D;
// The relationship between food and cow.
vector<int> foods[MAX_FOODS];
// The relationship between drink and cow.
vector<int> drinks[MAX_DRINKS];

// Data for solve.
#define MAX_NODES 500 // 110 * 4 + 2

struct Edge {
	// Node this edge goes to, cap of edge, index of the reverse edge.
	int to, cap, rev;
};

vector<Edge> graph[MAX_NODES];

// Data for max flow
// A used array for dfs.
bool used[MAX_NODES];

int dfs(int sourceNode, int destNode, int flow) {
	if (sourceNode == destNode) {
		return flow;
	}
	used[sourceNode] = true;

	// Examine all edges going out from source.
	for (int i = 0; i < graph[sourceNode].size(); i++) {
		Edge& e = graph[sourceNode][i];
		if (e.cap > 0 && (used[e.to] == false)) {
			int tmp_flow = dfs(e.to, destNode, min(e.cap, flow));
			if (tmp_flow > 0) {
				e.cap -= tmp_flow;
				graph[e.to][e.rev].cap += tmp_flow;
				return tmp_flow;
			}
		}
	}
	return 0;
}

int max_flow(int sourceNode, int destNode) {
	// The total flow to return.
	int rtn = 0;

	while (true) {
		// dfs returns if a path exists. if true, prevv and preve is udpated.
		memset(used, false, sizeof(used));

		int flow_volumn = dfs(sourceNode, destNode, INF);
		if (flow_volumn == 0) {
			return rtn;
		}
		rtn += flow_volumn;
	}
}

void addEdge(int from, int to, int cap) {
	Edge e;
	e.to = to;
	e.cap = cap;
	e.rev = graph[to].size();
  graph[from].push_back(e);

  e.to = from;
  e.cap = 0;
  e.rev = graph[from].size() - 1;
  graph[to].push_back(e);
}

void solve() {
  // Build the graph.
  int sourceNode = 2 * N + F + D;
  int destNode = sourceNode + 1;
  // Node 0 ~ N - 1, N ~ 2 * N - 1, the cows.
  // Add an edge and capacity 1 between them.
  for (int i = 0; i < N; i++) {
  	addEdge(i, i + N, 1);
  }

  // Foods has base 2N.
  for (int i = 0; i < F; i++) {
  	for (int j = 0; j < foods[i].size(); j++) {
  		addEdge(2 * N + i, foods[i][j], 1);
  	}
  	addEdge(sourceNode, 2 * N + i, 1);
  }

	// Drinks has base 2N + F.
  for (int i = 0; i < D; i++) {
  	for (int j = 0; j < drinks[i].size(); j++) {
  		addEdge(drinks[i][j] + N, 2 * N + F + i, 1);
  	}
  	addEdge(2 * N + F + i, destNode, 1);
  }
  
  printf("%d\n", max_flow(sourceNode, destNode));
}

int main() {
	while(scanf("%d %d %d", &N, &F, &D) != EOF) {
		for (int i = 0; i < N; i++) {
			int numberOfFoods, numberOfDrinks;
			scanf("%d %d", &numberOfFoods, &numberOfDrinks);
			for (int j = 0; j < numberOfFoods; j++) {
				int tmp;
				scanf("%d", &tmp);
				foods[tmp - 1].push_back(i);
			}
			for (int j = 0; j < numberOfDrinks; j++) {
				int tmp;
				scanf("%d", &tmp);
				drinks[tmp - 1].push_back(i);
			}
		}
		solve();
	}

  return 0;
}