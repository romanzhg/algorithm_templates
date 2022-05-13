/**
 * Details about this code can be improved, but the basic idea is the same as the
 * solutions.
 *
 */

#include "common.h"

bool visited[10010];
class Solution {
  // Length in nodes, to get length in edges need to decrese this value by 1.
  int max_len = 1;
  map<int, vector<int>> adj_list;

  // Return the lenght of the longest path(in node count) starting from @node.
  int dfs(int node) {
    if (visited[node]) {
      return 0;
    }
    visited[node] = true;

    // TODO: could use largest/second largest instead.
    vector<int> tmp_len;
    for (int n : adj_list[node]) {
      tmp_len.push_back(dfs(n));
    }

    sort(tmp_len.begin(), tmp_len.end());
    if (tmp_len.size() >= 2) {
      max_len = max(max_len, 1 + *tmp_len.rbegin() + *(tmp_len.rbegin() + 1));
    }
    if (tmp_len.size() == 0) {
      return 1;
    } else {
      return *tmp_len.rbegin() + 1;
    }
  }

 public:
  int treeDiameter(vector<vector<int>>& edges) {
    memset(visited, 0, sizeof(visited));

    adj_list.clear();
    for (const vector<int>& edge : edges) {
      adj_list[edge[0]].push_back(edge[1]);
      adj_list[edge[1]].push_back(edge[0]);
    }

    max_len = max(max_len, dfs(0));
    return max_len - 1;
  }
};