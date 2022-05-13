/**
 * Check if the graph is DAG, if yes, output the longest path.
 *
 * Both implementations use a modified version of DFS topological sort algorithm, instead of print the sequence,
 * get the length of the longest path.
 *
 * The two implementations differ in the semantic of dfs.
 * In Solution, dfs return the length of the longest path start from node.
 * In Solution1, dfs keeps track of the length of one path end at a node.
 *
 * The approach in Solution can be improved by store the previous results.
 *
 */
#include "common.h"

struct Edge {
  int from, to;
  Edge(int from, int to) : from(from), to(to) {};
};

/*****************************************************************************/


class Solution {
  vector<Edge> edges;
  vector<vector<int>> graph;
  bool mark[5010];
  int max_dist[5010];
  bool has_cycle;
 public:
  int minimumSemesters(int N, vector<vector<int>>& relations) {
    has_cycle = false;
    graph.resize(N + 1);
    for (int i = 1; i <= N; i++) {
      graph[i].clear();
    }
    for (const vector<int>& e : relations) {
      int from = e[1], to = e[0];
      graph[from].push_back(edges.size());
      edges.push_back({from, to});
    }
    memset(mark, 0, sizeof(mark));
    memset(max_dist, 0, sizeof(max_dist));

    int dist = 0;
    for (int i = 1; i <= N; i++) {
      dist = max(dist, dfs(i));
      if (has_cycle) {
        return -1;
      }
    }
    return dist;
  }

  // Return the longest path start from @node.
  int dfs(int node) {
    if (mark[node]) {
      has_cycle = true;
      return 0;
    }

    if (max_dist[node]) {
      return max_dist[node];
    }

    mark[node] = true;
    int dist = 1;
    for (int edge_index : graph[node]) {
      dist = max(dist, dfs(edges[edge_index].to) + 1);
    }
    max_dist[node] = dist;
    mark[node] = false;
    return dist;
  }
};


/*****************************************************************************/

class Solution1 {
  vector<Edge> edges;
  vector<vector<int>> graph;
  int max_len;
  bool has_cycle;
  bool mark[5010];
 public:
  int minimumSemesters(int N, vector<vector<int>>& relations) {
    max_len = 0;
    has_cycle = false;
    graph.resize(N + 1);
    for (int i = 1; i <= N; i++) {
      graph[i].clear();
    }
    for (const vector<int>& e : relations) {
      int from = e[1], to = e[0];
      graph[from].push_back(edges.size());
      edges.push_back({from, to});
    }
    memset(mark, 0, sizeof(mark));

    for (int i = 1; i <= N; i++) {
      dfs(i, 1);
      if (has_cycle) {
        return -1;
      }
    }
    return max_len;
  }

  void dfs(int node, int depth) {
    if (mark[node]) {
      has_cycle = true;
      return;
    }
    max_len = max(max_len, depth);
    mark[node] = true;
    for (int edge_index : graph[node]) {
      dfs(edges[edge_index].to, depth + 1);
    }
    mark[node] = false;
  }
};