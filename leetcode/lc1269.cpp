/**
 * Counting, note that arrLen can be much larger than steps, so there are a few ways to improve
 * the performance.
 *
 * A value at dp[x] takes x steps to affect value at dp[0].
 *
 * Solution/Solution1 can be further improved by extend/shrink the range of the j loop.
 *
 */
#include "common.h"

class Solution {
  const int kMod = 1e9 + 7;
 public:
  int numWays(int steps, int arrLen) {
    if (arrLen == 1) {
      return 1;
    }

    vector<int> dp(arrLen, 0);
    dp[0] = 1;

    for (int i = 0; i < steps; i++) {
      int dp_prev = dp[0];
      dp[0] = (dp[0] + dp[1]) % kMod;
      for (int j = 1; j < arrLen - 1; j++) {
        int tmp = dp[j];
        dp[j] = ((dp_prev + dp[j]) % kMod + dp[j + 1]) % kMod;
        dp_prev = tmp;
      }
      dp[arrLen - 1] = (dp[arrLen - 1] + dp_prev) % kMod ;
    }

    return dp[0];
  }
};


/*****************************************************************************/

int dp[300];

class Solution1 {
  const int kMod = 1e9 + 7;
 public:
  int numWays(int steps, int arrLen) {
    if (arrLen == 1) {
      return 1;
    }

    memset(dp, 0, sizeof(dp));
    dp[0] = 1;

    for (int i = 0; i < steps; i++) {
      int dp_prev = dp[0];
      dp[0] = (dp[0] + dp[1]) % kMod;
      for (int j = 1; j < min(arrLen - 1, steps / 2 + 1); j++) {
        int tmp = dp[j];
        dp[j] = ((dp_prev + dp[j]) % kMod + dp[j + 1]) % kMod;
        dp_prev = tmp;
      }
      if (arrLen - 1 < 300) {
        dp[arrLen - 1] = (dp[arrLen - 1] + dp_prev) % kMod ;
      }
    }

    return dp[0];
  }
};