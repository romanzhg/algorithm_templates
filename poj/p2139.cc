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

#define MAXN 310
int dist[MAXN][MAXN];
#define INF 100000000
int N, M;

// Floyd-Washall
void solve() {
  for (int k = 0; k < N; k++) {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
      }
    }
  }
  int minSum = INT_MAX;
  for (int i = 0; i < N; i++) {
    int sum = 0;
    for (int j = 0; j < N; j++) {
      sum += dist[i][j];
    }
    if (minSum > sum) {
      minSum = sum;
    }
  }
  minSum = minSum * 100 / (N - 1);
  printf("%d\n", minSum);
}

int main() {
  while (scanf("%d %d", &N, &M) != EOF) {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        dist[i][j] = INF;
      }
    }
    for (int i = 0; i < M; i++) {
      int tmpCows;
      scanf("%d", &tmpCows);
      vector<int> cowSet;
      for (int j = 0; j < tmpCows; j++) {
        int tmp;
        scanf("%d", &tmp);
        cowSet.push_back(tmp - 1);
      }
      for (int j = 0; j < tmpCows; j++) {
        for (int k = 0; k < tmpCows; k++) {
          dist[cowSet[j]][cowSet[k]] = 1;
        }
      }
    }
    for (int i = 0; i < N; i++ ) {
      dist[i][i] = 0;
    }
    solve();
  }
  return 0;
}
