/**
 * Binary search to minimize the max value.
 * Note for double values with the stop condition as a diff range, we don't need to add +1/-1 to the medium, it will
 * always stop.
 * But there is no harm to add one value to min(the invalid value), for example, in this implementation,
 * "min_v = m + 1e-7;".
 */
#include "common.h"

class Solution {
 public:
  bool IsValid(vector<int>& stations, int K, double dist) {
    double prev = stations[0];
    int index = 1;
    while (index < stations.size()) {
      if (prev + dist < stations[index]) {
        if (K > 0) {
          prev = prev + dist;
          K--;
        } else {
          return false;
        }
      } else {
        prev = stations[index];
        index++;
      }
    }
    return true;
  }

  double minmaxGasDist(vector<int>& stations, int K) {
    double min_v = 0, max_v = 0;
    for (int i = 0; i < stations.size() - 1; i++) {
      max_v = max(max_v, (double)stations[i + 1] - stations[i]);
    }

    while (max_v - min_v > 1e-6) {
      double m = (max_v + min_v) / 2.0;
      if (IsValid(stations, K, m)) {
        max_v = m;
      } else {
        min_v = m + 1e-7;
        // This is also valid: min_v = m;
      }
    }
    return min_v;
  }
};