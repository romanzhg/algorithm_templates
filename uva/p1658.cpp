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

const int INT_INF = INT_MAX / 2;

const int MAX_NODES = 2200;
/*****************************************************************************/

// Min cost maximum flow.

struct Edge {
  int from, to, cap, flow, cost;
  Edge(int u, int v, int c, int f, int w) :
      from(u), to(v), cap(c), flow(f), cost(w) {}
};

struct MCMF {
  int node_count, edge_count;
  vector<Edge> edges;
  vector<int> graph[MAX_NODES];

  int in_queue[MAX_NODES];
  // Distance for Bellman-Ford, actually corresponds to the cost.
  int dist[MAX_NODES];
  int arg_amount[MAX_NODES];
  int from_edge[MAX_NODES];

  void init(int node_count) {
    this->node_count = node_count;
    for (int i = 0; i < node_count; i++) {
      graph[i].clear();
    }
    edges.clear();
  }

  void AddEdge(int from, int to, int cap, int cost) {
    edges.push_back(Edge(from, to, cap, 0, cost));
    edges.push_back(Edge(to, from, 0, 0, -cost));
    edge_count = edges.size();
    graph[from].push_back(edge_count - 2);
    graph[to].push_back(edge_count - 1);
  }

  bool BellmanFord(int src, int target, int &flow, long long &cost) {
    for (int i = 0; i < node_count; i++) {
      dist[i] = INT_INF;
    }
    memset(in_queue, 0, sizeof(in_queue));
    dist[src] = 0;
    in_queue[src] = 1;
    from_edge[src] = 0;
    arg_amount[src] = INT_INF;

    queue<int> vertex_queue;
    vertex_queue.push(src);
    while (!vertex_queue.empty()) {
      int cur_vertex = vertex_queue.front();
      vertex_queue.pop();
      in_queue[cur_vertex] = 0;
      for (int i = 0; i < graph[cur_vertex].size(); i++) {
        Edge &e = edges[graph[cur_vertex][i]];
        if (e.cap > e.flow && dist[e.to] > dist[cur_vertex] + e.cost) {
          dist[e.to] = dist[cur_vertex] + e.cost;
          from_edge[e.to] = graph[cur_vertex][i];
          arg_amount[e.to] = min(arg_amount[cur_vertex], e.cap - e.flow);
          if (!in_queue[e.to]) {
            vertex_queue.push(e.to);
            in_queue[e.to] = 1;
          }
        }
      }
    }

    if (dist[target] == INT_INF) {
      return false;
    }

    flow += arg_amount[target];
    cost += (long long) dist[target] * (long long) arg_amount[target];
    for (int u = target; u != src; u = edges[from_edge[u]].from) {
      edges[from_edge[u]].flow += arg_amount[target];
      edges[from_edge[u] ^ 1].flow -= arg_amount[target];
    }
    return true;
  }

  int MinCostMaxFlow(int s, int t, long long &cost) {
    int flow = 0;
    cost = 0;
    while (BellmanFord(s, t, flow, cost)) {};
    return flow;
  }
};

/*****************************************************************************/

int vertex_count, edge_count;

int GetIncomingVertexIndex(int vertex_id) {
  return vertex_id * 2 - 1;
}

int GetOutgoingVertexIndex(int vertex_id) {
  return vertex_id * 2;
}

int main() {
  MCMF solver;
  while (scanf("%d %d", &vertex_count, &edge_count) == 2) {
    solver.init(vertex_count * 2 + 1);

    // For src node.
    solver.AddEdge(1, 2, 2, 0);
    // For other nodes.
    for (int i = 2 ; i <= vertex_count; i++) {
      solver.AddEdge(GetIncomingVertexIndex(i), GetOutgoingVertexIndex(i), 1, 0);
    }

    for (int i = 0; i < edge_count; i++) {
      int from, to, cost;
      scanf("%d %d %d", &from, &to, &cost);
      solver.AddEdge(GetOutgoingVertexIndex(from), GetIncomingVertexIndex(to), 1, cost);
    }

    long long cost = 0;
    solver.MinCostMaxFlow(1, GetIncomingVertexIndex(vertex_count), cost);
    cout << cost << endl;
  }
  return 0;
}