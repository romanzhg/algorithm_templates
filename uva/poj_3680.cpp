#include <cstdio>
#include <cstdlib>
#include <climits>
#include <queue>
#include <vector>
#include <utility>
#include <algorithm>
#include <functional>
#include <cstring>
#include <string>
#include <map>
#include <set>
#include <iostream>
using namespace std;

const int INT_INF = INT_MAX / 2;

const int MAX_NODES = 101000;
int max_overlapping_intervals;
int number_of_intervals;

#define MAX_INPUT_SIZE 210
int i_start[MAX_INPUT_SIZE];
int i_end[MAX_INPUT_SIZE];
int i_weight[MAX_INPUT_SIZE];

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

MCMF solver;

int GetIndex(const vector<int>& end_points, int value) {
  for (int i = 0; i < (int)end_points.size(); i++) {
    if (end_points[i] == value) {
      return i;
    }
  }
  return -1;
}

int main() {
  int test_cases;
  scanf("%d", &test_cases);
  for (int test_case = 0; test_case < test_cases; test_case++) {
    scanf("%d %d", &number_of_intervals, &max_overlapping_intervals);
    for (int i = 0; i < number_of_intervals; i++) {
      scanf("%d %d %d", &i_start[i], &i_end[i], &i_weight[i]);
    }

    vector<int> end_points;
    for (int i = 0; i < number_of_intervals; i++) {
      end_points.push_back(i_start[i]);
      end_points.push_back(i_end[i]);
    }
    sort(end_points.begin(), end_points.end());
    end_points.erase(unique(end_points.begin(), end_points.end()), end_points.end());

    solver.init(end_points.size() + 2);
    for (int i = 0; i < number_of_intervals; i++) {
      solver.AddEdge(GetIndex(end_points, i_start[i]),
                     GetIndex(end_points, i_end[i]),
                     1, -i_weight[i]);
    }
    for (int i = 0; i < end_points.size() - 1; i++) {
      solver.AddEdge(i, i + 1, INT_INF, 0);
    }
    solver.AddEdge(end_points.size(), 0, max_overlapping_intervals, 0);
    solver.AddEdge(end_points.size() - 1, end_points.size() + 1, max_overlapping_intervals, 0);

    long long cost;
    solver.MinCostMaxFlow(end_points.size(), end_points.size() + 1, cost);
    cout << -cost << endl;
  }
  return 0;
}