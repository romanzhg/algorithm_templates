#include <cstdio>
#include <cstdlib>
#include <climits>
#include <queue>
#include <vector>
#include <utility>
#include <algorithm>
#include <functional>
#include <cstring>
#include <string>
#include <map>
// unordered_map is not supported.
using namespace std;

#define N 20000+10
#define E 50000+10

struct Edge {
  int from;
  int to;
  int cost;
};

int parent[N];
int rank_[N];

class CompEdge {
 public:
  bool operator() (const Edge& lhs, const Edge& rhs) {
    return lhs.cost > rhs.cost;
  }
};

priority_queue<Edge, vector<Edge>, CompEdge> pq;

void addEdge(int from, int to, int cost) {
  Edge e;
  e.from = from;
  e.to = to;
  e.cost = cost;
  pq.push(e);
}

int getParent(int a) {
  if (a == parent[a]) {
    return a;
  } else {
    parent[a] = getParent(parent[a]);
    return parent[a];
  }
}

void join(int a, int b) {
  if (rank_[a] == rank_[b]) {
    parent[b] = a;
    rank_[a]++;
  } else if (rank_[a] > rank_[b]) {
    parent[b] = a;
  } else {
    parent[a] = b;
  }
}


void solve(int totalNodes) {
  int totalCost = 0;
  for (int i = 0; i < totalNodes; i++) {
    parent[i] = i;
    rank_[i] = 0;
  }

  while (!pq.empty()) {
    Edge e = pq.top();
    pq.pop();
    if (getParent(e.from) == getParent(e.to)) {
      continue;
    } else {
      join(getParent(e.from), getParent(e.to));
      totalCost += e.cost;
    }
  }

  printf("%d\n", totalNodes * 10000 + totalCost);
}

int main() {
  int testCases;
  int females, males, relations;

  scanf("%d", &testCases);
  for (int i = 0; i < testCases; i++) {
    scanf("%d %d %d", &females, &males, &relations);
    for (int j = 0; j < relations; j++) {
      int rFemale, rMale, relationScore;
      scanf("%d %d %d", &rFemale, &rMale, &relationScore);
      addEdge(rFemale, females + rMale, -relationScore);
    }
    solve(females + males);
  }
  return 0;
}
