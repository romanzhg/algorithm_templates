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
#define INF 1000

#define MAXN 10

vector<pair<int, int>> directions = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};

int cons[MAXN + 1][MAXN + 1];
int values[MAXN][MAXN];
bool visited[MAXN][MAXN];
int sizeN;

struct Elem {
  int r, c;
  Elem(int r = 0, int c = 0) : r(r), c(c) {
  };

  // Cells with more constrains goes first.
  // Please note if cells with constrain value 0 goes first, for the oj
  // test cases this program will TLE.
  bool operator < (const Elem& o) const {
    int consCount = 0;
    int oConsCount = 0;
    for (int i = 0; i < directions.size(); i++) {
      int newR = r + directions[i].first;
      int newC = c + directions[i].second;
      if (cons[newR][newC] == INF) {
        continue;
      } else {
        consCount++;
      }
    }

    for (int i = 0; i < directions.size(); i++) {
      int newR = o.r + directions[i].first;
      int newC = o.c + directions[i].second;
      if (cons[newR][newC] == INF) {
        continue;
      } else {
        oConsCount++;
      }
    }
    return consCount > oConsCount;
  }
};

// Decide if a cell is out of range.
bool isCellOutOfRange(int r, int c) {
  return r < 0 || r >= sizeN || c < 0 || c >= sizeN;
}

// r, c, startR, startC: points.
// tmpR, tmpC: tile index.
bool dfsHasCycle(int r, int c, int startR, int startC) {  
  if (startR == r && startC == c) {
    return true;
  }

  int tmpR, tmpC;
  // Four cells to expand to.
  tmpR = startR - 1;
  tmpC = startC - 1;
  if (!isCellOutOfRange(tmpR, tmpC)
    && !visited[tmpR][tmpC]
    && values[tmpR][tmpC] == 2) {
    visited[tmpR][tmpC] = true;
    if (dfsHasCycle(r, c, startR - 1, startC - 1)) {
      return true;
    }
  }

  tmpR = startR - 1;
  tmpC = startC;
  if (!isCellOutOfRange(tmpR, tmpC)
    && !visited[tmpR][tmpC]
    && values[tmpR][tmpC] == 1) {
    visited[tmpR][tmpC] = true;
    if (dfsHasCycle(r, c, startR - 1, startC + 1)) {
      return true;
    }
  }

  tmpR = startR;
  tmpC = startC - 1;
  if (!isCellOutOfRange(tmpR, tmpC)
    && !visited[tmpR][tmpC]
    && values[tmpR][tmpC] == 1) {
    visited[tmpR][tmpC] = true;
    if (dfsHasCycle(r, c, startR + 1, startC - 1)) {
      return true;
    }
  }

  tmpR = startR;
  tmpC = startC;
  if (!isCellOutOfRange(tmpR, tmpC)
    && !visited[tmpR][tmpC]
    && values[tmpR][tmpC] == 2) {
    visited[tmpR][tmpC] = true;
    if (dfsHasCycle(r, c, startR + 1, startC + 1)) {
      return true;
    }
  }
  return false;
}

bool hasCycle(int r, int c) {
  memset(visited, 0, sizeof(visited));
  visited[r][c] = true;
  int curR, curC, startR, startC;
  if (values[r][c] == 1) {
    curR = r;
    curC = c + 1;
    startR = r + 1;
    startC = c;
  } else {
    curR = r;
    curC = c;
    startR = r + 1;
    startC = c + 1;
  }
  bool rtn = dfsHasCycle(curR, curC, startR, startC);
  return rtn;
}

// A point is not satisfied if all the adjacent cells are set
// and the point constrain is not zero.
bool pointConstrainSatisfied(int r, int c) {
  vector<pair<int, int>> adjCells = {
    {r - 1, c - 1},
    {r, c - 1},
    {r - 1, c},
    {r, c}};

  if (cons[r][c] > 100) {
    return true;
  }
  if (cons[r][c] < 0) {
    return false;
  }
  for (int i = 0; i < 4; i++) {
    const pair<int, int> cell = adjCells[i];
    if (isCellOutOfRange(cell.first, cell.second)) {
      continue;
    }
    if (values[cell.first][cell.second] == 0) {
      return true;
    }
  }
  return cons[r][c] == 0;
}

// For cell (r, c), the four constrains on its vertexs are satisfied.
bool satisfied(int r, int c) {
  return pointConstrainSatisfied(r, c)
    && pointConstrainSatisfied(r + 1, c)
    && pointConstrainSatisfied(r, c + 1)
    && pointConstrainSatisfied(r + 1, c + 1);
}

bool dfs(const vector<Elem>& elems, int index) {
  if (index >= elems.size()) {
    return true;
  }

  int r = elems[index].r;
  int c = elems[index].c;

  values[r][c] = 1;
  cons[r + 1][c]--;
  cons[r][c + 1]--;

  // State can be checked either before going to next level
  // or as the first thing after going to next step.
  // If the condition checked is related to the action just taken,
  // maybe check it before, since we don't need to pass the action
  // to the next level.
  // If the condition checked is not related to the action just taken,
  // check it after results in more concise code.
  if (satisfied(r, c) && !hasCycle(r, c)) {
    if (dfs(elems, index + 1)) {
      return true;
    }
  }

  cons[r + 1][c]++;
  cons[r][c + 1]++;

  values[r][c] = 2;
  cons[r][c]--;
  cons[r + 1][c + 1]--;

  if (satisfied(r, c) && !hasCycle(r, c)) {
    if (dfs(elems, index + 1)) {
      return true;
    }
  }
  cons[r][c]++;
  cons[r + 1][c + 1]++;

  values[r][c] = 0;
  return false;
}

void solve() {
  vector<Elem> elems;
  for (int i = 0; i < sizeN; i++) {
    for (int j = 0; j < sizeN; j++) {
      elems.push_back(Elem(i, j));
    }
  }
  sort(elems.begin(), elems.end());

  dfs(elems, 0);

  for (int i = 0; i < sizeN; i++) {
    for (int j = 0; j < sizeN; j++) {
      if (values[i][j] == 1) {
        printf("/");
      } else if (values[i][j] == 2) {
        printf("\\");
      }
    }
    printf("\n");
  }
}

bool isValid(char c) {
  return c == '.' || isdigit(c);
}

int main() {
  #ifndef ONLINE_JUDGE
  clock_t t;
  t = clock();
  #endif

  int testCases;
  scanf("%d", &testCases);
  while(testCases--) {
    scanf("%d", &sizeN);
    for (int i = 0; i < sizeN + 1; i++) {
      for (int j = 0; j < sizeN + 1; j++) {
        char c;
        while (scanf("%c", &c) == 1 && !isValid(c)) {};
        if (c == '.') {
          cons[i][j] = INF;
        } else if (isdigit(c)) {
          cons[i][j] = (int)(c - '0');
        }
      }
    }
    memset(values, 0, sizeof(values));
    solve();
  }
  #ifndef ONLINE_JUDGE
  t = clock() - t;
  printf ("This program took %f seconds.\n", ((float)t) / CLOCKS_PER_SEC);
  #endif

  return 0;
}