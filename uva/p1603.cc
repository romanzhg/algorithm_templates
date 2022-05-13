// Iterative deepening.
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

#define MAXN 150

int sizeN;

// Edges are numbered [1, totalEdges].
int totalEdges;
// Squares are numberd [0, totalSquares).
int totalSquares;

// hasEdge[i]: true if edge i is not removed.
bool hasEdge[MAXN];

// contains[i][j]: square i contains edge j.
bool contains[MAXN][MAXN];
// fullSize[i]: number of edges square i should contain if it is valid.
int fullSize[MAXN];
int realSize[MAXN];

// Get the edge number of the left to right edge start from point (r, c).
int GetHorizontalEdgeNumber(int r, int c) {
  return 1 + r * (2 * sizeN + 1) + c;
}

// Get the edge number of the top to down edge start from point (r, c).
int GetVerticalEdgeNumber(int r, int c) {
  return 1 + (r + 1) * sizeN + r * (sizeN + 1) + c;
}

int findFirstSquare() {
  for (int i = 0; i < totalSquares; i++) {
    if (realSize[i] == fullSize[i]) {
      return i;
    }
  }
  return -1;
}

bool dls(int depth) {
  if (depth == 0) {
    return findFirstSquare() == -1;
  }

  // Find the first full square.
  int squareIndex = findFirstSquare();

  for (int i = 1; i <= totalEdges; i++) {
    if (hasEdge[i] && contains[squareIndex][i]) {
      hasEdge[i] = false;
      for (int j = 0; j < totalSquares; j++) {
        if (contains[j][i]) {
          realSize[j]--;
        }
      }

      if (dls(depth - 1)) {
        return true;
      }

      hasEdge[i] = true;
      for (int j = 0; j < totalSquares; j++) {
        if (contains[j][i]) {
          realSize[j]++;
        }
      }
    }
  }

  return false;
}

void solve() {
  for (int r = 0; r < sizeN; r++) {
    for (int c = 0; c < sizeN; c++) {
      for (int width = 1;
           width + r < sizeN + 1 && width + c < sizeN + 1;
           width++) {

        fullSize[totalSquares] = 4 * width;
        int edgeNumber;

        for (int k = 0; k < width; k++) {
          // Upper side.
          edgeNumber = GetHorizontalEdgeNumber(r, c + k);
          contains[totalSquares][edgeNumber] = true;
          if (hasEdge[edgeNumber]) {
            realSize[totalSquares]++;
          }

          // Bottom side.
          edgeNumber = GetHorizontalEdgeNumber(r + width, c + k);
          contains[totalSquares][edgeNumber] = true;
          if (hasEdge[edgeNumber]) {
            realSize[totalSquares]++;
          }

          // Left side.
          edgeNumber = GetVerticalEdgeNumber(r + k, c);
          contains[totalSquares][edgeNumber] = true;
          if (hasEdge[edgeNumber]) {
            realSize[totalSquares]++;
          }

          // Right side.
          edgeNumber = GetVerticalEdgeNumber(r + k, c + width);
          contains[totalSquares][edgeNumber] = true;
          if (hasEdge[edgeNumber]) {
            realSize[totalSquares]++;
          }
        }
        totalSquares++;
      }
    }
  }

  for (int depth = 0; depth < MAXN; depth++) {
    if (dls(depth)) {
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

    memset(contains, 0, sizeof(contains));
    memset(hasEdge, 0, sizeof(hasEdge));
    memset(realSize, 0, sizeof(realSize));

    totalEdges = 2 * sizeN * (sizeN + 1);
    totalSquares = 0;

    for (int i = 1; i <= totalEdges; i++) {
      hasEdge[i] = true;
    }

    int edgesToRemoveCount;
    scanf("%d", &edgesToRemoveCount);
    for (int i = 0; i < edgesToRemoveCount; i++) {
      int edgeNumber;
      scanf("%d", &edgeNumber);
      hasEdge[edgeNumber] = false;
    }

    solve();
  }
  return 0;
}