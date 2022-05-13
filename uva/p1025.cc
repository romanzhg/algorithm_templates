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

#define MAXN 60
#define MAXT 1500

int numberOfStations;
int targetTime;
int travelTime[MAXN];

// graph[t][n]: at time t, is there a train going left/right from station n.
// This is effectively an encoded adjacent list.
int graph[MAXT][MAXN];

// dp[t][n], min waiting time to be in station n at time t.
// To achieve state [t][n], there are three ways, wait in station n, took a train
// from left, took a train from right(taking a train does not add to the wait time).
// The train can be taken only if such an edge exists.
int dp[MAXT][MAXN];

void setRight(int& v) {
  v = v | (1U << 0);
}
void setLeft(int& v) {
  v = v | (1U << 1);
}
bool hasRight(int v) {
  return v & (1U << 0);
}
bool hasLeft(int v) {
  return v & (1U << 1);
}

int main() {
  int testCase = 0;
  while (scanf("%d", &numberOfStations) == 1 && numberOfStations != 0) {
    // Initialize.
    memset(graph, 0, sizeof(graph));
    for (int t = 0; t < MAXT; t++) {
      for (int s = 0; s < MAXN; s++) {
        dp[t][s] = INF;
      }
    }
    memset(travelTime, 0, sizeof(travelTime));

    scanf("%d", &targetTime);
    for (int i = 1; i < numberOfStations; i++) {
      scanf("%d", &travelTime[i]);
    }

    int m1;
    scanf("%d", &m1);
    for (int i = 0; i < m1; i++) {
      int departureTime;
      scanf("%d", &departureTime);

      // At the last station, there is no train going right.
      for (int j = 1; j < numberOfStations; j++) {
        departureTime += travelTime[j - 1];
        setRight(graph[departureTime][j]);
      }
    }

    int m2;
    scanf("%d", &m2);
    for (int i = 0; i < m2; i++) {
      int departureTime;
      scanf("%d", &departureTime);

      for (int j = numberOfStations; j > 1; j--) {
        departureTime += travelTime[j];
        setLeft(graph[departureTime][j]);
      }
    }

    dp[0][1] = 0;
    for (int t = 1; t <= targetTime; t++) {
      for (int s = 1; s <= numberOfStations; s++) {
        // Stay in current station.
        dp[t][s] = min(dp[t][s], dp[t - 1][s] + 1);

        // Took a train from a station on left(the train was going right).
        int timeFromLeft = travelTime[s - 1];
        if (s > 1
          && t - timeFromLeft >= 0
          && hasRight(graph[t - timeFromLeft][s - 1])) {
          dp[t][s] = min(dp[t][s], dp[t - timeFromLeft][s - 1]);
        }

        // Took a train from a station on right(the train was going left).
        int timeFromRight = travelTime[s];
        if (s < numberOfStations
          && t - timeFromRight >= 0
          && hasLeft(graph[t - timeFromRight][s + 1])) {
          dp[t][s] = min(dp[t][s], dp[t - timeFromRight][s + 1]);
        }
      }
    }
    if (dp[targetTime][numberOfStations] == INF) {
      printf("Case Number %d: impossible\n", ++testCase);
    } else {
      printf("Case Number %d: %d\n", ++testCase, dp[targetTime][numberOfStations]);
    }
  }
  return 0;
}