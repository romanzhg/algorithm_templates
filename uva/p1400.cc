// Segment tree/interval tree.
// Please note the tree is built recursive, bottom up way,
// with the range of the nodes passed in.
// Both build and query of the maxSub needs to consider 3 cases:
// L and R both in the left interval, both in the right interval, and
// one in left and one in right.
// This structure handles dynamic maximum sum sequence query.
// Please note the standard segment tree handles only sum, this solution
// extends it with some more and became capable for very different thing.
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

struct Node {
  pair<int, int> maxSub;
  // The index corresponds to the maximum sum of [a, b], where the
  // sequence is values[a] + values[a+1] ... + values[x], a <= x <= b.
  int maxPrefix;
  int maxSuffix;
};

const int MAX_SIZE = 500000 + 100;

// Original values.
ll values[MAX_SIZE];
ll sumArray[MAX_SIZE];

// log2(MAX_SIZE) <= 19, so the total array size should be 2^20(1M).
// The other way to look at the array total size: MAX_SIZE_OF_ORIGINAL * 2.
Node nodes[1024 * 1024 + 1];

int dishes, queries;

// Get range sum in the original array.
ll getRangeSum(int l, int r) {
  return sumArray[r] - sumArray[l - 1];
}

int queryMaxPrefix(int index, int l, int r, int qr) {
  int m = l + (r - l) / 2;
  if (qr >= nodes[index].maxPrefix) {
    return nodes[index].maxPrefix;
  } else if (qr > m) {
    int tmpL = nodes[index * 2].maxPrefix;
    int tmpR = queryMaxPrefix(index * 2 + 1, m + 1, r, qr);
    // This comparison ensures the smaller right end is always chosen.
    if (getRangeSum(l, tmpL) < getRangeSum(l, tmpR)) {
      return tmpR;
    } else {
      return tmpL;
    }
  } else {
    // qr <= m
    return queryMaxPrefix(index * 2, l, m, qr);
  }
}

int queryMaxSuffix(int index, int l, int r, int ql) {
  int m = l + (r - l) / 2;
  if (ql <= nodes[index].maxSuffix) {
    return nodes[index].maxSuffix;
  } else if (ql <= m) {
    int tmpL = queryMaxSuffix(index * 2, l, m, ql);
    int tmpR = nodes[index * 2 + 1].maxSuffix;
    if (getRangeSum(tmpL, r) < getRangeSum(tmpR, r)) {
      return tmpR;
    } else {
      return tmpL;
    }
  } else {
    // ql > m
    return queryMaxSuffix(index * 2 + 1, m + 1, r, ql);
  }
}

pair<int, int> queryMaxSub(int root, int l, int r, int ql, int qr) {
  int m = l + (r - l) / 2;

  if (ql <= l && r <= qr) {
    return nodes[root].maxSub;
  }
  if (qr <= m) {
    return queryMaxSub(root * 2, l, m, ql, qr);
  } else if (ql > m) {
    return queryMaxSub(root * 2 + 1, m + 1, r, ql, qr);
  } else {
    pair<int, int> pair0 = queryMaxSub(root * 2, l, m, ql, qr);
    ll v0 = getRangeSum(pair0.first, pair0.second);

    pair<int, int> pair1 = queryMaxSub(root * 2 + 1, m + 1, r, ql, qr);
    ll v1 = getRangeSum(pair1.first, pair1.second);

    int tmpL = queryMaxSuffix(root * 2, l, m, ql);
    int tmpR = queryMaxPrefix(root * 2 + 1, m + 1, r, qr);
    pair<int, int> pair2 = make_pair(tmpL, tmpR);
    ll v2 = getRangeSum(pair2.first, pair2.second);
    
    ll tmpV = v0;
    pair<int, int> result = pair0;

    if (tmpV < v1) {
      tmpV = v1;
      result = pair1;
    } else if (tmpV == v1 && pair1 < result) {
      result = pair1;
    }
    if (tmpV < v2) {
      tmpV = v2;
      result = pair2;
    } else if (tmpV == v2 && pair2 < result) {
      result = pair2;
    }
    return result;
  }
}

void setValues(int index, int l, int r) {
  int m = l + (r - l) / 2;
  if (l == r) {
    nodes[index].maxPrefix = l;
    nodes[index].maxSuffix = l;
    nodes[index].maxSub = make_pair(l, l);
    return;
  }

  setValues(index * 2, l, m);
  setValues(index * 2 + 1, m + 1, r);

  // Set maxPrefix.
  int leftMaxPrefix = nodes[index * 2].maxPrefix;
  int rightMaxPrefix = nodes[index * 2 + 1].maxPrefix;
  if (getRangeSum(l, rightMaxPrefix) > getRangeSum(l, leftMaxPrefix)) {
    nodes[index].maxPrefix = rightMaxPrefix;
  } else {
    nodes[index].maxPrefix = leftMaxPrefix;
  }

  // Set maxSuffix.
  int leftMaxSuffix = nodes[index * 2].maxSuffix;
  int rightMaxSuffix = nodes[index * 2 + 1].maxSuffix;
  if (getRangeSum(rightMaxSuffix, r) > getRangeSum(leftMaxSuffix, r)) {
    nodes[index].maxSuffix = rightMaxSuffix;
  } else {
    nodes[index].maxSuffix = leftMaxSuffix;
  }

  // Set maxSub.
  pair<int, int> pair0 = nodes[index * 2].maxSub;
  ll v0 = getRangeSum(pair0.first, pair0.second);

  pair<int, int> pair1 = nodes[index * 2 + 1].maxSub;
  ll v1 = getRangeSum(pair1.first, pair1.second);

  int tmpL = nodes[index * 2].maxSuffix;
  int tmpR = nodes[index * 2 + 1].maxPrefix;
  pair<int, int> pair2 = make_pair(tmpL, tmpR);
  ll v2 = getRangeSum(pair2.first, pair2.second);
  
  ll tmpV = v0;
  nodes[index].maxSub = pair0;

  if (tmpV < v1) {
    tmpV = v1;
    nodes[index].maxSub = pair1;
  } else if (tmpV == v1 && pair1 < nodes[index].maxSub) {
    nodes[index].maxSub = pair1;
  }
  if (tmpV < v2) {
    tmpV = v2;
    nodes[index].maxSub = pair2;
  } else if (tmpV == v2 && pair2 < nodes[index].maxSub) {
    nodes[index].maxSub = pair2;
  }
}

int main() {
  int testCase = 0;
  while (scanf("%d %d", &dishes, &queries) == 2) {
    printf("Case %d:\n", ++testCase);
    for (int i = 1; i <= dishes; ++i) {
      scanf("%lld", &values[i]);
    }

    sumArray[0] = 0;
    for (int i = 1; i<= dishes; ++i) {
      sumArray[i] = sumArray[i - 1] + values[i];
    }

    setValues(1, 1, dishes);

    for (int i = 0; i < queries; ++i) {
      int l, r;
      scanf("%d %d", &l, &r);
      pair<int, int> result = queryMaxSub(1, 1, dishes, l, r);
      printf("%d %d\n", result.first, result.second);
    }
  }
  return 0;
}