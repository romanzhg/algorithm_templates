#include <cstdio>
#include <climits>
#include <queue>
#include <vector>
#include <utility>
#include <algorithm>
#include <functional> // for greater
#include <cstring>
using namespace std;

// Data structure to represent the graph.
#define N 100000+10  
#define INF 100000000  
struct Edge {
  int to;
  int cost;
};

vector<Edge> graph[N];

// A possible shortest distance to a node.
// First is the dist, second is the node.
typedef pair<int, int> P;

// Shortest distance to each node, and second shortest.
int dist[N];
int sdist[N];

// Actual number of nodes and edges.
int V, E;

void add_edge(int from, int to, int cost) {  
  Edge e;
  e.to = to;
  e.cost = cost; 
  graph[from].push_back(e);  
}  

void solve() {
  // Assign int max to everything.
  for (int i = 0; i < V; i++) {
    dist[i] = INF;
    sdist[i] = INF;
  }

  dist[0] = 0;

  // The priority queue.
  priority_queue<P, vector<P>, greater<P> > que;
  que.push(P(0,0));

  // In this first version, find 
  while(!que.empty()) {
    P tmp = que.top();
    que.pop();

    int current_node = tmp.second;
    int current_dist = tmp.first;

    // If for the current node, we already find a shorter path, skip.
    if (sdist[current_node] < current_dist) {
      continue;
    }

    for (int i = 0; i < graph[current_node].size(); i++) {
      Edge& e = graph[current_node][i];

      int tmpDist = e.cost + current_dist;
      if (dist[e.to] > tmpDist) {
        swap(dist[e.to], tmpDist);
        que.push(P(dist[e.to], e.to));
      }

      if (sdist[e.to] > tmpDist && dist[e.to] < tmpDist) {
        sdist[e.to] = tmpDist;
        que.push(P(sdist[e.to], e.to));
      }
    }
  }
  
  printf("%d\n", sdist[V - 1]);
}

int main() {
  while (scanf("%d %d", &V, &E) != EOF) {
    for (int i = 0; i < E; i++) {
      int from, to, cost;
      scanf("%d %d %d", &from, &to, &cost);
      from--;
      to--;
      add_edge(from, to, cost);
      add_edge(to, from, cost);
    }
    solve();
  }
  return 0;
}
