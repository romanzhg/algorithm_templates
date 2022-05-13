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


#define MAX_NODES 2010
#define MAX_EDGES 10010

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
	int maxDist = INT_MIN;

	// Kruskal.
	sort(edges, edges + M, less<Edge>());

	int groupCount = N;
	for (int i = 0; i < M; i++) {
		Edge& e = edges[i];
		int from = e.second.first;
		int to = e.second.second;
		int cost = e.first;
		if (getParent(from) != getParent(to)) {
			join(getParent(from), getParent(to));
			groupCount--;
			maxDist = max(maxDist, cost);
			if (groupCount <= 1) {
				printf("%d\n", maxDist);
				return;
			}
		}
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