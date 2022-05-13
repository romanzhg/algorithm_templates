/**
 * DP. dp[i][j] maximum valued tree build from the input array [i, j].
 *
 * There is a greedy way, choose the smallest and second smallest leaf, build an internal node,
 * after we build a node, the smaller leaf is no longer used
 * and the larger leaf will be used.
 * but I don't know how to prove it, copied from discussion:
class Solution:
    def mctFromLeafValues(self, arr: List[int]) -> int:
        res = 0
        while len(arr) > 1:
            mini_idx = arr.index(min(arr))
            if 0 < mini_idx < len(arr) - 1:
                res += min(arr[mini_idx - 1], arr[mini_idx + 1]) * arr[mini_idx]
            else:
                res += arr[1 if mini_idx == 0 else mini_idx - 1] * arr[mini_idx]
            arr.pop(mini_idx)
        return res
 */

#include "common.h"

class Solution {
 public:
  int GetMaxInRange(const vector<int>& arr, int i, int j) {
    int rtn = 0;
    for (int index = i; index <= j; index++) {
      rtn = max(rtn, arr[index]);
    }
    return rtn;
  }

  int mctFromLeafValues(vector<int>& arr) {
    int arr_len = arr.size();
    vector<vector<int>> max_in_range(arr_len, vector<int>(arr_len, 0));
    for (int i = 0; i < arr_len; i++) {
      for (int j = i; j < arr_len; j++) {
        max_in_range[i][j] = GetMaxInRange(arr, i, j);
      }
    }

    vector<vector<int>> dp(arr_len, vector<int>(arr_len, 0));

    for (int len = 2; len <= arr_len; len++) {
      for (int i = 0; i + len <= arr_len; i++) {
        int j = i + len - 1;
        int v = INT_MAX;
        for (int k = i + 1; k <= j; k++) {
          v = min(v, max_in_range[i][k - 1] * max_in_range[k][j] + dp[i][k - 1] + dp[k][j]);
        }
        dp[i][j] = v;
      }
    }
    return dp[0][arr_len - 1];
  }
};