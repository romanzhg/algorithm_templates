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

const int MAX_NODES = 350;

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

const int MAX_ELEM_SIZE = 110;
int converter_count;
int receptacle_count;
int device_count;

vector<string> receptacles(MAX_ELEM_SIZE);
vector<string> devices(MAX_ELEM_SIZE);
vector<pair<string, string>> converters(MAX_ELEM_SIZE);

int GetConverterVertexIndex(const pair<string, string>& v) {
  for (int i = 0; i < (int)converters.size(); i++) {
    if (converters[i] == v) {
      return i;
    }
  }
  return -1;
}

int GetReceptacleVertexIndex(const string& v) {
  for (int i = 0; i < (int) receptacles.size(); i++) {
    if (receptacles[i] == v) {
      return i + converter_count;
    }
  }
  return -1;
}

int GetReceptacleVertexIndex(int i) {
  return i + converter_count;
}

int GetDeviceVertexIndex(const string& v) {
  for (int i = 0; i < (int) devices.size(); i++) {
    if (devices[i] == v) {
      return i + converter_count + receptacle_count;
    }
  }
  return -1;
}

int GetDeviceVertexIndex(int i) {
  return i + converter_count + receptacle_count;
}

int GetSrcIndex() {
  return converter_count + receptacle_count + device_count;
}

int GetTargetIndex() {
  return GetSrcIndex() + 1;
}

int main() {
  bool is_first_test_case = true;
  int test_cases;
  scanf("%d", &test_cases);
  for (int test_case = 0; test_case < test_cases; test_case++) {
    scanf("%d", &receptacle_count);
    for (int i = 0; i < receptacle_count; i++) {
      char str_buffer[50];
      scanf("%s", str_buffer);
      receptacles[i] = str_buffer;
    }
    scanf("%d", &device_count);
    for (int i = 0; i < device_count; i++) {
      char device_name[50], device_plug_type[50];
      scanf("%s %s", device_name, device_plug_type);
      devices[i] = device_plug_type;
    }

    scanf("%d", &converter_count);
    EdmondsKarpSolver solver;
    solver.Init(converter_count + receptacle_count + device_count + 2);
    for (int i = 0; i < converter_count; i++) {
      char str_buffer_from[50], str_buffer_to[50];
      scanf("%s %s", str_buffer_from, str_buffer_to);
      converters[i].first = str_buffer_from;
      converters[i].second = str_buffer_to;
    }

    // Add edges.

    // Edges between converters.
    for (int i = 0; i < converter_count; i++) {
      for (int j = 0; j < converter_count; j++) {
        if (i == j) {
          continue;
        }
        if (converters[i].second == converters[j].first) {
          solver.AddEdge(i, j, INT_INF);
        }
      }
    }

    for (int i = 0; i < device_count; i++) {
      solver.AddEdge(GetSrcIndex(), GetDeviceVertexIndex(i), 1);
      for (int j = 0; j < converter_count; j++) {
        if (devices[i] == converters[j].first) {
          solver.AddEdge(GetDeviceVertexIndex(i), j, INT_INF);
        }
      }
      for (int j = 0; j < receptacle_count; j++) {
        if (devices[i] == receptacles[j]) {
          solver.AddEdge(GetDeviceVertexIndex(i), GetReceptacleVertexIndex(j), 1);
        }
      }
    }

    for (int i = 0; i < receptacle_count; i++) {
      solver.AddEdge(GetReceptacleVertexIndex(i), GetTargetIndex(), 1);
      for (int j = 0; j < converter_count; j++) {
        if (receptacles[i] == converters[j].second) {
          solver.AddEdge(j, GetReceptacleVertexIndex(i), INT_INF);
        }
      }
    }

    int flow = solver.MaxFlow(GetSrcIndex(), GetTargetIndex());
    if (is_first_test_case) {
      is_first_test_case = false;
    } else {
      cout << endl;
    }
    cout << device_count - flow << endl;
  }
  return 0;
}