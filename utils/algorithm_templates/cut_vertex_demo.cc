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

const int MAX_NODES = 10;

vector<int> graph[MAX_NODES];
// The order a vertex is visited. Set at DFS previsit.
// the value start from 1. 0 means not visited.
int pre[MAX_NODES];
// The node with the lowest pre value that node u and all its
// descents are connected to.
int low[MAX_NODES];

bool isCut[MAX_NODES];

int dfsClock = 0;

// For the initial vertex, parent should be a negative number.
int dfs(int u, int parent) {
  int lowu = pre[u] = ++dfsClock;
  int childCount = 0;
  for (int i = 0; i < graph[u].size(); i++) {
    int v = graph[u][i];
    if (!pre[v]) {
      childCount++;
      int lowv = dfs(v, u);
      lowu = min(lowu, lowv);
      if (lowv >= pre[u]) {
        isCut[u] = true;
      }
    } else if (pre[v] < pre[u] && v != parent) {
      // Reverse edge. v already visited.
      lowu = min(lowu, pre[v]);
    }
  }
  if (parent < 0 && childCount == 1) {
    isCut[u] = false;
  }
  low[u] = lowu;
  return lowu;
}

int main() {
  // graph[0].push_back(1);
  // graph[1].push_back(0);

  // graph[0].push_back(1);
  // graph[0].push_back(2);
  // graph[1].push_back(0);
  // graph[1].push_back(2);
  // graph[2].push_back(0);
  // graph[2].push_back(1);

  // graph[2].push_back(3);
  // graph[3].push_back(2);
  
  graph[1].push_back(2);
  graph[1].push_back(3);
  graph[2].push_back(1);
  graph[2].push_back(3);

  graph[4].push_back(3);
  graph[4].push_back(5);
  graph[5].push_back(3);
  graph[5].push_back(4);

  graph[3].push_back(1);
  graph[3].push_back(2);
  graph[3].push_back(4);
  graph[3].push_back(5);


  dfs(1, -1);

  for (int i = 1; i < 6; i++) {
    printf("%s\n", isCut[i] ? "true" : "false");
  }
  return 0;

}