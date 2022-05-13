/**
 * Print group first, then print nodes.
 *
 * Side notes: to represent a graph.
 *   1. vector<Edge> edges(edges), vector<int> adj[node_count](node to its outgoing
 *          edge list(a list of index for the edges array)).
 *
 *          : Preferred in oj problem, concise, can handle duplicate edges. Cannot be
 *          distributed among multiple threads.
 *
 *   2. map<int, map<int, EdgeData>>, a map with FromNode -> ToNode -> Edge data.
 *          (need to change the inner map to multimap in order to handle duplicate edge).
 *
 *          : More intuitive, good if the graph is sparse, can be distributed among multiple threads(sharded
 *          on the first key).
 *          : both 1 and 2 it is hard to implement reverse edge.
 *
 *   3. bool connected[node_count][node_count] : when edge information is not important,
 *          just use a matrix to record if two nodes are connected.
 *
 *          : Adjacency matrix.
 *
 *   4. Edge connected[node_count][node_count]: when edge information is important.
 *
 *   5. map<int, vector<int>> connected: similar to 3, when edge information is not important.
 *
 *
 * Side notes: there are two ways to print DAG in a topological order.
 *   The (find a node that has no incoming edges, print it, remove all outgoing edges) Kahn's algorithm has edges
 *   from free node to dependent nodes, and it requires keeping track of nodes without outgoing edges.
 *   The dfs way(as implemented here) has edges from dependent nodes to free nodes, but requires two mark array.
 */
#include "common.h"

const int kMaxNodes = 30010;

struct Edge {
  int from, to;
  Edge(int from, int to) : from(from), to(to) {};
};

map<int, int> elem_id_to_group;
map<int, vector<int>> group_id_to_elem;

vector<Edge> group_edges;
vector<int> group_graph[kMaxNodes];
vector<Edge> node_edges;
vector<int> node_graph[kMaxNodes];

bool group_p_mark[kMaxNodes];
bool group_mark[kMaxNodes];
bool node_p_mark[kMaxNodes];
bool node_mark[kMaxNodes];

int dfs_node(int cur, vector<int>& rtn) {
  if (node_p_mark[cur]) {
    return 0;
  }
  if (node_mark[cur]) {
    return -1;
  }

  node_mark[cur] = true;

  for (int edge_index : node_graph[cur]) {
    if (dfs_node(node_edges[edge_index].to, rtn) == -1) {
      return -1;
    }
  }
  rtn.push_back(cur);
  node_p_mark[cur] = true;
  node_mark[cur] = false;
  return 0;
}

int print_group(int cur, vector<int>& rtn) {
  const vector<int>& nodes = group_id_to_elem[cur];

  for (int n : nodes) {
    if (node_p_mark[n]) {
      continue;
    }
    if (dfs_node(n, rtn) == -1) {
      return -1;
    }
  }

  return 0;
}

int dfs_group(int cur, vector<int>& rtn) {
  if (group_p_mark[cur]) {
    return 0;
  }
  if (group_mark[cur]) {
    return -1;
  }

  group_mark[cur] = true;
  for (int edge_index : group_graph[cur]) {
    if (dfs_group(group_edges[edge_index].to, rtn) == -1) {
      return -1;
    }
  }
  if (print_group(cur, rtn) == -1) {
    return -1;
  }
  group_p_mark[cur] = true;
  group_mark[cur] = false;
  return 0;
}

class Solution {

 public:
  vector<int> sortItems(int n, int m, vector<int>& group, vector<vector<int>>& beforeItems) {
    vector<int> tmp = group;
    sort(tmp.begin(), tmp.end());
    int start_group_id = (*tmp.rbegin()) + 1;

    elem_id_to_group.clear();
    group_id_to_elem.clear();

    for (int i = 0; i < (int)group.size(); i++) {
      if (group[i] == -1) {
        elem_id_to_group[i] = start_group_id;
        group_id_to_elem[start_group_id].push_back(i);
        start_group_id++;
      } else {
        elem_id_to_group[i] = group[i];
        group_id_to_elem[group[i]].push_back(i);
      }
    }

    int elem_count = n, group_count = start_group_id;

    group_edges.clear();
    for (int i = 0; i < group_count; i++) {
      group_graph[i].clear();
    }
    node_edges.clear();
    for (int i = 0; i < elem_count; i++) {
      node_graph[i].clear();
    }

    for (int i = 0; i < elem_count; i++) {
      for (int e : beforeItems[i]) {
        int group_i = elem_id_to_group[i];
        int group_e = elem_id_to_group[e];
        if (group_i == group_e) {
          node_graph[i].push_back(node_edges.size());
          node_edges.push_back({i, e});
        } else {
          group_graph[group_i].push_back(group_edges.size());
          group_edges.push_back({group_i, group_e});
        }
      }
    }


    // Print group first, then nodes inside group.

    memset(group_p_mark, 0, sizeof(group_p_mark));
    memset(group_mark, 0, sizeof(group_mark));
    memset(node_p_mark, 0, sizeof(node_p_mark));
    memset(node_mark, 0, sizeof(node_mark));

    vector<int> rtn;
    for (int i = 0; i < group_count; i++) {
      if (group_p_mark[i]) {
        continue;
      }
      if (dfs_group(i, rtn) == -1) {
        return {};
      }
    }
    return rtn;
  }
};