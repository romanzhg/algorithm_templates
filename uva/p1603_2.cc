/*
IDA*.
The heuristic function is in best case(assume there is no overlap), how
many match sticks needs to be removed.
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

int encode(int r, int c) {
  return r * (sizeN + 1) + c;
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

int getHeuristicDist(const vector<set<int>>& edges,
  const vector<int>& indexes,
  int remainingSquares) {
  int counter = 0;
  for (int i = 0; i < indexes.size(); i++) {
    if (counter < remainingSquares) {
      counter += edges[indexes[i]].size();
    } else {
      return i;
    }
  }
  return 0;
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

bool search(const vector<set<int>>& edges, const int currentDist,
            const int bound, int& nextBound, const int largestRemovedEdge) {
  // printf("in a new search call.\n");
  set<int> remainingSquareSet;
  vector<int> indexes = getEdgesSortedIndex(edges, remainingSquareSet);

  int f = currentDist + getHeuristicDist(edges, indexes, remainingSquareSet.size());
  // printf("get current state estimated cost: %d\n", f);

  if (f > bound) {
    nextBound = f;
    return false;
  }

  if (remainingSquareSet.size() == 0) {
    return true;
  }

  int minBound = INF;

  for (int i = 0; i < indexes.size(); i++) {
    if (indexes[i] < largestRemovedEdge) {
      continue;
    }

    vector<set<int>> tmpEdges = edges;
    removeEdge(tmpEdges, indexes[i]);

    int tmpBound;
    bool found = search(tmpEdges, currentDist + 1, bound, tmpBound, indexes[i]);
    if (found) {
      return true;
    }
    minBound = min(minBound, tmpBound);
  }
  nextBound = minBound;
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

  set<int> remainingSquareSet;
  vector<int> indexes = getEdgesSortedIndex(edges, remainingSquareSet);

  int bound = getHeuristicDist(edges, indexes, remainingSquareSet.size());
  
  while (true) {
    int nextBound;
    bool found = search(edges, 0, bound, nextBound, 0);
    if (found) {
      printf("%d\n", bound);
      return;
    }
    if (nextBound == INF) {
      printf("error here\n");
      exit(-1);
      return;
    } else {
      bound = nextBound;
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