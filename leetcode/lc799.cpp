/**
 * Function helper(r, c, x) returns how much water the cup has(if no pour), after x bottles are used.
 *
 * Solution1 is more intuitive.
 */
#include "common.h"

class Solution {
 public:
  map<pair<int, int>, double> results;
  double helper(int r, int c, int x) {
    if (r == 0) {
      return x;
    }
    pair<int, int> key{r, c};
    if (results.find(key) != results.end()) {
      return results[key];
    }
    int left = c - 1, right = c;
    double rtn = 0;
    if (left >= 0) {
      rtn += max((double)0, helper(r - 1, left, x) - (double)1) / 2;
    }
    if (right < r) {
      rtn += max((double)0, helper(r - 1, right, x) - (double)1) / 2;
    }
    results[key] = rtn;
    return rtn;
  }

  double champagneTower(int poured, int query_row, int query_glass) {
    results.clear();
    return min(helper(query_row, query_glass, poured), (double)1);
  }
};

class Solution {
 public:
  double champagneTower(int poured, int query_row, int query_glass) {
    double amount[102][102];
    memset(amount, 0, sizeof(amount));
    amount[0][0] = poured;
    for (int r = 0; r <= query_row; r++) {
      for (int c = 0; c <= query_glass; c++) {
        // The amount pours to next level from the current glass.
        double p = amount[r][c] - 1;
        if (p > 0) {
          amount[r + 1][c] += p / 2;
          amount[r + 1][c + 1] += p / 2;
        }
      }
    }
    return min((double)1, amount[query_row][query_glass]);
  }
};