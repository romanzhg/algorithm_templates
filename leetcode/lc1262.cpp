/**
 * Solution2 is the ref, note it only consider the values.
 *
 * Solution checks if the prev values are valid.
 *
 * Solution1 only consider new values.
 */
#include "common.h"

class Solution {
 public:
  int maxSumDivThree(vector<int>& nums) {
    // Max value with remainder 0, 1, 2.
    vector<int> dp = {0, 0, 0};

    for (int v : nums) {
      vector<int> tmp = dp;
      for (int i = 0; i < 3; i++) {
        if (tmp[i] % 3 != i) {
          continue;
        }
        int new_s = tmp[i] + v;
        dp[new_s % 3] = max(tmp[new_s % 3], new_s);
      }
    }
    return dp[0];
  }



};

class Solution1 {
  int maxSumDivThree(vector<int>& nums) {
    // Max value with remainder 0, 1, 2.
    vector<int> dp = {0, 0, 0};

    for (int v : nums) {
      vector<int> tmp = dp;
      for (int i = 0; i < 3; i++) {
        int new_s = tmp[i] + v;
        dp[new_s % 3] = max(dp[new_s % 3], new_s);
      }
    }
    return dp[0];
  }

};

class Solution2 {
  int maxSumDivThree(vector<int>& A) {
    vector<int> dp = {0, 0, 0}, dp2;
    for (int a : A) {
      dp2 = dp;
      for (int i: dp2) {
        dp[(i + a) % 3] = max(dp[(i + a) % 3], i + a);
      }
    }
    return dp[0];
  }
};