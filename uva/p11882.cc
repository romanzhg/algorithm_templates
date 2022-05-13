// This problem is essentially find the permutation of 30 numbers.
// The state space is large but there are obvious ways to prune.
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

#define MAXN 35

int rows, cols;
char maze[MAXN][MAXN];
bool visited[MAXN][MAXN];
bool connectedRegionVisited[MAXN][MAXN];

char result[MAXN];
int resultSize;

char tmpResult[MAXN];
int tmpResultSize;

vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

void printTmpResult() {
  printf("printing tmp result: ");
  for (int i = 0; i < tmpResultSize; i++) {
    printf("%c", tmpResult[i]);
  }
  printf("\n");
}

// Returns true if tmpResult is larger than result.
bool isTmpResultLarger() {
  if (tmpResultSize != resultSize) {
    return tmpResultSize > resultSize;
  }

  for (int i = 0; i < tmpResultSize; i++) {
    if (tmpResult[i] > result[i]) {
      return true;
    } else if (tmpResult[i] < result[i]) {
      return false;
    }
  }
  return false;
}

int getConnectedRegionSize(int r, int c) {
  if (r < 0 || r >= rows || c < 0 || c >= cols) {
    return 0;
  }
  if (maze[r][c] == '#') {
    return 0;
  }
  if (connectedRegionVisited[r][c]) {
    return 0;
  }
  connectedRegionVisited[r][c] = true;
  int rtn = 1;
  rtn += getConnectedRegionSize(r - 1, c);
  rtn += getConnectedRegionSize(r + 1, c);
  rtn += getConnectedRegionSize(r, c - 1);
  rtn += getConnectedRegionSize(r, c + 1);
  return rtn;
}

void dfs(int r, int c) {
  visited[r][c] = true;
  tmpResult[tmpResultSize++] = maze[r][c];
  if (isTmpResultLarger()) {
    resultSize = tmpResultSize;
    for (int i = 0; i < resultSize; i++) {
      result[i] = tmpResult[i];
    }
  }

  // Except for the first step, only goes into the largest connected region.
  int maxConnectedRegionSize = 0;
  set<int> directionIndexes;

  for (int i = 0; i < directions.size(); i++) {
    const pair<int, int>& direction = directions[i];
    memcpy(connectedRegionVisited, visited, sizeof(visited));
    int tmpCRSize = getConnectedRegionSize(
      r + direction.first, c + direction.second);
    if (tmpCRSize == 0) {
      continue;
    }
    if (directionIndexes.size() == 0) {
      directionIndexes.insert(i);
      maxConnectedRegionSize = tmpCRSize;
    } else {
      if (tmpCRSize > maxConnectedRegionSize) {
        directionIndexes.clear();
        directionIndexes.insert(i);
        maxConnectedRegionSize = tmpCRSize;
      } else if (tmpCRSize == maxConnectedRegionSize) {
        directionIndexes.insert(i);
      }
    }
  }

  // Prunes.
  bool shouldPrune = false;
  if (maxConnectedRegionSize + tmpResultSize < resultSize) {
    shouldPrune = true;
  } else if (maxConnectedRegionSize + tmpResultSize == resultSize) {
    for (int i = 0; i < tmpResultSize; i++) {
      if (tmpResult[i] < result[i]) {
        shouldPrune = true;
        break;
      } else if (tmpResult[i] > result[i]) {
        break;
      }
    }
  }

  if (!shouldPrune) {
    for (int i : directionIndexes) {
      const pair<int, int>& direction = directions[i];
      dfs(r + direction.first, c + direction.second);
    }
  }

  tmpResultSize--;
  visited[r][c] = false;
}

bool isValid(char c) {
  if (c == '#') {
    return true;
  }
  if (c >= '1' && c <= '9') {
    return true;
  }
  return false;
}

int main() {
  while (scanf("%d %d", &rows, &cols) == 2) {
    if (rows == 0 && cols == 0) {
      break;
    }
    for (int row = 0; row < rows; row++) {
      for (int col = 0; col < cols; col++) {
        char c;
        while (scanf("%c", &c) == 1 && !isValid(c)) {};
        maze[row][col] = c;
      }
    }

    // Initialize.
    resultSize = 0;
    tmpResultSize = 0;
    for (int row = 0; row < rows; row++) {
      for (int col = 0; col < cols; col++) {
        visited[row][col] = false;
      }
    }

    // DFS.
    for (int row = 0; row < rows; row++) {
      for (int col = 0; col < cols; col++) {
        if (maze[row][col] != '#') {
          dfs(row, col);
        }
      }
    }

    // Print result.
    for (int i = 0; i < resultSize; i++) {
      printf("%c", result[i]);
    }
    printf("\n");
  }
  return 0;
}