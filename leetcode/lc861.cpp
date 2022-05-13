/**
 * The important thing for each row is the first bit, everything else added cannot overweight the first bit.
 * All 0 and all 1 is the same, so for the first col we check this two conditions.
 * Column sum is the largest possible 1s, which is either (1 count) or (<size> - 1 count).
 */
#include "common.h"
class Solution {
 public:
  int rows, cols;
  void flip(vector<int>& vec) {
    for (int i = 0; i < vec.size(); i++) {
      vec[i] = vec[i] ^ 1;
    }
  }

  int GetSum(vector<vector<int>>& mat) {
    int value = 0;
    for (int i = 0; i < cols; i++) {
      int count = 0;
      for (int j = 0; j < rows; j++) {
        count += mat[j][i];
      }
      int multiplier = 1 << (cols - 1 - i);
      value += max(count, rows - count) * multiplier;
    }
    return value;
  }

  int matrixScore(vector<vector<int>>& A) {
    rows = A.size();
    cols = A[0].size();

    vector<vector<int>> B = A;
    for (int i = 0; i < rows; i++) {
      if (A[i][0] == 0) {
        flip(A[i]);
      }
    }
    for (int i = 0; i < rows; i++) {
      if (B[i][0] == 1) {
        flip(B[i]);
      }
    }

    int rtn = max(GetSum(A), GetSum(B));
    return rtn;
  }
};