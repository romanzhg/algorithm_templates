// Build all the lattices first, and see if they can fit.
// Use set<set<pair<int, int>>> to store the results.
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

#define MAX_N 15
#define LIMIT 10

int ans[MAX_N][MAX_N][MAX_N];

typedef set<pair<int, int>> State;
// Used for both storing states and repetition check.
set<State> states[MAX_N];

int xDirections[4] = {-1, 1, 0, 0};
int yDirections[4] = {0, 0, -1, 1};

void printState(const State& s) {
  for (const pair<int, int>& p : s) {
    printf("elem: f: %d s: %d ", p.first, p.second);
  }
  printf("\n");
}

// Make min x and min y are both 0.
// So all lattice has positive coordinates.
State normalize(const State& s) {
  State rtn;
  int minX = INF, minY = INF;
  for (const pair<int, int>& elem : s) {
    minX = min(minX, elem.first);
    minY = min(minY, elem.second);
  }
  for (const pair<int, int>& elem : s) {
    rtn.insert(make_pair(elem.first - minX, elem.second - minY));
  }
  return rtn;
}

// Flip, (x, y) -> (x, -y).
State flip(const State& s) {
  State rtn;
  for (const pair<int, int>& elem : s) {
    rtn.insert(make_pair(elem.first, -elem.second));
  }
  return normalize(rtn);
}

// Rotate 90 degrees clockwise, (x, y) -> (y, -x).
// Note, rotate 90 degrees counter clockwise: (-y, x).
State rotate(State& s) {
  State rtn;
  for (const pair<int, int>& elem : s) {
    rtn.insert(make_pair(elem.second, -elem.first));
  }
  return normalize(rtn);
}

// The input parameter "next" should be normalized.
void tryInsert(const State& next, set<State>& curStates) {
  // Originals.
  State tmp = normalize(next);
  State tmpFlipped = flip(tmp);
  
  // Check originals and the rotated versions. If any version is already
  // in the set, abort. Insert the original version at last.
  // Check originals.
  int ctn = 3;
  do {
    if (curStates.find(tmp) != curStates.end()) {
      return;
    }
    if (curStates.find(tmpFlipped) != curStates.end()) {
      return;
    }
    tmp = rotate(tmp);
    tmpFlipped = rotate(tmpFlipped);
  } while (ctn--);
  curStates.insert(tmp);
}

// Only expand one level.
void expandOneLevel(State& cur, set<State>& curStates) {
  // Expand current state by adding one more tile.
  for (const pair<int, int>& node : cur) {
    // Try 4 neighbours of this node, if they are not in the current state,
    // then insert it.
    for (int i = 0; i < 4 ; i++) {
      pair<int, int> tmp = node;
      tmp.first += xDirections[i];
      tmp.second += yDirections[i];
      if (cur.find(tmp) == cur.end()) {
        auto itPair = cur.insert(tmp);
        tryInsert(cur, curStates);
        cur.erase(itPair.first);
      }
    }
  }
}

// bool canFitInternal(const State& cur, int w, int h) {
//   for (const pair<int, int>& p : cur) {
//     if (p.first >= w) {
//       return false;
//     }
//     if (p.second >= h) {
//       return false;
//     }
//   }
//   return true;
// }

// bool canFit(const State& cur, int w, int h) {
//   // Originals.
//   State tmp = cur;
//   State tmpFlipped = flip(tmp);

//   int ctn = 3;
//   do {
//     if (canFitInternal(tmp, w, h)) {
//       return true;
//     }
//     if (canFitInternal(tmpFlipped, w, h)) {
//       return true;
//     }
//     tmp = rotate(tmp);
//     tmpFlipped = rotate(tmpFlipped);
//   } while (ctn--);
//   return false;
// }

// canFit, compare each side.
bool canFit(const State& cur, int w, int h) {
  int maxX = 0, maxY = 0;
  for (const pair<int, int>& p : cur) {
    maxX = max(maxX, p.first);
    maxY = max(maxY, p.second);
  }
  if (maxX < maxY) {
    swap(maxX, maxY);
  }
  if (w < h) {
    swap(w, h);
  }
  if (maxX < w && maxY < h) {
    return true;
  } else {
    return false;
  }
}

void buildAns() {
  // Build the state of n tiles.
  State start;
  start.insert(make_pair(0, 0));
  states[1].insert(start);

  // The n lattice animal should be built from the n - 1 lattice animal, since
  // we only care about the distinct ones.
  for (int i = 2; i <= LIMIT; i++) {
    
    for (State s : states[i-1]) {
      expandOneLevel(s, states[i]);
    }
  }

  // For each n, see how may of them can fit into the board of w * h.
  // When inserting, should try all the forms of a single state.
  for (int n = 1; n <= LIMIT; n++) {
    for (int w = 1; w <= LIMIT; w++) {
      for (int h = 1; h <= LIMIT; h++) {
        int ctn = 0;
        for (const State& s : states[n]) {
          if (canFit(s, w, h)) {
            ctn++;
          }
        }
        ans[n][w][h] = ctn;
      }
    }
  }
}

int main() {
  buildAns();
  int n, w, h;
  while (scanf("%d %d %d", &n, &w, &h) == 3) {
    printf("%d\n", ans[n][w][h]);
  }
  return 0;
}