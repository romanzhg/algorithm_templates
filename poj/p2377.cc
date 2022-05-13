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

/*
For minimum spanning tree, the edge direction actually doesn't matter.
For Prim's algorithm, we need to fill in the adjacent matrix with edges
from both directions.
For Kurskal's algorithm, since we only process edges, we don't need
to duplicate the edges.
*/

#define MAX_NODES 1010
#define MAX_EDGES 20010

typedef pair<int, pair<int, int> > Edge;

int parent[MAX_NODES];
int rank_[MAX_NODES];

Edge edges[MAX_EDGES];

// N: nodes, M: edges.
int N, M;

int getParent(int a) {
  if (a == parent[a]) {
    return a;
  } else {
    parent[a] = getParent(parent[a]);
    return parent[a];
  }
}

void join(int a, int b) {
  if (rank_[a] == rank_[b]) {
    parent[b] = a;
    rank_[a]++;
  } else if (rank_[a] > rank_[b]) {
    parent[b] = a;
  } else {
    parent[a] = b;
  }
}

void solve() {
	// Initialize.
	for (int i = 0; i < N; i++) {
		parent[i] = i;
		rank_[i] = 0;
	}
	int totalCost = 0;

	// Kruskal.
	sort(edges, edges + M, greater<Edge>());
	int groupCount = N;

	for (int i = 0; i < M; i++) {
		Edge& e = edges[i];
		int from = e.second.first;
		int to = e.second.second;
		int cost = e.first;
		if (getParent(from) != getParent(to)) {
			join(getParent(from), getParent(to));
			groupCount--;
			totalCost += cost;
		}
	}

  // Please note the if/else condition: don't print both! 
	if (groupCount != 1) {
		printf("%d", -1);
	} else {
		printf("%d", totalCost);
	}
}

int main() {
	while (scanf("%d %d", &N, &M) != EOF) {
		for (int i = 0; i < M; i++) {
			int from, to, cost;
			scanf("%d %d %d", &from, &to, &cost);
			edges[i] = make_pair(cost, make_pair(from - 1, to - 1));
		}
		solve();
	}
	
  return 0;
}