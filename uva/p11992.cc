// Interval tree.
// Note in (cur, l, r), l refers the l-th element. For an array of n elements,
// they are refered as the 1-th element ... n-th element.
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

const int ROW_COUNT = 21;
const int COL_COUNT = 2 * 1024 * 1024;

struct Node {
  int maxVal;
  int minVal;
  int sumVal;
  int addMark;
  int setMark;
};

Node data[ROW_COUNT][COL_COUNT];

int rows, cols, queries;

int globalSum, globalMax, globalMin;

void maintain(Node* nodes, int cur, int l, int r) {
  int lc = cur * 2, rc = cur * 2 + 1;
  if (nodes[cur].setMark >= 0) {
    nodes[cur].minVal = nodes[cur].maxVal = nodes[cur].setMark;
    nodes[cur].sumVal = nodes[cur].setMark * (r - l + 1);
  } else {
    // No set mark, get values from children, and process the current add mark.
    if (r > l) {
      nodes[cur].sumVal = nodes[lc].sumVal + nodes[rc].sumVal;
      nodes[cur].minVal = min(nodes[lc].minVal, nodes[rc].minVal);
      nodes[cur].maxVal = max(nodes[lc].maxVal, nodes[rc].maxVal);
    } else {
      // Leaf node.
      nodes[cur].sumVal = 0;
      nodes[cur].minVal = 0;
      nodes[cur].maxVal = 0;
    }

    // Process the add mark at current level.
    nodes[cur].minVal += nodes[cur].addMark;
    nodes[cur].maxVal += nodes[cur].addMark;
    nodes[cur].sumVal += nodes[cur].addMark * (r - l + 1);
  }
}

void pushdown(Node* nodes, int cur) {
  int lc = cur * 2, rc = cur * 2 + 1;
  // This code assumes the set operation only take values larger than 0,
  // need to verify.
  if (nodes[cur].setMark >= 0) {
    nodes[lc].setMark = nodes[rc].setMark = nodes[cur].setMark;
    nodes[lc].addMark = nodes[rc].addMark = 0;
    nodes[cur].setMark = -1;
  } else if (nodes[cur].addMark > 0) {
    int v = nodes[cur].addMark;
    if (nodes[lc].setMark == -1) {
      nodes[lc].addMark += v;
    } else {
      nodes[lc].setMark += v;
    }
    if (nodes[rc].setMark == -1) {
      nodes[rc].addMark += v;
    } else {
      nodes[rc].setMark += v;
    }
    nodes[cur].addMark = 0;
  }
}

// Functions related to set.
void updateAdd(Node* nodes, int cur, int l, int r, int y1, int y2, int value) {
  int lc = cur * 2, rc = cur * 2 + 1;
  if (y1 <= l && y2 >= r) {
    // Marking an add does not change the existing set.
    if (nodes[cur].setMark >= 0) {
      nodes[cur].setMark += value;
    } else {
      nodes[cur].addMark += value;
      // printf("l, r: (%d, %d), add mark: %d, delta: %d\n", l, r, nodes[cur].addMark, value);
    }
  } else {
    if (nodes[cur].setMark >= 0) {
      pushdown(nodes, cur);
    }
    int m = l + (r - l) / 2;
    if (y1 <= m) {
      updateAdd(nodes, lc, l, m, y1, y2, value);
    } else {
      maintain(nodes, lc, l, m);
    }
    if (y2 > m) {
      updateAdd(nodes, rc, m + 1, r, y1, y2, value);
    } else {
      maintain(nodes, rc, m + 1, r);
    }
  }
  maintain(nodes, cur, l, r);
}

void updateSet(Node* nodes, int cur, int l, int r, int y1, int y2, int value) {
  int lc = cur * 2, rc = cur * 2 + 1;
  if (y1 <= l && y2 >= r) {
    nodes[cur].addMark = 0;
    nodes[cur].setMark = value;
  } else {
    pushdown(nodes, cur);
    int m = l + (r - l) / 2;
    if (y1 <= m) {
      updateSet(nodes, lc, l, m, y1, y2, value);
    } else {
      maintain(nodes, lc, l, m);
    }
    if (y2 > m) {
      updateSet(nodes, rc, m + 1, r, y1, y2, value);
    } else {
      maintain(nodes, rc, m + 1, r);
    }
  }
  maintain(nodes, cur, l, r);
}

// It is possible for set to a smaller range then add to a larger range.
// So set can be a child of add, but add cannot be children of set.
void querySum(Node* nodes, int cur, int l, int r, int y1, int y2, int prevAdd) {
  int lc = cur * 2, rc = cur * 2 + 1;
  if (nodes[cur].setMark >= 0) {
    globalSum += (nodes[cur].setMark + prevAdd) * (min(y2, r) - max(y1, l) + 1);
      // printf("1 added value %d for range (%d, %d), prevAdd: %d\n",
      //   (nodes[cur].setMark + prevAdd) * (min(y2, r) - max(y1, l) + 1),
      //   min(y2, r), max(y1, l), prevAdd);
  } else {
    if (y1 <= l && y2 >= r) {
      globalSum += nodes[cur].sumVal + prevAdd * (r - l + 1);
      // printf("2 added value %d for range (%d, %d), prevAdd: %d\n",
      //   nodes[cur].sumVal + prevAdd * (r - l + 1), l, r, prevAdd);
    } else {
      int m = l + (r - l) / 2;
      if (y1 <= m) {
        querySum(nodes, lc, l, m, y1, y2, prevAdd + nodes[cur].addMark);
      }
      if (y2 > m) {
        querySum(nodes, rc, m + 1, r, y1, y2, prevAdd + nodes[cur].addMark);
      }
    }
  }
}

void queryExtremeValues(Node* nodes, int cur, int l, int r, int y1, int y2, int prevAdd) {
  int lc = cur * 2, rc = cur * 2 + 1;
  if (nodes[cur].setMark >= 0) {
    globalMin = min(globalMin, nodes[cur].minVal + prevAdd);
    globalMax = max(globalMax, nodes[cur].maxVal + prevAdd);
  } else {
    if (y1 <= l && y2 >= r) {
      globalMin = min(globalMin, nodes[cur].minVal + prevAdd);
      globalMax = max(globalMax, nodes[cur].maxVal + prevAdd);
    } else {
      int m = l + (r - l) / 2;
      if (y1 <= m) {
        queryExtremeValues(nodes, lc, l, m, y1, y2,
          prevAdd + nodes[cur].addMark);
      }
      if (y2 > m) {
        queryExtremeValues(nodes, rc, m + 1, r, y1, y2,
          prevAdd + nodes[cur].addMark);
      }
    }
  }
}

int main() {
  while (scanf("%d %d %d", &rows, &cols, &queries) == 3) {
    // Initialize the data. This way of allocation may exceed memory limit.
    // May try to allocate dynamically.
    memset(data, 0, sizeof(data));

    // Initialize all set mark to -1.
    for (int i = 0; i < ROW_COUNT; i++) {
      for (int j = 0; j < COL_COUNT; j++) {
        data[i][j].setMark = -1;
      }
    }

    for (int i = 0; i < queries; i++) {
      int type, x1, x2, y1, y2, value;
      scanf("%d %d %d %d %d", &type, &x1, &y1, &x2, &y2);
      if (type == 1 || type == 2) {
        scanf("%d", &value);
      }

      if (type == 1) {
        for (int j = x1; j <= x2; j++) {
          updateAdd(data[j], 1, 1, cols, y1, y2, value);
        }
      } else if (type == 2) {
        for (int j = x1; j <= x2; j++) {
          updateSet(data[j], 1, 1, cols, y1, y2, value);
        }
      } else {
        globalSum = 0;
        globalMin = INT_MAX;
        globalMax = 0;
        for (int j = x1; j <= x2; j++) {
          querySum(data[j], 1, 1, cols, y1, y2, 0);
          queryExtremeValues(data[j], 1, 1, cols, y1, y2, 0);
        }
        printf("%d %d %d\n", globalSum, globalMin, globalMax);
      }
    }
  }
  return 0;
}