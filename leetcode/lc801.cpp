/**
 * Use dp to check all the swap/not swap situation.
 * dp[i][0/1] after solving the first i chars in the original string, and swap/not swap at the last char,
 * the min total cost.
 * Note since there is always a solution, there won't be overflow for INT_MAX + 1: dp[*][0] and dp[*][1]
 * won't be INT_MAX, since there must be a route to the current subproblem with state.
 */
#include "common.h"
class Solution {
 public:
  bool match(int i, int c, int c_prev, const vector<int>& A, const vector<int>& B) {
    int a0 = A[i - 1], a1 = A[i], b0 = B[i - 1], b1 = B[i];
    if (c) {
      swap(a1, b1);
    }
    if (c_prev) {
      swap(a0, b0);
    }
    return a1 > a0 && b1 > b0;

  }

  int minSwap(vector<int>& A, vector<int>& B) {
    vector<vector<int>> dp(A.size() + 1, vector<int>(2, INT_MAX));

    dp[1][0] = 0;
    dp[1][1] = 1;
    for (int i = 1; i < A.size(); i++) {
      if (match(i, 0, 0, A, B)) {
        dp[i + 1][0] = min(dp[i + 1][0], dp[i][0]);
      }
      if (match(i, 0, 1, A, B)) {
        dp[i + 1][0] = min(dp[i + 1][0], dp[i][1]);
      }
      if (match(i, 1, 0, A, B)) {
        dp[i + 1][1] = min(dp[i + 1][1], dp[i][0] + 1);
      }
      if (match(i, 1, 1, A, B)) {
        dp[i + 1][1] = min(dp[i + 1][1], dp[i][1] + 1);
      }
    }

    return min(dp[A.size()][0], dp[A.size()][1]);
  }
};