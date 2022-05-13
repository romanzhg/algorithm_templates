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

#define MAXN 25

// Represent the graph in connected matrix.
bool connected[MAXN][MAXN];

int target;
int totalPaths;

void dfs(vector<int>& path, set<int>& pathSet) {
  int cur = path.back();
  if (cur == target) {
    totalPaths++;
    for (int i = 0; i < path.size() - 1; i++) {
      printf("%d ", path[i]);
    }
    printf("%d\n", path.back());
  }

  for (int i = 1; i < MAXN; i++) {
    if (connected[cur][i] && (pathSet.find(i) == pathSet.end())) {
      path.push_back(i);
      pathSet.insert(i);
      
      dfs(path, pathSet);

      path.pop_back();
      pathSet.erase(i);
    }
  }
}

bool dfsReachable(int cur, set<int>& visited) {
  if (visited.find(cur) != visited.end()) {
    return false;
  }
  visited.insert(cur);

  if (cur == target) {
    return true;
  }
  
  for (int i = 1; i < MAXN; i++) {
    if (connected[cur][i]) {
      if (dfsReachable(i, visited)) {
        return true;
      }
    }
  }
  return false;
}

int main() {
  int test_case = 1;

  while (scanf("%d", &target) == 1) {
    printf("CASE %d:\n", test_case++);

    memset(connected, 0, sizeof(connected));
    totalPaths = 0;

    int a, b;
    while (scanf("%d %d", &a, &b)) {
      if (a == 0 && b == 0) {
        break;
      }
      connected[a][b] = true;
      connected[b][a] = true;
    }

    set<int> visited;
    if (dfsReachable(1, visited)) {
      vector<int> path;
      set<int> pathSet;
      path.push_back(1);
      pathSet.insert(1);

      dfs(path, pathSet);
    }

    printf("There are %d routes from the firestation to streetcorner %d.\n",
      totalPaths, target);
  }

  return 0;
}