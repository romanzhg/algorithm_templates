/**
 * A hard greedy problem, the point is to search for the splitting point.
 * There is a point, before it, everything is +K, after it, everything is -K.
 */
#include "common.h"

class Solution {
 public:
  int smallestRangeII(vector<int>& A, int K) {
    sort(A.begin(), A.end());
    int size = A.size();
    int ans = A[size - 1] - A[0];

    for (int i = 0; i < size - 1; i++) {
      int a = A[i], b = A[i + 1];
      int high = max(A[size - 1] - K, a + K);
      int low = min(A[0] + K, b - K);
      ans = min(ans, high - low);
    }
    return ans;
  }
};