/*
This approach is coverting the problem to a vertex cover problem.
However, the vertex set may be huge, since it is in the order of
2^(sticks count).
This approach doesn't work.
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

#define MAXN 100

int stickCount;
int sticks[MAXN];

void dfs(vector<set<int>>& result, set<int>& tmpResult, int targetLen, int curIndex) {
  if (targetLen < 0) {
    return;
  }

  if (targetLen == 0) {
    result.push_back(tmpResult);
    return;
  }

  // Check the target first, then check index.
  if (curIndex >= stickCount) {
    return;
  }

  // Take.
  tmpResult.insert(curIndex);
  dfs(result, tmpResult, targetLen - sticks[curIndex], curIndex + 1);

  tmpResult.erase(curIndex);
  // Does not take.
  dfs(result, tmpResult, targetLen, curIndex + 1);
}

vector<vector<int>> getAdjList(int targetLen) {
  vector<set<int>> combinations;
  set<int> tmpResult;
  dfs(combinations, tmpResult, targetLen, 0);

  int nodesSize = combinations.size();
  vector<vector<int>> rtn(nodesSize);
  for (int i = 0; i < nodesSize; i++) {
    for (int j = i + 1; j < nodesSize; j++) {
      const set<int>& setI = combinations[i];
      const set<int>& setJ = combinations[j];
      for (int e : setI) {
        if (setJ.find(e) != setJ.end()) {
          rtn[i].push_back(j);
          rtn[j].push_back(i);
          break;
        }
      }
    }
  }
  return rtn;
}

int getAvailableSize(int* available, int limit, int curIndex) {
  int rtn = 0;
  for (int i = curIndex; i < limit; i++) {
    if (available[i] > 0) {
      rtn++;
    }
  }
  return rtn;
}

int getFirstavailable(int* available, int limit, int curIndex) {
  for (int i = curIndex; i < limit; i++) {
    if (available[i] > 0) {
      return i;
    }
  }
  return -1;
}

bool canSelect(const vector<vector<int>>& adjList, int count,
  int* available, int curIndex) {
  if (count == 0) {
    return true;
  }

  if (curIndex >= adjList.size()) {
    return false;
  }

  // Take.
  // The first available index needs to be equal or larger than curIndex.
  int availableSize = getAvailableSize(available, adjList.size(), curIndex);
  if (availableSize < count) {
    return false;
  }

  int firstAvailable = getFirstavailable(available, adjList.size(), curIndex);
  if (firstAvailable == -1) {
    return false;
  }

  available[firstAvailable]--;
  for (int tmp : adjList[firstAvailable]) {
    available[tmp]--;
  }

  if (canSelect(adjList, count - 1, available, firstAvailable + 1)) {
    return true;
  }

  available[firstAvailable]++;
  for (int tmp : adjList[firstAvailable]) {
    available[tmp]++;
  }

  // Do not take.
  if (canSelect(adjList, count, available, curIndex + 1)) {
    return true;
  }
  return false;
}

void solve() {
  int sum = 0;
  for (int i = 0; i < stickCount; i++) {
    sum += sticks[i];
  }
  sort(sticks, sticks + stickCount, std::greater<int>());

  int len;

  for (len = sticks[0]; len <= sum; len++) {
    if (sum % len != 0) {
      continue;
    }

    int originalSticks = sum / len;
    vector<vector<int>> adjList = getAdjList(len);

    // C++ notes: this is effectively a new operator, some memory in heap
    // is allocated.
    int available[adjList.size()];
    for (int i = 0; i < adjList.size(); i++) {
      available[i] = 1;
    }

    if (canSelect(adjList, originalSticks, available, 0)) {
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