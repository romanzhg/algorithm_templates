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
// unordered_map is not supported.
using namespace std;

#define MAXN 101
int N;
int adjMatrix [MAXN][MAXN];
bool used[MAXN];
int distSet[MAXN];

// Minimum spanning tree.
// because the input is adjMatrix, we don't need union find to solve this problem(Kruskal).
// instead, apply Prim algorithm, keep a used set and update the the distance from this set to un-used nodes.
void solve() {
  // Init data structures.
  fill(used, used + MAXN, false);
  int totalCost = 0;

  // Init.
  used[0] = true;
  for (int i = 0; i < N; i++) {
    distSet[i] = adjMatrix[0][i];
  }
  
  // Run the outer loop N-1 times.
  for (int k = 0; k < N - 1; k++) {
    // Get min element.
    int closestNode;
    int tmpMin = INT_MAX;
    for (int i = 0; i < N; i++) {
      if (!used[i] && tmpMin > distSet[i]) {
        tmpMin = distSet[i];
        closestNode = i;
      }
    }
    // Update the cost.
    totalCost += tmpMin;
    
    // Update distSet.
    for (int i = 0; i < N; i++) {
      distSet[i] = min(distSet[i], adjMatrix[closestNode][i]);
    }
    used[closestNode] = true;
  }
  printf("%d\n", totalCost);
}

int main() {
  while (scanf("%d", &N) != EOF) {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        scanf("%d", &adjMatrix[i][j]);
      }
    }
    solve();
  }
  return 0;
}
