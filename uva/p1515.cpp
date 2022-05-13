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
const int MAX_NODES = 5200;

/*****************************************************************************/

// Maximum flow.
struct Edge {
  int from, to, cap, flow;
  Edge(int from, int to, int cap, int flow) :
      from(from), to(to), cap(cap), flow(flow) {}
};

// An easy but lower efficient algorithm.
struct EdmondsKarpSolver {
  int node_count, edge_count;
  vector<Edge> edges;
  // graph[i][j] is the index(in edges) of the j-th edge going out from the i-th node.
  vector<int> graph[MAX_NODES];

  // Possible improvements from source to the node.
  int arg_amount[MAX_NODES];
  int from_edge[MAX_NODES];

  void Init(int input_node_count) {
    node_count = input_node_count;
    for (int i = 0; i < node_count; i++) {
      graph[i].clear();
    }
    edges.clear();
  }

  void AddEdge(int from, int to, int cap) {
    edges.push_back(Edge(from, to, cap, 0));
    // Reverse arc.
    edges.push_back(Edge(to, from, 0, 0));
    edge_count = edges.size();
    graph[from].push_back(edge_count - 2);
    graph[to].push_back(edge_count - 1);
  }

  int MaxFlow(int src, int target) {
    int flow = 0;
    while (true) {
      memset(arg_amount, 0, sizeof(arg_amount));
      queue<int> vertex_queue;
      vertex_queue.push(src);
      arg_amount[src] = INT_INF;

      while (!vertex_queue.empty()) {
        int cur = vertex_queue.front(); vertex_queue.pop();
        for (int i = 0; i < (int) graph[cur].size(); i++) {
          Edge& e = edges[graph[cur][i]];
          if ((arg_amount[e.to] == 0) && e.cap > e.flow) {
            from_edge[e.to] = graph[cur][i];
            arg_amount[e.to] = min(arg_amount[cur], e.cap - e.flow);
            vertex_queue.push(e.to);
          }
        }
        if (arg_amount[target] != 0) {
          break;
        }
      }

      if (arg_amount[target] == 0) {
        break;
      }

      for (int u = target; u != src; u = edges[from_edge[u]].from) {
        edges[from_edge[u]].flow += arg_amount[target];
        edges[from_edge[u] ^ 1].flow -= arg_amount[target];
      }

      flow += arg_amount[target];
    }
    return flow;
  }
};

/*****************************************************************************/

const int MAP_SIZE = 60;

int width, height, dig_cost, fill_cost, boundary_cost;

int GetIndex(int i, int j) {
  return i * width + j;
}

bool IsOnEdge(int i, int j) {
  return (i == 0) || (i == (height - 1)) || (j == 0) || (j == (width - 1));
}

const vector<pair<int, int>> kNeighbor = {
    {-1, 0}, {1, 0},
    {0, -1}, {0, 1}};

int main() {
  int test_case_count;
  EdmondsKarpSolver solver;
  scanf("%d", &test_case_count);
  for (int test_case_num = 0; test_case_num < test_case_count; test_case_num++) {
    scanf("%d %d", &width, &height);
    scanf("%d %d %d", &dig_cost, &fill_cost, &boundary_cost);
    char input_map[MAP_SIZE][MAP_SIZE];
    for (int i = 0; i < height; i++) {
      scanf("%s", input_map[i]);
    }

    int total_cost = 0;
    // Fix the boundary and get the cost.
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        if (IsOnEdge(i, j)) {
          if (input_map[i][j] == '.') {
            total_cost += fill_cost;
            input_map[i][j] = '#';
          }
        }
      }
    }

    const int src_index = height * width;
    const int target_index = src_index + 1;
    solver.Init(height * width + 2);

    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        int index = GetIndex(i, j);

        if (IsOnEdge(i, j)) {
          solver.AddEdge(src_index, index, INT_INF);
        } else {
          if (input_map[i][j] == '.') {
            solver.AddEdge(index, target_index, fill_cost);
          } else {
            solver.AddEdge(src_index, index, dig_cost);
          }
        }

        for (const auto& delta : kNeighbor) {
          int new_i = i + delta.first;
          int new_j = j + delta.second;
          if (new_i >= 0 && new_i < height && new_j >= 0 && new_j < width) {
            int new_index = GetIndex(new_i, new_j);
            if (new_index < index) {
              continue;
            }
            solver.AddEdge(index, new_index, boundary_cost);
            solver.AddEdge(new_index, index, boundary_cost);
          }
        }
      }
    }
    total_cost += solver.MaxFlow(src_index, target_index);
    cout << total_cost << endl;
  }
  return 0;
}