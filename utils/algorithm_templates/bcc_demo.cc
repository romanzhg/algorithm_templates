// Bi-connected component.
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

int pre[MAX_NODES], isCut[MAX_NODES], bccNumber[MAX_NODES];
int dfsClock = 0, bccCount = 0;

vector<int> graph[MAX_NODES];
vector<int> bcc[MAX_NODES];

struct Edge {
  int u, v;
};

stack<Edge> edgeStack;

int dfs(int u, int parent) {
  int lowu = pre[u] = ++dfsClock;
  int childCount = 0;
  for (int i = 0; i < graph[u].size(); i++) {
    int v = graph[u][i];
    Edge e = (Edge){u, v};
    if (!pre[v]) {
      edgeStack.push(e);
      childCount++;
      int lowv = dfs(v, u);
      lowu = min(lowu, lowv);
      if (lowv >= pre[u]) {
        isCut[u] = true;
        bccCount++;
        bcc[bccCount].clear();
        for (;;) {
          Edge x = edgeStack.top(); edgeStack.pop();
          if (bccNumber[x.u] != bccCount){
            bcc[bccCount].push_back(x.u);
            bccNumber[x.u] = bccCount;
          }
          if (bccNumber[x.v] != bccCount){
            bcc[bccCount].push_back(x.v);
            bccNumber[x.v] = bccCount;
          }
          if (x.u == u && x.v == v) {
            break;
          }
        }
      }
    } else if (pre[v] < pre[u] && v != parent) {
      edgeStack.push(e);
      lowu = min(lowu, pre[v]);
    }
  }
  if (parent < 0 && childCount == 1) {
    isCut[u] = false;
  }
  return lowu;
}

void findBCC(int nodeCount) {
  memset(pre, 0, sizeof(pre));
  memset(isCut, 0, sizeof(isCut));
  memset(bccNumber, 0, sizeof(bccNumber));
  dfsClock = 0;
  bccCount = 0;
  for (int i = 0; i < nodeCount; i++) {
    if (!pre[i]) {
      dfs(i, -1);
    }
  }
}

int main() {
  // graph[1].push_back(2);
  // graph[1].push_back(3);
  // graph[4].push_back(2);
  // graph[4].push_back(3);

  // graph[2].push_back(1);
  // graph[2].push_back(3);
  // graph[2].push_back(4);

  // graph[3].push_back(1);
  // graph[3].push_back(2);
  // graph[3].push_back(4);

  graph[1].push_back(2);
  graph[1].push_back(3);

  graph[2].push_back(1);
  graph[2].push_back(3);
  graph[2].push_back(4);

  graph[3].push_back(1);
  graph[3].push_back(2);
  graph[3].push_back(5);

  graph[4].push_back(2);
  graph[4].push_back(6);

  graph[5].push_back(3);
  graph[5].push_back(6);

  graph[6].push_back(4);
  graph[6].push_back(5);

  findBCC(10);

  for (int i = 1; i <= bccCount; i++) {
    for (int vertex : bcc[i]) {
      printf("%d ", vertex);
    }
    printf("\n");
  }
  return 0;
}
