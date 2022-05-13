// Iterative deepening search.
// This version use set to check for duplication, this would take a lot of
// memory (120M in practice), but could cut the runtime to a lot(34 -> 12, 139 -> 35).
// Other optimizations are not well understood. 
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
// A state is visited and the result is false.
// Having visited here is optional, but it speeds up the check a lot.
set<set<int, greater<int>>> visited;

// Return true if the target is found.
bool dls(int depth, set<int, greater<int>>& numbers) {
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

  // Duplication check.
  if (visited.find(numbers) != visited.end()) {
    return false;
  }

  // Expand.
  for (auto it0 = numbers.begin(); it0 != numbers.end(); it0++) {
    for (auto it1 = it0; it1 != numbers.end(); it1++) {
      int newNumber1 = *it0 - *it1;
      if (newNumber1 > 0 && numbers.find(newNumber1) == numbers.end()) {
        numbers.insert(newNumber1);
        if (dls(depth - 1, numbers)) {
          return true;
        }
        numbers.erase(newNumber1);
      }

      int newNumber0 = *it0 + *it1;
      if (numbers.find(newNumber0) == numbers.end()) {
        numbers.insert(newNumber0);
        if (dls(depth - 1, numbers)) {
          return true;
        }
        numbers.erase(newNumber0);
      }
    }
  }

  if (numbers.size() <= 20 && visited.size() < (1024 * 1024 * 80)) {
    visited.insert(numbers);
  }

  return false;
}

int solve() {
  set<int, greater<int>> numbers = {1};
  for (int depth = 0; depth < 20; depth++) {
    #ifndef ONLINE_JUDGE
    clock_t t;
    t = clock();
    #endif
    visited.clear();
    if (dls(depth, numbers)) {
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