// A slightly different version compare to p1625.
// The order of evaluation is changed, and there is a few tricks on
// processing boundary conditions.
// This version also improve by remove one level of iteration when
// counting colors.
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

#define MAXN 5010

char a1[MAXN], a2[MAXN];

// Number of elements in each array. The elements are indexed [1 ~ size].
int a1Size, a2Size;

// dp[i][j]: the min color dist sum when there are ith elements from the first
// array and jth elements from the second array were taken. 
uint dp[MAXN][MAXN];

// colors[i][j]: number of started but not ended color at state (i, j).
// This is effectively the cost of each state transition.
// Actually when a color is not ended, it should not be counted into
// the result. But this way of calculation uses the fact that all colors
// eventually ends.
int colors[MAXN][MAXN];

int a1Start[26], a1End[26], a2Start[26], a2End[26];

void solve() {
  for (int i = 0; i < 26; i++) {
    a1Start[i] = INF;
    a2Start[i] = INF;
    a1End[i] = -INF;
    a2End[i] = -INF;
  }

  for (int i = 1; i <= a1Size; i++) {
    int tmpColor = a1[i] - 'A';
    a1Start[tmpColor] = min(a1Start[tmpColor], i);
    a1End[tmpColor] = i;
  }
  for (int i = 1; i <= a2Size; i++) {
    int tmpColor = a2[i] - 'A';
    a2Start[tmpColor] = min(a2Start[tmpColor], i);
    a2End[tmpColor] = i;
  }

  colors[0][0] = 0;
  for (int i = 0; i <= a1Size; i++) {
    for (int j = 0; j <= a2Size; j++) {
      if (i == 0 && j == 0) {
        continue;
      }
      if (i != 0) {
        // Update all other rows.
        colors[i][j] = colors[i - 1][j];
        int tmpColor = a1[i] - 'A';
        if (i == a1Start[tmpColor] && j < a2Start[tmpColor]) {
          colors[i][j]++;
        }
        if (i == a1End[tmpColor] && j >= a2End[tmpColor]) {
          colors[i][j]--;
        }
      } else if (j != 0) {
        // Update the first row.
        colors[i][j] = colors[i][j - 1];
        int tmpColor = a2[j] - 'A';
        if (j == a2Start[tmpColor] && i < a1Start[tmpColor]) {
          colors[i][j]++;
        }
        if (j == a2End[tmpColor] && i >= a1End[tmpColor]) {
          colors[i][j]--;
        }
      }
    }
  }

  // dp[0][0] means we've taken 0 cars from a2 and 0 cars from a2.
  dp[0][0] = 0;
  for (int i = 0; i <= a1Size; i++) {
    for (int j = 0; j <= a2Size; j++) {
      if (i == 0 && j == 0) {
        continue;
      }
      int v1 = INF, v2 = INF;
      if (i > 0) {
        v1 = dp[i - 1][j] + colors[i - 1][j];
      }
      if (j > 0) {
        v2 = dp[i][j - 1] + colors[i][j - 1];
      }
      dp[i][j] = min(v1, v2);
    }
  }

  printf("%u\n", dp[a1Size][a2Size]);
}

int main() {
  #ifndef ONLINE_JUDGE
  clock_t t;
  t = clock();
  #endif

  int testCases;
  scanf("%d", &testCases);

  for (int i = 0; i < testCases; i++) {
    scanf("%s %s", a1 + 1, a2 + 1);
    a1Size = strlen(a1 + 1);
    a2Size = strlen(a2 + 1);
    solve();
  }

  #ifndef ONLINE_JUDGE
  t = clock() - t;
  printf ("This program took %f seconds.\n", ((float)t) / CLOCKS_PER_SEC);
  #endif

  return 0;
}