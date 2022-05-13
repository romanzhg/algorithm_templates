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

const long long INF = LONG_MAX / 2;
const int VERTEX_SIZE = 310;
int vertex_count, edge_count, g_src, g_dest;

/*****************************************************************************/
// Dijkstra, for single source shortest path.
struct HeapNode {
  long long dist;
  int vertex_id;

  bool operator< (const HeapNode& o) const {
    return dist > o.dist;
  }
};

struct Edge {
  int from;
  int to;

  int open_time, close_time;
  long long dist;

  Edge(int from, int to, int a, int b, long long cost)
      : from(from), to(to), open_time(a), close_time(b), dist(cost) {};

  long long GetActualDist(long long start_time) const {
    long long tmp = start_time % (open_time + close_time);

    if (tmp + dist <= open_time) {
      return dist;
    } else {
      return (open_time + close_time) - tmp + dist;
    }
  }
};

struct Dijkstra {
  long long dists[VERTEX_SIZE];
  vector<Edge> edges;
  vector<int> adj_list[VERTEX_SIZE];

  void Init() {
    edges.clear();
    for (int i = 0; i < VERTEX_SIZE; i++) {
      adj_list[i].clear();
    }
  }

  void AddEdge(const Edge& e) {
    edges.push_back(e);
    adj_list[e.from].push_back(edges.size() - 1);
  }

  void GetShortestDist(int src_id) {
    priority_queue<HeapNode> pq_;
    for (int i = 0; i < VERTEX_SIZE; i++) {
      dists[i] = INF;
    }
    dists[src_id] = 0;
    pq_.push((HeapNode){0, src_id});

    while(!pq_.empty()) {
      HeapNode x = pq_.top(); pq_.pop();
      int vertex_id = x.vertex_id;

      if (x.dist > dists[x.vertex_id]) {
        continue;
      }
      for (int i = 0; i < (int)adj_list[vertex_id].size(); i++) {
        Edge& e = edges[adj_list[vertex_id][i]];
        if (e.open_time < e.dist) {
          // Note this check.
          continue;
        }
        long long new_dist = e.GetActualDist(x.dist) + x.dist;
        if (dists[e.to] > new_dist) {
          dists[e.to] = new_dist;
          pq_.push((HeapNode){dists[e.to], e.to});
        }
      }
    }
  }
};

/*****************************************************************************/


int main() {
  int test_case = 0;
  Dijkstra solver;
  while (scanf("%d %d %d %d", &vertex_count, &edge_count, &g_src, &g_dest) == 4) {
    solver.Init();

    for (int i = 0; i < edge_count; i++) {
      int from, to, a, b, cost;
      scanf("%d %d %d %d %d", &from, &to, &a, &b, &cost);
      solver.AddEdge({from, to, a, b, cost});
    }

    solver.GetShortestDist(g_src);
    cout << "Case " << ++test_case << ": " << solver.dists[g_dest] << endl;
  }
  return 0;
}