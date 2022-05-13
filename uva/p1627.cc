// Form the knowing graph build the not knowing graph.
// Color the nodes in the not knowing graph, adj nodes have different colors.
// Nodes with the same color is to be in the same set.
// Find a way to put elements into two teams(maintain the diff) so the
// sum is close to 0.
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

#define MAX_PEOPLE 105

int peopleCount;
bool knows[MAX_PEOPLE][MAX_PEOPLE];
bool notKnowing[MAX_PEOPLE][MAX_PEOPLE];

// Visit the connected component of node i, update the connected
// components set.
void dfs(int node, bool* visited, set<int>& cc) {
  if (visited[node]) {
    return;
  }
  cc.insert(node);
  visited[node] = true;
  for (int i = 0; i < peopleCount; i++) {
    if (notKnowing[node][i]) {
      dfs(i, visited, cc);
    }
  }
}

// Return if it is possible to print each connected component in the
// nowKnowing graph to two colors, with adj element having
// different colors.
bool dfsColoring(int node, bool inVisited0, bool* visited0, bool* visited1) {
  if (inVisited0) {
    if (visited0[node]) {
      return true;
    }
    if (visited1[node]) {
      return false;
    }
  } else {
    if (visited1[node]) {
      return true;
    }
    if (visited0[node]) {
      return false;
    }
  }

  if (inVisited0) {
    visited0[node] = true;
  } else {
    visited1[node] = true;
  }

  for (int i = 0; i < peopleCount; i++) {
    if (i == node) {
      continue;
    }
    if (notKnowing[node][i]) {
      if (!dfsColoring(i, !inVisited0, visited0, visited1)) {
        return false;
      }
    }
  }
  return true;
}

void gatherOps(bool op, int index, const vector<set<int>>& cc,
  bool* visited0, bool* visited1, set<int>& elems0, set<int>& elems1) {
  for (int v : cc[index]) {
    if (visited0[v]) {
      if (op) {
        elems0.insert(v);
      } else {
        elems1.insert(v);
      }
    } else {
      if (op) {
        elems1.insert(v);
      } else {
        elems0.insert(v);
      }
    }
  }
}

void printSet(const set<int>& s) {
  printf("%d ", (int)s.size());
  for (int e : s) {
    printf("%d ", e + 1);
  }
  printf("\n");
}

int main() {
  int testCases;
  scanf("%d", &testCases);

  bool visited[MAX_PEOPLE];
  bool visited0[MAX_PEOPLE];
  bool visited1[MAX_PEOPLE];

  bool isFirstCase = true;
  while(testCases--) {

    if (isFirstCase) {
      isFirstCase = false;
    } else {
      printf("\n");
    }

    scanf("%d", &peopleCount);

    memset(knows, 0, sizeof(knows));
    memset(notKnowing, 0, sizeof(notKnowing));
    
    for (int i = 0; i < peopleCount; i++) {
      int k;
      while (scanf("%d", &k) == 1 && k != 0) {
        knows[i][k - 1] = true;
      }
    }

    for (int i = 0; i < peopleCount; i++) {
      for (int j = 0; j < peopleCount; j++) {
        if (i == j) {
          continue;
        }
        if (!knows[i][j]) {
          notKnowing[i][j] = true;
          notKnowing[j][i] = true;
        }
      }
    }

    memset(visited, 0, sizeof(visited));
    vector<set<int>> connectedComponent;
    for (int i = 0; i < peopleCount; i++) {
      if (!visited[i]) {
        set<int> tmpCC;
        dfs(i, visited, tmpCC);
        connectedComponent.push_back(tmpCC);
      }
    }

    // Check connected components. If any of them failed,
    // then this problem has no solution.
    // Print the elements to two colors in a dfs way, find any conflict.
    memset(visited0, 0, sizeof(visited0));
    memset(visited1, 0, sizeof(visited1));
    bool hasSolution = true;
    for (const set<int> cc : connectedComponent) {
      if (!dfsColoring(*(cc.begin()), true, visited0, visited1)) {
        hasSolution = false;
        break;
      }
    }

    if (!hasSolution) {
      printf("No solution\n");
      continue;
    }

    vector<int> maxDiff(connectedComponent.size());
    for (int i = 0; i < connectedComponent.size(); i++) {
      int size0 = 0, size1 = 0;
      for (int elem : connectedComponent[i]) {
        if (visited0[elem]) {
          size0++;
        } else {
          size1++;
        }
      }
      maxDiff[i] = size0 - size1;
    }

    int maxV = 0;
    for (int v : maxDiff) {
      maxV += abs(v);
    }

    int valueRange = 2 * maxV + 1;
    bool dp[connectedComponent.size() + 1][valueRange];
    bool ops[connectedComponent.size() + 1][valueRange];
    int path[connectedComponent.size() + 1][valueRange];

    memset(dp, 0, sizeof(dp));
    memset(ops, 0, sizeof(ops));
    memset(path, 0, sizeof(path));

    dp[0][0 + maxV] = true;
    for (int i = 0; i < connectedComponent.size(); i++) {
      for (int j = 0; j < valueRange; j++) {
        if (dp[i][j]) {
          int diff = maxDiff[i];
          dp[i + 1][j + diff] = true;
          dp[i + 1][j - diff] = true;
          path[i + 1][j + diff] = j;
          path[i + 1][j - diff] = j;
          ops[i + 1][j + diff] = true;
          ops[i + 1][j - diff] = false;
        }
      }
    }

    int minDist = INF;
    int minDistIndex;
    for (int j = 0; j < valueRange; j++) {
      if (dp[connectedComponent.size()][j]) {
        if (abs(j - maxV) < minDist) {
          minDist = abs(j - maxV);
          minDistIndex = j;
        }
      }
    }

    set<int> elems0;
    set<int> elems1;
    for (int i = connectedComponent.size(); i > 0; i--) {
      bool op = ops[i][minDistIndex];
      gatherOps(op, i - 1, connectedComponent, visited0, visited1,
        elems0, elems1);
      minDistIndex = path[i][minDistIndex];
    }
    printSet(elems0);
    printSet(elems1);
  }
  return 0;
}