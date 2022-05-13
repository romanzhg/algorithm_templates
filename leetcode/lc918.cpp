/**
 * Two cases, one is normal max subarray, the other is max continues sum from front and back.
 *
 */
#include "common.h"

class Solution {
 public:
  int maxSubarraySumCircular(vector<int>& A) {
    int rtn = INT_MIN;
    int prev_sum = 0;

    for (int i = 0; i < A.size(); i++) {
      if (prev_sum <= 0) {
        prev_sum = A[i];
      } else {
        prev_sum = A[i] + prev_sum;
      }
      rtn = max(rtn, prev_sum);
    }

    if (A.size() > 1) {
      vector<int> backward_max(A.size(), INT_MIN);
      backward_max.back() = A.back();
      int tmp = A.back();
      for (int i = A.size() - 2; i >= 0; i--) {
        tmp += A[i];
        backward_max[i] = max(tmp, backward_max[i + 1]);
      }

      prev_sum = 0;
      for (int i = 0; i < A.size() - 2; i++) {
        prev_sum += A[i];
        rtn = max(rtn, prev_sum + backward_max[i + 2]);
      }
    }

    return rtn;
  }
};