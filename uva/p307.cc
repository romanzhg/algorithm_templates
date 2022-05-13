/*
An example why greedy cannot work
8
43 33 30 46 35 41 27 33 
0
An example why one number by one number cannot work
36
42 40 24 25 39 12 25 40 10 43 23 21 40 5 17 35 16 9 35 50 38 31 12 47 22 47 9 24 23 2 43 16 24 24 3 14 
0
38 47 47 16 24 14 will be left, which addes us to 93 * 2, but can not be separated to two 93.
This version is correct but slow, to be improved.

The key to this solution is prunings.
1. Sort the sticks.
2. Use start index to give ordering.
3. The first stick to rebuild an original stick should be fixed.
4. If a previous stick with same length was not chosen, do not chose this one.
With out any of these pruning, the solution will TLE.

p307_1 implements a solution based on vertex cover, but that one is much less efficient.
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

#define MAXN 150

int stickCount;
int sticks[MAXN];
int originalLen;

bool fillin(int remainingSticks, int len, int startIndex, bool* used) {
  if (remainingSticks == 0) {
    return true;
  }

  if (len == 0) {
    return fillin(remainingSticks - 1, originalLen, 0, used);
  }

  if (true) {
    // Pruning.
    // A special case, for a new stick, always take the first available.
    for (int i = startIndex; i < stickCount; i++) {
      if (used[i] == false) {
        used[i] = true;

        if (fillin(remainingSticks, len - sticks[i], i + 1, used)) {
          return true;
        }

        used[i] = false;
        return false;
      }
    }
    return false;
  } else {
    for (int i = startIndex; i < stickCount; i++) {
      // Pruning.
      // If the last stick with same length was not taken, don't take this one.
      if (i > 0 && used[i - 1] == false && sticks[i] == sticks[i-1]) {
        continue;
      }
      if (used[i] == false && len >= sticks[i]) {
        used[i] = true;

        if (fillin(remainingSticks, len - sticks[i], i + 1, used)) {
          return true;
        }

        used[i] = false;
      }
    }
    return false;
  }
}

void solve() {
  int sum = 0;
  for (int i = 0; i < stickCount; i++) {
    sum += sticks[i];
  }

  sort(sticks, sticks + stickCount, std::greater<int>());

  int len;
  bool used[stickCount];

  for (len = sticks[0]; len <= sum; len++) {
    if (sum % len != 0) {
      continue;
    }

    memset(used, 0, sizeof(used));
    originalLen = len;
    if (fillin(sum / len, len, 0, used)) {
      break;
    }
  }

  printf("%d\n", len);
}

int main() {
  while (scanf("%d", & stickCount) == 1 && stickCount != 0) {
    for (int i = 0; i < stickCount; i++) {
      scanf("%d", &sticks[i]);
    }
    solve();
  }
  return 0;
}
