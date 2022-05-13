// 1. There are at most 128 elements, the number of all combinations
// is 2^128, there cannot be an array for this many elements.
// Map of set is used here instead of the dp matrix.
// set<int> can be replaced by a string of length 128
// (or a bit array of length 128).
// The combinations of interest has a much smaller size than
// all combinations.
// 
// 2. On different branch, there could be different strategy.
//
// 3. The problem is: put elements at the leaf of a decision tree, what's
// the min tree height. 
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

#define MAX_OBJECTS 150
#define MAX_FEATURES 15

int featureCount, objectCount;
map<set<int>, int> dp;
bool features[MAX_OBJECTS][MAX_FEATURES];

// For elements in curSet, if any of them differs on feature i.
bool differs(const set<int>& curSet, int i) {
  int trueCount = 0;
  int falseCount = 0;
  for (int elem : curSet) {
    if (features[elem][i]) {
      trueCount++;
    } else {
      falseCount++;
    }
  }
  return trueCount * falseCount != 0;
}

int getMinValue(const set<int>& curSet) {
  if (dp.find(curSet) != dp.end()) {
    return dp.find(curSet)->second;
  }
  if (curSet.size() == 1) {
    return 0;
  } else if (curSet.size() == 2) {
    return 1;
  } else if (curSet.size() == 3) {
    return 2;
  }

  int rtn = INF;
  for (int i = 0; i < featureCount; i++) {
    if (differs(curSet, i)) {
      set<int> falseSet;
      set<int> trueSet;

      for (int elem : curSet) {
        if (features[elem][i]) {
          trueSet.insert(elem);
        } else {
          falseSet.insert(elem);
        }   
      }
      rtn = min(rtn, max(getMinValue(trueSet), getMinValue(falseSet)) + 1);
    }
  }
  dp[curSet] = rtn;
  return rtn;
}

void longToFeatureArray(uint64_t value, bool* f, int size) {
  for (int i = 0; i < size; i++) {
    f[i] = value % 10;
    value = value / 10;
  }
}

int main() {
  while (scanf("%d %d", &featureCount, &objectCount) == 2
    && (featureCount + objectCount) > 0) {

    set<int> initSet;
    dp.clear();
    for (int i = 0; i < objectCount; i++) {
      uint64_t obj;
      scanf("%llu", &obj);
      longToFeatureArray(obj, features[i], featureCount);
      initSet.insert(i);
    }

    printf("%d\n", getMinValue(initSet));
  }
  return 0;
}