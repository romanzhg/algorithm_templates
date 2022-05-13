// Fill next states based on current state.
// This is different from one other normal implementation which fill current
// state based on previous states.
#include <cfloat>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <algorithm>
#include <deque>
#include <functional>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <utility>

using namespace std;
typedef long long ll;
#define INF 1000000000

#define MAXN 1000

int pointCount;
double points[MAXN][2];
double dp[MAXN][MAXN];

double dist(int a, int b) {
  double deltaX = points[a][0] - points[b][0];
  double deltaY = points[a][1] - points[b][1];
  return sqrt(deltaX * deltaX + deltaY * deltaY);
}

int main() {
  while(scanf("%d", &pointCount) == 1) {
    for (int i = 1; i <= pointCount; i++) {
      for (int j = 0; j < 2; j++) {
        scanf("%lf", &points[i][j]);
      }
    }

    for (int i = 0; i < MAXN; i++) {
      for (int j = 0; j < MAXN; j++) {
        dp[i][j] = INF;
      }
    }

    // The initial state. The first non starting point(point 2) has to be
    // visited by either one of the two people.
    dp[2][1] = dist(1, 2);
    // Update next metric on next state base on the current one.
    for (int i = 1; i < pointCount; i++) {
      for (int j = 1; j < i; j++) {
        dp[i + 1][j] = min(dp[i + 1][j], dp[i][j] + dist(i, i + 1));
        dp[i + 1][i] = min(dp[i + 1][i], dp[i][j] + dist(i + 1, j));
      }
    }

    for (int j = 1; j < pointCount; j++) {
      dp[pointCount][pointCount] = min(dp[pointCount][pointCount],
        dp[pointCount][j] + dist(j, pointCount));
    }
    printf("%.2lf\n", dp[pointCount][pointCount]);
  }
  return 0;
}