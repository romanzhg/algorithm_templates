/**
 * Greedy, 2 types of matches.
 * Type 1 unmatch: "x,y", type 2 unmatch: "y,x".
 * Two unmatches of the same type: 1 swap.
 * Two unmatches of different types: 2 swaps.
 * So we should always match unmatches of the same type first.
 */
#include "common.h"

class Solution {
 public:
  int minimumSwap(string s1, string s2) {
    int type_1_count = 0, type_2_count = 0;
    for (int i = 0; i < s1.size(); i++) {
      if (s1[i] == s2[i]) {
        continue;
      }
      if (s1[i] == 'x') {
        type_1_count++;
      } else {
        type_2_count++;
      }
    }
    int rtn = 0;
    rtn += type_1_count / 2;
    rtn += type_2_count / 2;
    type_1_count = type_1_count % 2;
    type_2_count = type_2_count % 2;
    if (type_1_count == type_2_count) {
      rtn += type_1_count * 2;
      return rtn;
    } else {
      return -1;
    }
  }
};