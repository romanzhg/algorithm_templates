/**
 * Print all pathes in dag. memory usage is max(path_len) ^ 2.
 */
#include "common.h"

class Solution {
  vector<vector<int>> rtn;

  void helper(int node, vector<int> prev, const vector<vector<int>>& graph) {
    prev.push_back(node);
    if (node == (graph.size() - 1)) {
      rtn.push_back(prev);
      return;
    }
    for (int n : graph[node]) {
      helper(n, prev, graph);
    }
  }
 public:
  vector<vector<int>> allPathsSourceTarget(vector<vector<int>>& graph) {
    rtn.clear();
    helper(0, {}, graph);
    return rtn;
  }
};