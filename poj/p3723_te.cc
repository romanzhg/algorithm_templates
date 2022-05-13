#include <cstdio>
#include <climits>
#include <queue>
#include <vector>
#include <utility>
#include <algorithm>
#include <functional>
#include <cstring>
using namespace std;

#define N 20000+10

struct Edge {
  int to;
  int cost;
};

// An adjacent list is used here because N*N cannot fit into memory.
// However, with adjacent list and the Prim algorithm, we cannot play some trick
// to find the next node with minimum distance to the existing set, so TLE.
// Should try the Kruskal algorithm.
vector<Edge> graph[N];

// Min cost of all the interset edges.
int mincost[N];
// The visited set.
bool used[N];

void addEdge(int from, int to, int cost) {
  //printf("adding edges: from: %d, to: %d, cost: %d\n", from, to, cost);
  // check for duplicate
  bool hasDuplicate = false;

  vector<Edge>& edges = graph[from];
  for (int i = 0; i < edges.size(); i++) {
    if (edges[i].to == to) {
      hasDuplicate = true;
      edges[i].cost = min(edges[i].cost, cost);
    }
  }
  if (hasDuplicate) {
    return;
  }

  Edge e;
  e.to = to;
  e.cost = cost;
  graph[from].push_back(e);
}

int getNewNodeWithMinCost(int totalNodes) {
  int minCost = 0;
  int nodeId = -1;
  for (int i = 0; i < totalNodes; i++) {
    if (!used[i] && minCost > mincost[i]) {
      minCost = mincost[i];
      nodeId = i;
    }
  }
  if (nodeId != -1) {
    return nodeId;
  }

  for (int i = 0; i < totalNodes; i++) {
    if (!used[i]) {
      return i;
    }
  }
  return -1;
}

void solve(int totalNodes) {

/*
  printf("total nodes %d\n", totalNodes);
  for (int i = 0; i < totalNodes; i++) {
    const vector<Edge>& edges = graph[i];
    for (int j = 0; j < edges.size(); j++) {
      printf("from %d, to %d, cost %d\n", i, edges[j].to, edges[j].cost);
    }
  }

*/

  // A negative number, to be minimized.
  int totalCost = 0;
  for (int i = 0; i < totalNodes; i++) {
    mincost[i] = 0;
    used[i] = false;
  }

  // need a while loop to deal with unconnected nodes.
  while (true) {
    int unvisited = getNewNodeWithMinCost(totalNodes);
    //printf("the unvisited node number: %d\n", unvisited);
    if (unvisited == -1) {
      break;
    }
    used[unvisited] = true;
    //printf("newly added cost: %d\n", mincost[unvisited]);
    totalCost += mincost[unvisited];
    
    // update the min cost of all nodes reachable.
    for (int edgeIndex = 0; edgeIndex < graph[unvisited].size(); edgeIndex++) {
      const Edge& e = graph[unvisited][edgeIndex];
      if (used[e.to]) { continue; }
      //printf("before update: node %d, cost %d\n", e.to, mincost[e.to]);
      mincost[e.to] = min(mincost[e.to], e.cost);
      //printf("after update: node %d, cost %d\n", e.to, mincost[e.to]);
    }
  }
 
  printf("%d\n", totalNodes * 10000 + totalCost);
  for (int i = 0; i < totalNodes; i++) {
    graph[i].clear();
  }
}

int main() {
  int testCases;
  int females, males, relations;

  scanf("%d", &testCases);
  for (int i = 0; i < testCases; i++) {
    scanf("%d %d %d", &females, &males, &relations);
    for (int j = 0; j < relations; j++) {
      int rFemale, rMale, relationScore;
      scanf("%d %d %d", &rFemale, &rMale, &relationScore);
      addEdge(rFemale, females + rMale, -relationScore);
      addEdge(females + rMale, rFemale, -relationScore);
    }
    solve(females + males);
  }
  return 0;
}
