/**
 * Two pointers.
 */
#include "common.h"

class Solution {
 public:
  vector<int> shortestToChar(string S, char C) {
    int l = 0, r = 0;
    vector<int> rtn(S.size(), 0);

    bool found_first = false;
    int counter = 0;
    while (r < S.size()) {
      if (S[r] == C) {
        if (!found_first) {
          int tmp_r = r - 1;
          int tmp_l = 0;
          while (tmp_l < tmp_r) {
            swap(rtn[tmp_l], rtn[tmp_r]);
            tmp_l++;
            tmp_r--;
          }
          found_first = true;
        } else {
          int tmp_r = r - 1;
          int tmp_l = l;
          while (tmp_l < tmp_r) {
            rtn[tmp_r] = rtn[tmp_l];
            tmp_l++;
            tmp_r--;
          }
        }
        rtn[r] = 0;
        l = r + 1;
        counter = 0;
      } else {
        rtn[r] = ++counter;
      }
      r++;
    }
    return rtn;
  }
};