/**
 * Since l is returned, invalid means l should get increamented by 1 from the invalid value.
 * Answer is in range [low, high](both inclusive).
 */
#include "common.h"

class Solution {
 public:
  bool IsValid(int s, const vector<int>& piles, int H) {
    int c = 0;
    for (int p : piles) {
      c += p / s;
      c += ((p % s) > 0) ? 1 : 0;
      if (c > H) {
        return false;
      }
    }
    return true;
  }

  int minEatingSpeed(vector<int>& piles, int H) {
    int l = 1, h = 1e9;
    while (l < h) {
      int m = (l + h) / 2;
      if (IsValid(m, piles, H)) {
        h = m;
      } else {
        l = m + 1;
      }
    }
    return l;
  }
};