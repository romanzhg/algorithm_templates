#include "common.h"

class Solution {
 public:
  int minFallingPathSum(vector<vector<int>>& arr) {
    int matrix_size = arr.size();
    vector<int> falling_sum_prev = arr[0];

    for (int i = 1; i < matrix_size; i++) {
      vector<int> tmp_falling_sum = arr[i];
      for (int j = 0; j < matrix_size; j++) {

        /****************************************************/
        // TODO: a little improvement here, we only need to record
        // the min value, its index, the second min value.
        // If j equals to the min value index, use the second min value.
        // Otherwise use the min value.
        // So the for loop below can be moved out of the j loop, and for each
        // "falling_sum_prev" array, we only process once.
        int min_prev = INT_MAX;
        for (int k = 0; k < matrix_size; k++) {
          if (j != k) {
            min_prev = min(min_prev, falling_sum_prev[k]);
          }
        }
        /****************************************************/

        tmp_falling_sum[j] += min_prev;
      }
      falling_sum_prev = tmp_falling_sum;
    }

    int min_value = INT_MAX;
    for (int v : falling_sum_prev) {
      min_value = min(min_value, v);
    }
    return min_value;
  }
};