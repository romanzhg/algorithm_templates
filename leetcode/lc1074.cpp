/**
 * Convert to get the subarray sum of an one dimentional array.
 */
#include "common.h"

class Solution {
 public:
  int GetNumberOfSums(const vector<int>& a, int target) {
    int prefix_sum = 0;
    map<int, int> value_to_count;

    int rtn = 0;
    value_to_count[0] = 1;
    for (int v : a) {
      prefix_sum += v;
      rtn += value_to_count[prefix_sum - target];
      value_to_count[prefix_sum]++;
    }
    return rtn;
  }

  int numSubmatrixSumTarget(vector<vector<int>>& matrix, int target) {
    int rows = matrix.size();
    int cols = matrix[0].size();

    for (int c = 0; c < cols; c++) {
      for (int r = 1; r < rows; r++) {
        matrix[r][c] += matrix[r - 1][c];
      }
    }

    int rtn = 0;
    for (int j = 0; j < rows; j++) {
      rtn += GetNumberOfSums(matrix[j], target);
      for (int i = 0; i < j; i++) {
        vector<int> tmp = matrix[j];
        for (int k = 0; k < cols; k++) {
          tmp[k] = tmp[k] - matrix[i][k];
        }
        rtn += GetNumberOfSums(tmp, target);
      }
    }
    return rtn;
  }
};