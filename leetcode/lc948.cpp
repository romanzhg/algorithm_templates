/**
 * Note some edge conditions.
 */

#include "common.h"
class Solution {
 public:
  int bagOfTokensScore(vector<int>& tokens, int P) {
    sort(tokens.begin(), tokens.end());
    int points = 0;
    int l_ptr = 0, r_ptr = tokens.size() - 1;
    while (r_ptr >= l_ptr) {
      if (P < tokens[l_ptr]) {
        break;
      }
      while (l_ptr <= r_ptr && P >= tokens[l_ptr]) {
        P -= tokens[l_ptr];
        l_ptr++;
        points++;
      }

      if (r_ptr - l_ptr >= 1) {
        P += tokens[r_ptr];
        r_ptr--;
        points--;
      }
    }
    return points;
  }
};