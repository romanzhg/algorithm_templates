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
int cube[MAXN][3];
int cubeCount;

int dp[MAXN][3];

// Get length the width of a square.
// l is larger or equal to w.
void getDimension(int index, int k, int& l, int& w) {
  if (k == 0) {
    l = cube[index][1];
    w = cube[index][2];
  } else if (k == 1) {
    l = cube[index][0];
    w = cube[index][2];
  } else {
    l = cube[index][0];
    w = cube[index][1];
  }
  if (w > l) {
    swap(w, l);
  }
}

// Return if the square (index1, k1) can hold the square (index2, k2).
bool canHold(int index1, int k1, int index2, int k2) {
  // a > b.
  int l1, w1, l2, w2;
  getDimension(index1, k1, l1, w1);
  getDimension(index2, k2, l2, w2);

  if (l1 > l2 && w1 > w2) {
    return true;
  } else {
    return false;
  }
}

int getValue(int i1, int k1) {
  if (dp[i1][k1] > 0) {
    return dp[i1][k1];
  }

  int height = cube[i1][k1];
  dp[i1][k1] = height;
  for (int i2 = 0; i2 < cubeCount; i2++) {
    for (int k2 = 0; k2 < 3; k2++) {
      if (canHold(i2, k2, i1, k1)) {
        dp[i1][k1] = max(dp[i1][k1], getValue(i2, k2) + height);
      } 
    }
  }

  return dp[i1][k1];
}

int main() {
  int testCase = 0;
  while (scanf("%d", &cubeCount) == 1 && cubeCount != 0) {
    for (int i = 0; i < cubeCount; i++) {
      for (int j = 0; j < 3; j++) {
        scanf("%d", &cube[i][j]);
      }
    }
    for (int i = 0; i < MAXN; i++) {
      for (int k = 0; k < 3; k++) {
        dp[i][k] = -1;
      }
    }

    int ans = 0;
    for (int i1 = 0; i1 < cubeCount; i1++) {
      for (int k1 = 0; k1 < 3; k1++) {
        ans = max(ans, getValue(i1, k1));
      }
    }
    printf("Case %d: maximum height = %d\n", ++testCase, ans);
  }
  return 0;
}