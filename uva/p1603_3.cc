/*
Iterative deepening.

Compare to p1603_4, this version stores which squares an edge can destroy, instead
of storing the whole graph and figure out this in each run.
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

// A set of visited existing square set.
set<set<int>> visited;

int encode(int r, int c) {
  return r * (sizeN + 1) + c;
}

bool isTarget(const vector<vector<int>>& edges) {
  return true;
}

struct TmpElem {
  int index;
  int attachedSquare;
  TmpElem(int index, int attachedSquare)
    : index(index), attachedSquare(attachedSquare) {};

  bool operator < (const TmpElem& o) const {
    return attachedSquare > o.attachedSquare;
  }
};

// Return all edges that in at least one square. Sorted in the descent
// order of how many squares they are attached to.
// Also returns the existing squares count.
vector<int> getEdgesSortedIndex(const vector<set<int>>& edges, set<int>& squares) {
  vector<TmpElem> tmp;
  // Edges index start at 1.
  for (int i = 1; i < edges.size(); i++) {
    if (edges[i].size() > 0) {
      tmp.push_back(TmpElem(i, (int)edges[i].size()));
      for (int v : edges[i]) {
        squares.insert(v);
      }
    }
  }
  sort(tmp.begin(), tmp.end());

  vector<int> rtn;
  for (int i = 0; i < tmp.size(); i++) {
    rtn.push_back(tmp[i].index);
  }
  return rtn;
}

void removeEdge(vector<set<int>>& edges, int toRemove) {
  set<int> tmp = edges[toRemove];
  edges[toRemove].clear();

  for (int i = 1; i < edges.size(); i++) {
    for (int elem : tmp) {
      edges[i].erase(elem);
    }
  }
}

bool dls(int depth, const vector<set<int>>& edges, int largestRemovedEdge) {
  set<int> remainingSquareSet;
  vector<int> indexes = getEdgesSortedIndex(edges, remainingSquareSet);

  if (visited.find(remainingSquareSet) != visited.end()) {
    return false;
  }
  visited.insert(remainingSquareSet);

  if (depth == 0) {
    // isTarget: there is no edge in any square.
    return indexes.size() == 0;
  }

  // One optimization.
  if (edges[indexes[0]].size() == 1) {
    return indexes.size() / 4 == depth;
  }

  // Another optimization.
  int counter = 0;
  for (int i = 0; i < indexes.size(); i++) {
    if (counter < remainingSquareSet.size()) {
      counter += edges[indexes[i]].size();
    } else {
      if (i > depth) {
        return false;
      } else {
        break;
      }
    }
  }

  for (int i = 0; i < indexes.size(); i++) {
    int edgeToRemove = indexes[i];
    if (edgeToRemove < largestRemovedEdge) {
      continue;
    }

    vector<set<int>> newEdges = edges;
    removeEdge(newEdges, edgeToRemove);
    if (dls(depth - 1, newEdges, edgeToRemove)) {
      return true;
    }
  }
  return false;
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

void mapSquareToEdge(const Graph& g, vector<set<int>>& edges) {
  for (int r = 0; r < sizeN; r++) {
    for (int c = 0; c < sizeN; c++) {
      for (int width = 1;
           width + r < sizeN + 1 && width + c < sizeN + 1;
           width++) {
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
          for (int edgeNumber : tmpEdges) {
            edges[edgeNumber].insert(r * 100 + c * 10 + width);
          }
        }
      }
    }
  }
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

  // A map from edge number to the encoded square id.
  int matchSticksSize = 2 * sizeN * (sizeN + 1) + 1;
  vector<set<int>> edges(matchSticksSize);
  mapSquareToEdge(graph, edges);

  for (int depth = 0; depth <= 2 * sizeN * (sizeN + 1); depth++) {
    printf("iteration with depth limit %d\n", depth);
    visited.clear();
    if (dls(depth, edges, 0)) {
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