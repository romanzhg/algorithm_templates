/**
 *
 */
#include "common.h"

class Solution {
 public:
  int smallestFactorization(int a) {
    map<int, int> digits;
    int count = 0;
    for (int i = 9; i >= 1; i--) {
      if (i == 1) {
        // current number is a prime, if the number is not 1 by now, then there is no solution.
        return 0;
      }
      while (a % i == 0) {
        digits[i]++;
        count++;
        a = a / i;
      }
      if (a == 1) {
        break;
      }
    }
    if (count > 10) {
      return 0;
    }
    long rtn = 0;
    for (int i = 2; i <= 9; i++) {
      while (digits[i] > 0) {
        rtn = rtn * 10 + i;
        digits[i]--;
      }
    }
    long limit = INT_MAX;
    if (rtn > limit) {
      return 0;
    }
    return (int) rtn;
  }
};