/**
 * Simple dp.
 */

#include "common.h"

class Solution {
 public:

  int helper(vector<int>& dp, int cur, const vector<int>& days, const vector<int>& costs) {
    if (cur > 365) {
      return 0;
    }
    if (dp[cur] > -1) {
      return dp[cur];
    }
    if (find(days.begin(), days.end(), cur) == days.end()) {
      dp[cur] = helper(dp, cur + 1, days, costs);
      return dp[cur];
    } else {
      int rtn = INT_MAX;
      rtn = min(rtn, helper(dp, cur + 1, days, costs) + costs[0]);
      rtn = min(rtn, helper(dp, cur + 7, days, costs) + costs[1]);
      rtn = min(rtn, helper(dp, cur + 30, days, costs) + costs[2]);
      dp[cur] = rtn;
      return dp[cur];
    }
  }

  int mincostTickets(vector<int>& days, vector<int>& costs) {
    // dp[i]: total min cost to satisfy all the travels after and on day i.
    vector<int> dp(366, -1);

    helper(dp, 1, days, costs);

    return dp[1];
  }
};