/**
 * Solution is O(n ^ 3) and TLE.
 * In Solution, dp[i][j] refers to when reached point i with j fills, the maximum remaining feul.
 * Added the start point and end point as special stations.
 *
 * Solution1 is O(n ^ 3) passed the test case.
 * It changed the order of iterating through i, j.
 * dp[i][j] refers to the max feul when the car reached point i(but did not take the feul at it)
 * with j fills.
 * This solution is kind of greedy.
 *
 * Solution2 is O(n ^ 2).
 * dp[i] means after i fills, the farest distance the car can reach.
 * And the problem is divided into subproblems, with first i/i + 1 gas stations considered.
 * Whenever a new gas station is added, only dp[i] larger than the gas station position can
 * take it, and after taking it we update to dp[i + 1].
 *
 * Solution2 is the reference solution. The lesson learned here is to think of what subproblem
 * structure to exploit, in addition to thinking about the state/additional state and transition.
 *
 */
#include "common.h"

class Solution {
 public:
  int minRefuelStops(int target, int startFuel, vector<vector<int>> &stations) {
    vector<vector<int>> tmp_stations;

    tmp_stations.push_back({0, startFuel});
    for (const vector<int> &s : stations) {
      if (s[0] < target) {
        tmp_stations.push_back(s);
      } else {
        break;
      }
    }
    tmp_stations.push_back({target, 0});

    int size = tmp_stations.size();
    vector<vector<long>> dp(size + 1, vector<long>(size + 1, -1));
    dp[0][1] = startFuel;

    for (int i = 0; i < size - 1; i++) {
      for (int j = 0; j < size; j++) {
        if (dp[i][j] == -1) {
          continue;
        }
        for (int k = i + 1; k < size; k++) {
          int dist = tmp_stations[k][0] - tmp_stations[i][0];
          if (dp[i][j] >= dist) {
            dp[k][j] = max(dp[k][j], dp[i][j] - dist);
            dp[k][j + 1] = max(dp[k][j + 1], dp[i][j] - dist + tmp_stations[k][1]);
          } else {
            break;
          }
        }
      }
    }
    int rtn = INT_MAX;
    for (int i = 0; i < size; i++) {
      rtn = min(rtn, dp[size - 1][i] >= 0 ? i : INT_MAX);
    }
    return rtn == INT_MAX ? -1 : rtn - 1;
  }
};

class Solution1 {
 public:
  int minRefuelStops(int target, int startFuel, vector<vector<int>> &stations) {
    vector<vector<int>> tmp_stations;

    tmp_stations.push_back({0, startFuel});
    for (const vector<int> &s : stations) {
      if (s[0] < target) {
        tmp_stations.push_back(s);
      } else {
        break;
      }
    }

    int size = tmp_stations.size();
    vector<vector<long>> dp(size + 1, vector<long>(size + 1, -1));
    dp[0][0] = 0;

    for (int j = 0; j < size; j++) {
      for (int i = 0; i < size; i++) {
        if (dp[i][j] == -1) {
          continue;
        }
        int fuel = tmp_stations[i][1];
        if (fuel + dp[i][j] >= target) {
          return j;
        }
        for (int k = i + 1; k < size; k++) {
          if (fuel + dp[i][j] >= tmp_stations[k][0]) {
            dp[k][j + 1] = max(dp[k][j + 1], fuel + dp[i][j]);
          } else {
            break;
          }
        }
      }
    }

    return -1;
  }
};

class Solution2 {
 public:
  int minRefuelStops(int target, int startFuel, vector<vector<int>>& stations) {
    vector<long> dp(stations.size() + 1, -1);
    dp[0] = startFuel;

    for (int i = 0; i < stations.size(); i++) {
      for (int t = i; t >= 0; t--) {
        if (dp[t] >= stations[i][0]) {
          dp[t + 1] = max(dp[t + 1], (long)dp[t] + stations[i][1]);
        }
      }
    }

    for (int i = 0; i <= stations.size(); i++) {
      if (dp[i] >= target) {
        return i;
      }
    }
    return -1;
  }
};