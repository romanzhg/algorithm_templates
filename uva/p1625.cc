// A lesson learned here is do not use memset to initialize matrix if
// the matrix size is large an normal test cases are small.
// This would waste a lot of time. memset a 100MB area would take 0.06
// seconds.
// There are two solutions to this problem, the first is not to avoid
// initialization by revise the write order. The second is to use a
// for loop to only initialize those needed in the current test case.
// 
// The key to this problem is dp[i][j] refers to the min value of
// all "appeared but not ended colors".
// colors[i][j] refers to the count of "appeared but not ended colors",
// it is effectively the cost of transition.
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

#define MAXN 5500

vector<char> a1, a2;
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

bool startedNotEnded(int c, int i, int j,
  const vector<pair<int, int>>& colorsA1,
  const vector<pair<int, int>>& colorsA2) {
  bool started = (i >= colorsA1[c].first) || (j >= colorsA2[c].first);
  bool ended = (i >= colorsA1[c].second) && (j >= colorsA2[c].second);
  return (started == true) && (ended == false);
}

void solve() {
  set<int> colorSet;

  vector<pair<int, int>> colorsA1, colorsA2;

  colorsA1 = vector<pair<int, int>>(26, {INF, -1});
  colorsA2 = vector<pair<int, int>>(26, {INF, -1});

  for (int i = 1; i <= a1Size; i++) {
    int index = a1[i] - 'A';
    colorsA1[index].first = min(colorsA1[index].first, i);
    colorsA1[index].second = max(colorsA1[index].second, i);
    colorSet.insert(index);
  }
  for (int i = 1; i <= a2Size; i++) {
    int index = a2[i] - 'A';
    colorsA2[index].first = min(colorsA2[index].first, i);
    colorsA2[index].second = max(colorsA2[index].second, i);
    colorSet.insert(index);
  }

  for (int i = 0; i <= a1Size; i++) {
    for (int j = 0; j <= a2Size; j++) {
      int startedColor = 0;
      for (int c : colorSet) {
        if (startedNotEnded(c, i, j, colorsA1, colorsA2)) {
          startedColor++;
        }
      }
      colors[i][j] = startedColor;
    }
  }


  for (int i = 0; i <= a1Size; i++) {
    for (int j = 0; j <= a2Size; j++) {
      dp[i][j] = INF;
    }
  }

  // dp[0][0] means we've taken 0 cars from a2 and 0 cars from a2.
  dp[0][0] = 0;
  for (int i = 0; i <= a1Size; i++) {
    for (int j = 0; j <= a2Size; j++) {
      if (i + 1 <= a1Size) {
        dp[i + 1][j] = min(dp[i + 1][j], dp[i][j] + colors[i + 1][j]);
      }
      if (j + 1 <= a2Size) {
        dp[i][j + 1] = min(dp[i][j + 1], dp[i][j] + colors[i][j + 1]);
      }
    }
  }

  printf("%u\n", dp[a1Size][a2Size]);
}

int main() {
  int testCases;
  cin >> testCases;

  char line[MAXN];

  cin.getline(line, sizeof(line));
  for (int i = 0; i < testCases; i++) {
    cin.getline(line, sizeof(line));
    a1 = vector<char>();
    a1.assign(line, line + strlen(line));
    a1Size = a1.size();
    a1.insert(a1.begin(), ' ');
    
    cin.getline(line, sizeof(line));
    a2 = vector<char>();
    a2.assign(line, line + strlen(line));
    a2Size = a2.size();
    a2.insert(a2.begin(), ' ');

    solve();
  }
  return 0;
}