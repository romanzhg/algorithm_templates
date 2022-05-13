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

#define MAXN 510

// Shortest distance, originally it contains only edges.
int dist[MAXN][MAXN];
#define INF 100000000

// Number of nodes.
int N;

// All pair shortest path.
void solve() {
  for (int k = 0; k < N; k++) {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
      }
    }
  }

  bool existNegativeLoop = false;
  for (int i = 0; i < N; i++) {
    if (dist[i][i] < 0) {
      existNegativeLoop = true;
    }
  }

  if (existNegativeLoop) {
    printf("YES\n");
  } else {
    printf("NO\n");
  }
}

int main() {
  int testCases;
  scanf("%d", &testCases);
  for (int testCase = 0; testCase < testCases; testCase++) {
  	int M, W;
  	scanf("%d %d %d", &N, &M, &W);

    // Initialize dist.
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        dist[i][j] = INF;
      }
    }
    for (int i = 0; i < N; i++) {
      dist[i][i] = 0;
    }

  	// Number of bidirectional path.
  	for (int i = 0; i < M; i++) {
  		int from, to, time;
  		scanf("%d %d %d", &from, &to, &time);
  		dist[from - 1][to - 1] = min(time, dist[from - 1][to - 1]);
  		dist[to - 1][from - 1] = min(time, dist[to - 1][from - 1]);
  	}

  	// Number of directional path.
  	for (int i = 0; i < W; i++) {
  		int from, to, time;
  		scanf("%d %d %d", &from, &to, &time);
  		dist[from - 1][to - 1] = min(-time, dist[from - 1][to - 1]);
  	}
  	solve();
  }
  return 0;
}