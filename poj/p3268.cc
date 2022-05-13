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

#define MAX_EDGES 100010
#define MAX_NODES 1010
#define INF 100000000

typedef pair<int, pair<int, int> > Edge;

Edge edges[MAX_EDGES];
Edge reversedEdges[MAX_EDGES];

int distJoin[MAX_NODES];
int distBack[MAX_NODES];

int N, M, X;

void solve() {
  // Get the shortest distance from each from to farm X. Solve with reversed edges.
  fill(distJoin, distJoin + MAX_NODES, INF);
	distJoin[X] = 0;
	while (true) {
		bool updated = false;
		for (int i = 0; i < M; i++) {
			Edge& e = reversedEdges[i];
      if (distJoin[e.second.second] > distJoin[e.second.first] + e.first) {
        distJoin[e.second.second] = distJoin[e.second.first] + e.first;
        updated = true;
      }
		}
		if (!updated) {
			break;
		}
	}

  // Get the shortest distance form X to each farm. Solve with edges.
  fill(distBack, distBack + MAX_NODES, INF);
	distBack[X] = 0;
	while (true) {
		bool updated = false;
		for (int i = 0; i < M; i++) {
			Edge& e = edges[i];
      if (distBack[e.second.second] > distBack[e.second.first] + e.first) {
        distBack[e.second.second] = distBack[e.second.first] + e.first;
        updated = true;
      }
		}
		if (!updated) {
			break;
		}
	}
	int result = INT_MIN;
	for (int i = 0; i < N; i++) {
		int tmp = distJoin[i] + distBack[i];
		if (result < tmp) {
			result = tmp;
		}
	}
	printf("%d\n", result);
}

int main() {
	while (scanf("%d %d %d", &N, &M, &X) != EOF) {
		X = X - 1;
		for (int i = 0; i < M; i++) {
			int from, to, cost;
			scanf("%d %d %d", &from, &to, &cost);
			edges[i] = make_pair(cost, make_pair(from - 1, to - 1));
			reversedEdges[i] = make_pair(cost, make_pair(to - 1, from - 1));
		}
		solve();
	}
  return 0;
}