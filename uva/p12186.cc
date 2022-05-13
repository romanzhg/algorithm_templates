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

#define MAXN 100010

vector<int> tree[MAXN];
int employeeCount;
int percentage;

// Return the min workers need to sign in order to have person id to sign.
// For workers this will return 1.
int dp(int id) {
  int reporterCount = tree[id].size();
  if (reporterCount == 0) {
    return 1;
  }

  int needToSign = (reporterCount * percentage - 1) / 100 + 1;

  vector<int> results;
  for (int reporter : tree[id]) {
    results.push_back(dp(reporter));
  }

  sort(results.begin(), results.end());

  int rtn = 0;
  for (int i = 0; i < needToSign; i++) {
    rtn += results[i];
  }
  return rtn;
}

int main () {
  while (scanf("%d %d", &employeeCount, &percentage) == 2 &&
    (employeeCount + percentage) != 0) {
    for (int i = 0; i <= employeeCount; i++) {
      tree[i].clear();
    }
    for (int i = 1; i <= employeeCount; i++) {
      int parent;
      scanf("%d", &parent);
      tree[parent].push_back(i);
    }
    printf("%d\n", dp(0));
  }
  return 0;
}