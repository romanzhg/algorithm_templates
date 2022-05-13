/**
 * Note the index.
 */
#include "common.h"
class Solution {
 public:
  vector<vector<int>> matrixBlockSum(vector<vector<int>>& mat, int K) {
    int rows = mat.size();
    int cols = mat[0].size();
    vector<vector<int>> tmp(rows + 1, vector<int>(cols + 1, 0));
    for (int i = 1; i <= rows; i++) {
      for (int j = 1; j <= cols; j++) {
        tmp[i][j] = tmp[i - 1][j] + tmp[i][j - 1] - tmp[i - 1][j - 1] + mat[i - 1][j - 1];
      }
    }

    vector<vector<int>> rtn(rows, vector<int>(cols, 0));
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        int r_up = max(0, i - K);
        int r_down = min(rows - 1, i + K);
        int c_left = max(0, j - K);
        int c_right = min(cols - 1, j + K);
        rtn[i][j] = tmp[r_down + 1][c_right + 1]
            - tmp[r_down + 1][c_left]
            - tmp[r_up][c_right + 1]
            + tmp[r_up][c_left];
      }
    }
    return rtn;
  }
};