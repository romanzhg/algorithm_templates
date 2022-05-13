// There is another way to implement.
// The graph traversal can be handled by dfs, visiting a node needs to
// provide its parent node also, to prevent backward travel. So we don't
// need to build the rooted tree from the un-rooted tree explicitly.
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

#define MAXN 10010
vector<int> tree[MAXN];
vector<int> adjList[MAXN];

// dp[u][0], for a tree rooted at u, and u is a server, minimum number of servers
// needed for covering.
// dp[u][1], for a tree rooted at u, and u is not a server, and u's parent is a
// server, the minimum number of servers needed for covering
// dp[u][2], for a tree rooted at u, u and u's parent are neither server,
// minimum number of servers needed for the covering.
int dp[MAXN][3];

int maxId;

bool isLeaf(int n) {
  return tree[n].size() == 0;
}

int getValue(int node, int s) {
  if (dp[node][s] >= 0) {
    return dp[node][s];
  }

  // Calculate.
  if (isLeaf(node)) {
    if (s == 0) {
      dp[node][s] = 1;     
    } else if (s == 1) {
      dp[node][s] = 0;
    } else {
      // Max value needs to be MAXN to prevent overflow.
      dp[node][s] = MAXN;
    }
    return dp[node][s];
  }

  if (s == 0) {
    int sum = 0;
    for (int child : tree[node]) {
      sum += min(getValue(child, 0), getValue(child, 1));
    }
    sum += 1;
    dp[node][s] = sum;
  } else if (s == 1) {
    int sum = 0;
    for (int child : tree[node]) {
      sum += getValue(child, 2);
    }
    dp[node][s] = sum;
  } else {
    int v = MAXN;
    for (int child : tree[node]) {
      v = min(v, getValue(node, 1) - getValue(child, 2) + getValue(child, 0));
    }
    dp[node][s] = v;
  }
  return dp[node][s];
}

void solve() {
  int v1 = getValue(1, 0);
  int v2 = getValue(1, 2);
  printf("%d\n", min(v1, v2));
}

int main() {
  int endSymbol;
  while (true) {
    scanf("%d", &maxId);
    for (int i = 1; i <= maxId; i++) {
      tree[i].clear();
      adjList[i].clear();
    }

    int from, to;
    for (int i = 1; i < maxId; i++) {
      scanf("%d %d", &from, &to);
      adjList[from].push_back(to);
      adjList[to].push_back(from);
    }

    if (maxId >= 1) {
      // Build a tree with root from a tree without root.
      queue<int> cur;
      unordered_set<int> visited;
      cur.push(1);
      
      while (!cur.empty()) {
        int node = cur.front(); cur.pop();
        visited.insert(node);
        for (int nextNode : adjList[node]) {
          if (visited.find(nextNode) == visited.end()) {
            tree[node].push_back(nextNode);
            cur.push(nextNode);
          }
        }
      }
      for (int i = 0; i<= maxId; i++) {
        for (int j = 0; j < 3; j++) {
          dp[i][j] = -1;
        }
      }
      solve();
    }

    scanf("%d", &endSymbol);
    if (endSymbol == -1) {
      break;
    }
  }
  return 0;
}