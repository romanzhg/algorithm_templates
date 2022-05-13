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

vector<int> parent, rank__;
vector<pair<int, pair<int, int> > > graph;

int getParent(int a) {
  if (a == parent[a]) {
    return a;
  } else {
    parent[a] = getParent(parent[a]);
    return parent[a];
  }
}

void join(int a, int b) {
  if (rank__[a] == rank__[b]) {
    parent[b] = a;
    rank__[a]++;
  } else if (rank__[a] > rank__[b]) {
    parent[b] = a;
  } else {
    parent[a] = b;
  }
}

void solve(int totalNodes) {
  int totalCost = 0;
  parent = vector<int>(totalNodes, 0);
  for (int i = 0; i < totalNodes; i++) {
    parent[i] = i;
  }
  rank__ = vector<int>(totalNodes, 0);

  sort(graph.begin(), graph.end(), less<pair<int, pair< int, int> > >());
  for (int i = 0; i < graph.size(); i++) {
    int from = graph[i].second.first;
    int to = graph[i].second.second;
    int cost = graph[i].first;
    if (getParent(from) != getParent(to)) {
      join(getParent(from), getParent(to));
      totalCost += cost;
    }
  }

  printf("%d\n", totalNodes * 10000 + totalCost);
  graph.clear();
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
      graph.push_back(pair<int, pair<int, int> >(-relationScore,
        pair<int, int> (rFemale, females + rMale)));
    }
    solve(females + males);
  }
  return 0;
}
