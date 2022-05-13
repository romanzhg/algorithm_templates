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

#define MAXN 250

int rows, cols;
int values[MAXN][MAXN];
// dp[r][c]: min cost to arrive at cell (r, c).
int dp[MAXN][MAXN];
int path[MAXN][MAXN];

int tmpIndexes[3];
int pathToPrint[MAXN];

// C is current col.
string getPath(int prevR, int col) {
  vector<int> tmpPath;
  tmpPath.push_back(prevR + 1);

  for (int c = col -1; c >= 1; c--) {
    prevR = path[prevR][c];
    tmpPath.push_back(prevR);
  }
  reverse(tmpPath.begin(), tmpPath.end());
  string rtn = "";
  for (int v : tmpPath) {
    rtn += v;
    rtn += " ";
  }
  return rtn;
}

int main() {
  while (scanf("%d %d", &rows, &cols) == 2) {
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        scanf("%d", &values[i][j]);
        dp[i][j] = INF;
      }
    }

    for (int r = 0; r < rows; r++) {
      dp[r][0] = values[r][0];
      path[r][0] = -1;
    }

    for (int c = 1; c < cols; c++) {
      for (int r = 0; r < rows; r++) {

        for (int d = -1; d <= 1; d++) {
          int prevR = (r + d) % rows;
          if (prevR < 0) {
            prevR += rows;
          }
          tmpIndexes[d + 1] = prevR;
        }
        sort(tmpIndexes, tmpIndexes + 3);

        for (int d = 0; d < 3; d++) {
          int prevR = tmpIndexes[d];
          if (dp[r][c] > (dp[prevR][c-1] + values[r][c])) {
            dp[r][c] = dp[prevR][c-1] + values[r][c];
            path[r][c] = prevR;
          } else if (dp[r][c] == (dp[prevR][c-1] + values[r][c])) {
            if (getPath(prevR, c) < getPath(path[r][c], c)) {
              path[r][c] = prevR;
            }
          }
        }
      }
    }

    int ans = INF;
    for (int r = 0; r < rows; r++) {
      ans = min(ans, dp[r][cols - 1]);
    }

    set<vector<int>> results;
    for (int r = 0; r < rows; r++) {
      if (dp[r][cols - 1] == ans) {
        int count = 0;
        int minRowIndex = r;
        pathToPrint[count++] = minRowIndex + 1;
        for (int c = cols - 1; c >= 1; c--) {
          minRowIndex = path[minRowIndex][c];
          pathToPrint[count++] = minRowIndex + 1;
        }
        reverse(pathToPrint, pathToPrint + cols);
        results.insert(vector<int>(pathToPrint, pathToPrint + cols));
      }
    }

    vector<int> result = *(results.begin());
    for (int i = 0; i < cols - 1; i++) {
      printf("%d ", result[i]);
    }
    printf("%d", result[cols - 1]);
    printf("\n");
    printf("%d\n", ans);
  }
  return 0;
}