/**
 * Note the condition of divisible.
 */
#include "common.h"

class Solution {
 public:
  int subarraysDivByK(vector<int>& A, int K) {
    vector<int> mod(K, 0);
    int prefix_sum = 0;
    mod[prefix_sum]++;
    int rtn = 0;
    for (int v : A) {
      prefix_sum += v;
      if (prefix_sum >= 0) {
        rtn += mod[prefix_sum % K];
        mod[prefix_sum % K]++;
      } else {
        rtn += mod[(prefix_sum % K + K) % K];
        mod[(prefix_sum % K + K) % K]++;
      }
    }
    return rtn;
  }
};