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

#define MAXN 250

int peoplecount;
vector<int> tree[MAXN];

// dp[u][0], for a tree with u at root, if u is not chosen, maximum people.
// dp[u][1], for a tree with u at root, if u is chosen, maximum people.
int dp[MAXN][2];

// unique[u][0], for a tree with u at root, if u is not
// chosen, is the result unique.
// unique[u][1], for a tree with u at root, if u is chosen, is the result unique.
// The program ensures if dp[u][i] is calculated, unique[u][i] is also calculated.
bool uni[MAXN][2];

bool isLeaf(int n) {
  return tree[n].size() == 0;
}

int getValue(int n, int u) {
  if (dp[n][u] > 0) {
    return dp[n][u];
  }

  // Calculate.
  if (isLeaf(n)) {
    if (u == 0) {
      dp[n][u] = 0;
      uni[n][u] = true;
    } else {
      dp[n][u] = 1;
      uni[n][u] = true;
    }
    return dp[n][u];
  }

  int sum = 0;
  bool isUnique = true;
  if (u == 0) {
    // Do not chose the current node.
    for (int child : tree[n]) {
      int tmp0 = getValue(child, 0);
      int tmp1 = getValue(child, 1);
      if (tmp0 == tmp1) {
        isUnique = false;
        sum += tmp0;
      } else if (tmp0 > tmp1) {
        sum += tmp0;
        isUnique = isUnique && uni[child][0];
      } else {
        sum += tmp1;
        isUnique = isUnique && uni[child][1];
      }
     }

  } else {
    // Chose the current node.
    for (int child : tree[n]) {
      sum += getValue(child, 0);
      isUnique = isUnique && uni[child][0];
    }
    sum += 1;
  }
  dp[n][u] = sum;
  uni[n][u] = isUnique;
  return dp[n][u];
}

void solve() {
  for (int i = 0; i < MAXN; i++) {
    for (int j = 0; j < 2; j++) {
      dp[i][j] = -1;
    }
  }
  dp[0][0] = getValue(0, 0);
  dp[0][1] = getValue(0, 1);

  if (dp[0][0] == dp[0][1]) {
    printf("%d No\n", dp[0][0]);
  } else if (dp[0][0] > dp[0][1]){
    printf("%d ", dp[0][0]);
    if (uni[0][0]) {
      printf("Yes\n");
    } else {
      printf("No\n");
    }
  } else {
    printf("%d ", dp[0][1]);
    if (uni[0][1]) {
      printf("Yes\n");
    } else {
      printf("No\n");
    }
  }


}

int main() {
  while (scanf("%d", &peoplecount) == 1 && peoplecount != 0) {
    for (int i = 0; i < peoplecount; i++) {
      tree[i].clear();
    }

    char nameEmployee[150], nameBoss[150];
    map<string, int> nameToId;

    int counter = 0;
    scanf("%s", nameEmployee);
    nameToId.insert({nameEmployee, counter++});
    for (int i = 1; i < peoplecount; i++) {
      scanf("%s %s", nameEmployee, nameBoss);
      if (nameToId.find(nameEmployee) == nameToId.end()) {
        nameToId.insert({nameEmployee, counter++});
      }
      if (nameToId.find(nameBoss) == nameToId.end()) {
        nameToId.insert({nameBoss, counter++});
      }
      int idEmployee = nameToId[nameEmployee];
      int idBoss = nameToId[nameBoss];
      tree[idBoss].push_back(idEmployee);
    }
    solve();
  }
  return 0;
}