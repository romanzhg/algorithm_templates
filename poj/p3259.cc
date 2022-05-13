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

#define MAX_NODES 510
#define MAX_EDGES 5500
// Number of nodes.
int N;
int edgeCount;

int dist[MAX_NODES];

// Edge, (cost, from, to).
typedef pair<int, pair<int, int> > Edge;
Edge edges[MAX_EDGES];


/*
The Floyd-Washall methods TLE, because MAX_NODES is 500 while MAX_EDGES is 5200.
The Floyd-Washall algorithm will run in 500 ^ 3.
The Bellman-Ford algorithm will run in 500 * 5200.
p3259_alt.cc contains the Floyd-Washall implementation.
*/

// Bellman-Ford negative cycle detection.
// Note the distance array is initialized in a special way.
void solve() {
  fill(dist, dist + MAX_NODES, 0);
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < edgeCount; j++) {
      Edge& e = edges[j];
      if (dist[e.second.second] > dist[e.second.first] + e.first) {
        dist[e.second.second] = dist[e.second.first] + e.first;

        if (i == (N - 1)) {
          printf("YES\n");
          return;
        }
      }
    }
  }
  printf("NO\n");
}

int main() {
  int testCases;
  scanf("%d", &testCases);
  for (int testCase = 0; testCase < testCases; testCase++) {
    edgeCount = 0;

  	int M, W;
  	scanf("%d %d %d", &N, &M, &W);

  	// Number of bidirectional path.
  	for (int i = 0; i < M; i++) {
  		int from, to, time;
  		scanf("%d %d %d", &from, &to, &time);
      edges[edgeCount++] = make_pair(time, make_pair(from - 1, to - 1));
      edges[edgeCount++] = make_pair(time, make_pair(to - 1, from - 1));
  	}

  	// Number of directional path.
  	for (int i = 0; i < W; i++) {
  		int from, to, time;
  		scanf("%d %d %d", &from, &to, &time);
      edges[edgeCount++] = make_pair(-time, make_pair(from - 1, to - 1));
  	}
  	solve();
  }
  return 0;
}