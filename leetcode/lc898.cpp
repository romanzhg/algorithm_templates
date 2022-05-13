/**
 * prev here is the numbers reachable by subarrays ending at the previous number.
 * Since the number of 1s is strictly increasing, the count of numbers in the "prev" set won't exceed 32.
 *
 */

#include "common.h"

class Solution {
 public:
  int subarrayBitwiseORs(vector<int>& A) {
    set<int> all;
    set<int> prev;
    for (int v : A) {
      set<int> new_set;
      for (int p : prev) {
        new_set.insert(p | v);
      }
      new_set.insert(v);
      prev = new_set;
      all.insert(new_set.begin(), new_set.end());
    }
    return all.size();
  }
};