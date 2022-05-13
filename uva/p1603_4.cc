/*
Iterative deepening.

This version is quite slow, since it stored the whole state, and in
each iteration, find how many squares each matchstick can break.

The only "optimization" used is ordering: the order that which edge
(match stick) get removed doesn't matter, so specify an order to remove
duplication - only remove A then B, not the reverse.
*/
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
typedef long long ll;
#define INF 1000000000

typedef map<pair<int, int>, int> Graph;
int sizeN;
set<int> edgesToRemove;

struct EdgePriority {
  int number;
  int squaresCount;

  EdgePriority(int number = 0, int squaresCount = 0)
    : number(number), squaresCount(squaresCount) {};

  bool operator < (const EdgePriority& o) const {
    return squaresCount < o.squaresCount;
  }
};

int encode(int r, int c) {
  return r * (sizeN + 1) + c;
}

void removeEdge(int edgeNumber, Graph& g) {
  for (auto it = g.begin(); it != g.end(); it++) {
    if (it->second == edgeNumber) {
      g.erase(it);
      return;
    }
  }
  exit(-1);
}

bool fillinEdgesOnSide(const pair<int, int>& startPoint, const Graph& g,
  const pair<int, int>& dir, int width, set<int>& tmpEdges) {
  for (int i = 0; i < width; i++) {
    pair<int, int> edgeKey = make_pair(
      encode(startPoint.first + i * dir.first,
             startPoint.second + i * dir.second),
      encode(startPoint.first + (i + 1) * dir.first,
             startPoint.second + (i + 1) * dir.second));

    if (g.find(edgeKey) == g.end()) {
      return false;
    } else {
      tmpEdges.insert(g.at(edgeKey));
    }
  }
  return true;
}

priority_queue<EdgePriority> getEdgePriority(const Graph& g, int& squares) {
  // A map from edge number to how many squares it can destroy.
  map<int, int> tmpMap;
  for (int r = 0; r < sizeN; r++) {
    for (int c = 0; c < sizeN; c++) {
      for (int width = 1;
           width + r < sizeN + 1 && width + c < sizeN + 1;
           width++) {
        // These 4 points defines a square.
        // First check if this square is valid(not broken).
        // If yes, all all its edges, otherwise, ignore it.
        // One optimization would be record all the already broken
        // squares and avoid searching for them.

        // // Upper left point.
        // pair<int, int> point1 = make_pair(r, c);
        // // Lower left point.
        // pair<int, int> point2 = make_pair(r + width, c);
        // // Upper right point.
        // pair<int, int> point3 = make_pair(r, c + width);
        // // Lower right point.
        // pair<int, int> point4 = make_pair(r + width, c + width);
        bool isValid = true;
        // The set of edges in this square.
        set<int> tmpEdges;

        // One direction.
        isValid = isValid &&
          fillinEdgesOnSide(make_pair(r, c), g, {1, 0}, width, tmpEdges);
        isValid = isValid &&
          fillinEdgesOnSide(make_pair(r, c + width), g, {1, 0}, width, tmpEdges);
        isValid = isValid &&
          fillinEdgesOnSide(make_pair(r, c), g, {0, 1}, width, tmpEdges);
        isValid = isValid &&
          fillinEdgesOnSide(make_pair(r + width, c), g, {0, 1}, width, tmpEdges);

        if (isValid) {
          squares++;
          for (int edgeNumber : tmpEdges) {
            tmpMap[edgeNumber]++;
          }
        }
      }
    }
  }

  priority_queue<EdgePriority> rtn;
  for (const pair<int, int>& v : tmpMap) {
    rtn.push(EdgePriority(v.first, v.second));
  }
  return rtn;
}

set<int> graphToEdgeSet(const Graph& g) {
  set<int> rtn;
  for (const pair<pair<int, int>, int>& v : g) {
    rtn.insert(v.second);
  }
  return rtn;
}

bool dls(int depth, const Graph& g, int largestRemovedEdge) {
  int remainingSquares = 0;
  priority_queue<EdgePriority> pq = getEdgePriority(g, remainingSquares);

  if (depth == 0) {
    // pq.size() == 0 means there is no squares.
    return pq.size() == 0;
  }

  // An optimization.
  if (pq.top().squaresCount == 1) {
    return ((pq.size()) / 4) == depth;
  }

  // An optimization.
  priority_queue<EdgePriority> pqDup = pq;
  int tmpSquares = 0;
  // At least this number of match sticks needs to be removed.
  int tmpCounter = 0;
  while (tmpSquares < remainingSquares) {
    EdgePriority ep = pqDup.top();
    pqDup.pop();
    tmpSquares += ep.squaresCount;
    tmpCounter++;
  }
  if (tmpCounter > depth) {
    return false;
  }

  // Try all the squares, get a count of edge/squares it can destroy.
  while (!pq.empty()) {
    EdgePriority ep = pq.top();
    pq.pop();

    // printf("the edge to remove: %d\n", ep.number);
    if (ep.number < largestRemovedEdge) {
      continue;
    }

    Graph tmpGraph = g;
    removeEdge(ep.number, tmpGraph);

    if (dls(depth - 1, tmpGraph, ep.number)) {
      return true;
    }
  }
  return false;
}

void solve() {
  // First build the full graph.
  Graph graph;

  for (int r = 0; r < sizeN + 1; r++) {
    int edgeNumber = 1 + r * (2 * sizeN + 1);
    for (int c = 1; c < sizeN + 1; c++) {
      graph.insert(make_pair(make_pair(encode(r, c - 1), encode(r, c)), edgeNumber));
      edgeNumber++;
    }
  }
  for (int r = 1; r < sizeN + 1; r++) {
    int edgeNumber = 1 + r * sizeN + (r - 1) * (sizeN + 1);
    for (int c = 0; c < sizeN + 1; c++) {
      graph.insert(make_pair(make_pair(encode(r - 1, c), encode(r, c)), edgeNumber));
      edgeNumber++;
    }
  }

  // Remove edges from the initial graph.
  for (int edge : edgesToRemove) {
    for (auto it = graph.begin(); it != graph.end();) {
      if (it->second == edge) {
        it = graph.erase(it);
      } else {
        it++;
      }
    }
  }

  for (int depth = 0; depth <= 2 * sizeN * (sizeN + 1); depth++) {
    // printf("iteration with depth limit %d\n", depth);
    if (dls(depth, graph, 0)) {
      printf("%d\n", depth);
      return;
    }
  }
}

int main() {
  int test_cases;
  scanf("%d", &test_cases);
  while (test_cases--) {
    scanf("%d", &sizeN);
    edgesToRemove.clear();
    int edgesToRemoveCount;
    scanf("%d", &edgesToRemoveCount);
    for (int i = 0; i < edgesToRemoveCount; i++) {
      int edgeNumber;
      scanf("%d", &edgeNumber);
      edgesToRemove.insert(edgeNumber);
    }
    solve();
  }
  return 0;
}