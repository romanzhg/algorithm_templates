/**
 * In a graph, find a path with maximum value. Value is on vertex, each vertex should
 * be visited only once.
 *
 * This should be a NPC problem, but since the size is small(25 vertexes in the graph)
 * and the connection is sparse(4 out going edges for each vertex), this can be solved by
 * brute force search.
 *
 * Note a little bug that I use keys in adj to iterate, in this case vertexes with no adj
 * are missed.
 *
 */

#include "common.h"

class Solution {
  int row_count, col_count;
  map<int, set<int>> adj;
  map<int, int> id_to_value;

  vector<pair<int, int>> kDelta = {
      {-1, 0},
      {1, 0},
      {0, -1},
      {0, 1},
  };

  int IndexToId(int i, int j) {
    return i * col_count + j;
  }

 public:
  // Returns the value of the maximum value path start at @node and won't visit nodes in @visited.
  int dfs(int node, set<int>& visited) {
    if (visited.find(node) != visited.end()) {
      return 0;
    }
    visited.insert(node);

    int tmp = 0;
    for (int v : adj[node]) {
      tmp = max(tmp, dfs(v, visited));
    }
    visited.erase(node);
    return id_to_value[node] + tmp;
  }

  int getMaximumGold(vector<vector<int>>& grid) {
    adj.clear();
    id_to_value.clear();

    row_count = grid.size();
    col_count = grid[0].size();

    for (int i = 0; i < row_count; i++) {
      for (int j = 0; j < col_count; j++) {
        if (grid[i][j] == 0) {
          continue;
        }
        id_to_value[IndexToId(i, j)] = grid[i][j];
        for (const auto& d : kDelta) {
          int new_i = i + d.first;
          int new_j = j + d.second;
          if (new_i < 0 || new_i >= row_count || new_j < 0 || new_j >= col_count) {
            continue;
          }
          if (grid[new_i][new_j] == 0) {
            continue;
          }
          adj[IndexToId(i, j)].insert(IndexToId(new_i, new_j));
        }
      }
    }

    int rtn = 0;
    for (const auto& kv : id_to_value) {
      set<int> visited;
      rtn = max(rtn, dfs(kv.first, visited));
    }
    return rtn;
  }
};