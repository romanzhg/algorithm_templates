/**
 * Note the two phase allocation.
 */
#include "common.h"

class Solution {
 public:
  vector<vector<int>> reconstructMatrix(int upper, int lower, vector<int>& colsum) {
    int tmp_sum = 0;
    for (int v : colsum) {
      tmp_sum += v;
    }
    if (tmp_sum != upper + lower) {
      return {};
    }
    vector<vector<int>> rtn(2, vector<int>(colsum.size(), 0));

    for (int col_index = 0; col_index < colsum.size(); col_index++) {
      if (colsum[col_index] == 2) {
        if (upper == 0 || lower == 0) {
          return {};
        }
        rtn[0][col_index] = 1;
        rtn[1][col_index] = 1;
        upper--;
        lower--;
      }
    }
    for (int col_index = 0; col_index < colsum.size(); col_index++) {
      if (colsum[col_index] == 1) {
        if (upper) {
          rtn[0][col_index] = 1;
          upper--;
        } else {
          if (lower) {
            rtn[1][col_index] = 1;
            lower--;
          } else {
            return {};
          }
        }
      }
      if (upper == 0 && lower == 0) {
        break;
      }
    }

    return rtn;
  }
};