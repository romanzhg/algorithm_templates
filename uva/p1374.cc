// Iterative deepening search.
// The key optimization: check only the latest generated number, this is not well
// understood. p1374_1 implements a easier to understand version(but much slower).
// 
// Since this optimization is used, there is no duplication check, because there
// will be little duplicated states.
// 
// There are some other optimizations available, those optimizations
// depend on the graph structure and properties of the target state.
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

int target;

// Return true if the target is found.
bool dls(int depth, set<int, greater<int>>& numbers, int toCheck) {
  if (depth == 0) {
    return numbers.find(target) != numbers.end();
  }

  // Prune.
  int largest = *numbers.begin();
  if (largest > 2048) {
    return false;
  }
  if (numbers.size() != 1 && (largest << depth) < target) {
    return false;
  }

  // Expand.
  for (auto it1 = numbers.begin(); it1 != numbers.end(); it1++) {
    int newNumber0 = toCheck + *it1;
    if (numbers.find(newNumber0) == numbers.end()) {
      numbers.insert(newNumber0);
      if (dls(depth - 1, numbers, newNumber0)) {
        return true;
      }
      numbers.erase(newNumber0);
    }

    int newNumber1 = toCheck - *it1;
    if (newNumber1 > 0 && numbers.find(newNumber1) == numbers.end()) {
      numbers.insert(newNumber1);
      if (dls(depth - 1, numbers, newNumber1)) {
        return true;
      }
      numbers.erase(newNumber1);
    }
  }

  return false;
}

int solve() {
  set<int, greater<int>> numbers = {1};
  for (int depth = 0; depth < 50; depth++) {
    #ifndef ONLINE_JUDGE
    clock_t t;
    t = clock();
    #endif
    if (dls(depth, numbers, 1)) {
      #ifndef ONLINE_JUDGE
      t = clock() - t;
      printf ("Level %d took %f seconds.\n", depth, ((float)t) / CLOCKS_PER_SEC);
      #endif
      return depth;
    }
    #ifndef ONLINE_JUDGE
    t = clock() - t;
    printf ("Level %d took %f seconds.\n", depth, ((float)t) / CLOCKS_PER_SEC);
    #endif
  }
  return 0;
}

int main() {
  while (scanf("%d", &target) == 1 && target != 0) {
    printf("%d\n", solve());
  }
  return 0;
}