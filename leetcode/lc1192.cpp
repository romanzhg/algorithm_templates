/**
 * Find bridge in graph.
 */

#include "common.h"
const int kMaxNodes = 1e5 + 1;
class Solution {
 public:
  int nodes;
  vector<int> graph[kMaxNodes];
  int pre[kMaxNodes];
  int low[kMaxNodes];
  bool isCut[kMaxNodes];
  int dfsClock = 0;

  vector<vector<int>> rtn;

  int dfs(int u, int parent) {
    int lowu = pre[u] = ++dfsClock;
    int childCount = 0;
    for (int i = 0; i < graph[u].size(); i++) {
      int v = graph[u][i];
      if (!pre[v]) {
        childCount++;
        int lowv = dfs(v, u);
        lowu = min(lowu, lowv);
        if (lowv >= pre[u]) {
          isCut[u] = true;
        }
        if (lowv > pre[u]) {
          rtn.push_back({u, v});
        }
      } else if (pre[v] < pre[u] && v != parent) {
        // Reverse edge. v already visited.
        lowu = min(lowu, pre[v]);
      }
    }
    if (parent < 0 && childCount == 1) {
      isCut[u] = false;
    }
    low[u] = lowu;
    return lowu;
  }

  vector<vector<int>> criticalConnections(int n, vector<vector<int>>& connections) {
    // Setups.
    nodes = n;
    for (int i = 0; i < nodes; i++) {
      graph[i].clear();
    }
    memset(pre, 0, sizeof(pre));
    memset(low, 0, sizeof(low));
    memset(isCut, 0, sizeof(isCut));
    rtn.clear();

    // Load input.
    for (const vector<int>& c : connections) {
      graph[c[0]].push_back(c[1]);
      graph[c[1]].push_back(c[0]);
    }

    dfs(0, -1);
    return rtn;
  }
};