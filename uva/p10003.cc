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

#define MAXN 55

int length;
int cuts;
int data[MAXN];
// dp[i][j]: the min cost of cutting stick i-j.
// the answer is dp[0][cuts + 1].
int dp[MAXN][MAXN];

int main() {
  while (scanf("%d", &length) == 1 && length != 0) {
    scanf("%d", &cuts);

    for (int i = 1; i <= cuts; i++) {
      scanf("%d", &data[i]);
    }
    data[0] = 0;
    data[cuts + 1] = length;

    for (int i = 0; i < MAXN; i++) {
      for (int j = 0; j < MAXN; j++) {
        dp[i][j] = 0;
      }
    }

    for (int len = 2; len <= cuts + 1; len++) {
      for (int start = 0; start + len <= cuts + 1; start++) {
        int end = start + len;
        int tmp = INF;
        for (int cut = start + 1; cut < end; cut++) {
          tmp = min(tmp,
            dp[start][cut] + dp[cut][end] + data[end] - data[start]);
        }
        dp[start][end] = tmp;
      }
    }

    printf("The minimum cutting is %d.\n", dp[0][cuts + 1]);
  }
  return 0;
}