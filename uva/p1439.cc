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

#define MAX_NODES 15
int edgeCount;
int nodesCount;
map<char, int> nodeToIndex;

// dp[S], minimum colors to print a graph subset represented by S.
int dp[1 << MAX_NODES];
bool noEdgesInside[1 << MAX_NODES];
int ops[1 << MAX_NODES];

// set the bit if another node is adjacent to this one.
int adjNodes[MAX_NODES];
vector<pair<char, char>> edges;

bool isBitSet(uint32_t value, uint32_t n) {
  return value & (1U << n);
}

uint32_t setBit(uint32_t value, int32_t n) {
  return value | (1U << n);
}

void fillNoEdgesInside() {
  memset(noEdgesInside, 1, 1 << nodesCount);
  for (int S = 1; S < (1 << nodesCount); S++) {
    for (int i = 0; i < nodesCount; i++) {
      if (isBitSet(S, i)) {
        if (S & adjNodes[i]) {
          noEdgesInside[S] = false;
          break;
        }
      }
    }
  }
}

int main() {
  while (scanf("%d", &edgeCount) == 1) {
    nodesCount = 0;
    memset(adjNodes, 0, sizeof(adjNodes));
    edges.clear();
    nodeToIndex.clear();
    for (int i = 0; i < edgeCount; i++) {
      char from, to;
      scanf(" %c %c", &from, &to);
      if (nodeToIndex.find(from) == nodeToIndex.end()) {
        nodeToIndex[from] = nodesCount;
        nodesCount++;
      }
      if (nodeToIndex.find(to) == nodeToIndex.end()) {
        nodeToIndex[to] = nodesCount;
        nodesCount++;
      }

      adjNodes[nodeToIndex[from]] =
        setBit(adjNodes[nodeToIndex[from]], nodeToIndex[to]);
      adjNodes[nodeToIndex[to]] =
        setBit(adjNodes[nodeToIndex[to]], nodeToIndex[from]);

      edges.push_back({from, to});
    }

    fillNoEdgesInside();

    // Get the min coloring on the graph.
    dp[0] = 0;
    // Iterate through all subsets of (1 << nodesCount) - 1, name it S.
    for (int S = 1; S < (1 << nodesCount); S++) {
      dp[S] = INF;
      // Iterate through all subsets of S.
      for (int S0 = S; S0; S0 = (S0 - 1) & S) {
        if (noEdgesInside[S0]) {
          if ((dp[S - S0] + 1) < dp[S]) {
            dp[S] = dp[S - S0] + 1;
            ops[S] = S0;
          }
        }
      }
    }

    vector<int> path;
    int cur = (1 << nodesCount) - 1;
    while (cur != 0) {
      path.push_back(ops[cur]);
      cur = cur - ops[cur];
    }

    printf("%d\n", dp[(1 << nodesCount) - 1] - 2);
    for (int i = 0; i < edgeCount; i++) {
      int n1 = nodeToIndex[edges[i].first];
      int n2 = nodeToIndex[edges[i].second];
      int n1Index, n2Index;
      for (int j = 0; j < path.size(); j++) {
        if (isBitSet(path[j], n1)) {
          n1Index = j;
        }
        if (isBitSet(path[j], n2)) {
          n2Index = j;
        }
      }
      if (n1Index > n2Index) {
        printf("%c %c\n", edges[i].first, edges[i].second);
      } else {
        printf("%c %c\n", edges[i].second, edges[i].first);
      }
    }
  }
  return 0;
}