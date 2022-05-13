/**
 * Topological sort(the dfs way) with a little modification.
 */
#include "common.h"

class Solution {
  map<int, set<int>> adj_list;
  vector<bool> p_mark;
  vector<bool> mark;
  vector<bool> is_safe;

 public:
  // Returns false if the node can go into a cycle.
  bool dfs(int node) {
    if (p_mark[node]) {
      return is_safe[node];
    }
    if (mark[node]) {
      return false;
    }
    mark[node] = true;
    bool tmp = true;
    for (int o : adj_list[node]) {
      tmp = tmp && dfs(o);
    }
    p_mark[node] = true;
    mark[node] = false;
    is_safe[node] = tmp;
    return tmp;
  }

  vector<int> eventualSafeNodes(vector<vector<int>>& graph) {
    int graph_size = (int) graph.size();
    for (int i = 0; i < graph_size; i++) {
      for (int o : graph[i]) {
        adj_list[i].insert(o);
      }
    }
    p_mark = vector<bool>(graph_size, false);
    mark = vector<bool>(graph_size, false);
    is_safe = vector<bool>(graph_size, false);

    for (int i = 0; i < graph_size; i++) {
      dfs(i);
    }

    vector<int> rtn;
    for (int i = 0; i < graph_size; i++) {
      if (is_safe[i]) {
        rtn.push_back(i);
      }
    }

    return rtn;
  }
};