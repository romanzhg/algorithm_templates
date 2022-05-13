/**
 * Greedy, flip the smallest value every time.
 * Use the fact that a map is implemented as a tree, and map.begin() gives a pointer to the smallest element.
 *
 */
#include "common.h"

class Solution {
 public:
  int largestSumAfterKNegations(vector<int>& A, int K) {
    map<int, int> value_to_count;
    for (int v : A) {
      value_to_count[v]++;
    }
    for (int i = 0; i < K; i++) {
      auto kv = *value_to_count.begin();
      value_to_count[kv.first]--;
      if (value_to_count[kv.first] == 0) {
        value_to_count.erase(kv.first);
      }
      value_to_count[-kv.first]++;
    }

    int rtn = 0;
    for (const auto& kv : value_to_count) {
      rtn += kv.first * kv.second;
    }
    return rtn;
  }
};