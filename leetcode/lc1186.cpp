/**
 * Note the requirement that the subarray cannot be empty.
 *
 */
#include "common.h"

class Solution {
 public:
  int maximumSum(vector<int>& arr) {
    vector<vector<int>> dp(arr.size(), vector<int>(2));
    int rtn;

    dp[0][0] = arr[0];
    dp[0][1] = INT_MIN / 2;
    rtn = dp[0][0];

    for (int i = 1; i < arr.size(); i++) {
      dp[i][0] = max(0, dp[i - 1][0]) + arr[i];
      dp[i][1] = max(dp[i - 1][0], dp[i - 1][1] + arr[i]);
      rtn = max(rtn, max(dp[i][0], dp[i][1]));
    }
    return rtn;
  }
};