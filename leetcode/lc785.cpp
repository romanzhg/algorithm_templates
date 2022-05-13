/**
 * Bipartite coloring with dfs.
 */
#include "common.h"

class Solution {
 public:
  int graph_size;

  // Returns true if the graph can be colored correctly.
  bool dfs(int node, int c, const vector<vector<int>>& graph, vector<int>& color) {
    if (color[node] == c) {
      return true;
    }
    if (color[node] != 0) {
      return false;
    }
    color[node] = c;
    for (int next : graph[node]) {
      if (!dfs(next, 3 - c, graph, color)) {
        return false;
      }
    }
    return true;
  }

  bool isBipartite(vector<vector<int>>& graph) {
    graph_size = graph.size();
    vector<int> color(graph_size, 0);

    for (int i = 0; i < graph_size; i++) {
      if (color[i] == 0) {
        if (!dfs(i, 1, graph, color)) {
          return false;
        }
      }
    }
    return true;
  }
};