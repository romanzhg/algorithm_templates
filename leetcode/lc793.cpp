/**
 * Count the number of 5s in the factorial, number of 5s equals to the number of 0s in the result.
 * Binary search for a value matched k, the result will be a 5 value range.
 * f(x) = x!
 * g(f(x)) -> count the 0s in f(x): while x != 0, rtn += x / 5; x = x / 5;
 * Another way to write g:
 * g(f(x)) = floor(x/5) + floor(x/25) + floor(x/125) ...
 * g(f(x)) ~~ x/4.
 * since K <= 1e9, we only need to check x <= 1e10.
 *
 * In the binary search, low is possible value, high is impossible value, because the / 2 is leaning below,
 * when setting low it should be "low = mid + 1;".
 *
 */
#include "common.h"

class Solution {
 public:
  long long GetZeros(long long v) {
    long long rtn = 0;
    while (v != 0) {
      rtn += v / 5;
      v = v / 5;
    }
    return rtn;
  }

  int preimageSizeFZF(int K) {
    long long low = 0, high = 1e10;
    while (low < high) {
      long mid = (low + high) / 2;
      long long v = GetZeros(mid);
      if (v == K) {
        return 5;
      }
      if (v > K) {
        high = mid;
      } else {
        low = mid + 1;
      }
    }
    return 0;
  }
};