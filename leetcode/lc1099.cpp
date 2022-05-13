/**
 * Two points is also ok, should present the reasoning.
 */
class Solution {
 public:
  int twoSumLessThanK(vector<int>& A, int K) {
    int rtn = -1;
    for (int i = 0; i < A.size(); i++) {
      for (int j = i + 1; j < A.size(); j++) {
        int tmp_sum = A[i] + A[j];
        if (tmp_sum >= K) {
          continue;
        } else {
          rtn = max(rtn, tmp_sum);
        }
      }
    }
    return rtn;
  }
};